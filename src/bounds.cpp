#include <algorithm>
#include <array>
#include <deque>
#include <memory>
#include <vector>

#include "../include/common.hpp"

constexpr int RIGHT = 0b100;
constexpr int TOP = 0b010;
constexpr int FRONT = 0b001;

using namespace YAAACD;

/**
 * @brief Check if 2 AABBs intersect.
 *
 * @param box AABB to check against
 * @return true if boxes intersect
 * @return false if boxes don't intersect
 */
bool BoundingBox::intersects(const BoundingBox &box) const {
    if (std::any_of(
            this->_corners.begin(),
            this->_corners.end(),
            [&box](const Vertex &vertex) {
                return box.contains(vertex);
            }
        ))
        return true;

    const std::array<Vertex, 8> &box_corners = box.corners();

    return std::any_of(
        box_corners.begin(),
        box_corners.end(),
        [&](const Vertex &vertex) {
            return this->contains(vertex);
        }
    );
}

/**
 * @brief Check if AABB contains any of the triangle's vertices.
 *
 * @param triangle triangle to check
 * @return true if triangle is inside a bounding box
 * @return false if triangle is not inside a bounding box
 */
bool BoundingBox::contains(const Triangle &triangle) const {
    return std::any_of(
        triangle.begin(),
        triangle.end(),
        [&](const Vertex &vertex) {
            return this->contains(vertex);
        }
    );
}

/**
 * @brief Check if AABB contains a vertex.
 *
 * @param vertex vertex to check
 * @return true if AABB contains the vertex
 * @return false if AABB doesn't contain the vertex
 */
const bool BoundingBox::contains(const Vertex &vertex) const {
    auto [x, y, z] = vertex.coordinates();

    return x >= this->_corners[!RIGHT].x && x <= this->_corners[RIGHT].x &&
           y >= this->_corners[!TOP].y && y <= this->_corners[TOP].y &&
           z >= this->_corners[!FRONT].z && z <= this->_corners[FRONT].z;
}

/**
 * @brief Return the center of the AABB.
 *
 * @return Vertex center of the bounding box
 */
Vertex BoundingBox::center() {
    if (!this->_center) {
        this->_center = new Vertex(
            (this->_corners[0].x + this->_corners[4].x) /
                2,  // (left + right)/2
            (this->_corners[0].y + this->_corners[2].y) /
                2,                                           // (bottom + top)/2
            (this->_corners[0].z + this->_corners[1].z) / 2  // (rear + front)/2
        );
    }

    return *(this->_center);
}

/**
 * @brief Construct a new BoundingBox object from the list of corners.
 *
 * @param corners AABB corners
 * @param level depth
 */
BoundingBox::BoundingBox(const std::array<Vertex, 8> &corners, int level) {
    this->_corners = corners;
    this->_level = level;
}

/**
 * @brief Construct a new BoundingBox object from the vector of vertices.
 *
 * @param vertices vertices contained by the bounding box
 */
BoundingBox::BoundingBox(const std::vector<Vertex> &vertices) {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;

    std::for_each(vertices.begin(), vertices.end(), [&](Vertex const &vertex) {
        x.push_back(vertex.x);
        y.push_back(vertex.y);
        z.push_back(vertex.z);
    });

    // X
    double left = *std::min_element(x.begin(), x.end());
    double right = *std::max_element(x.begin(), x.end());

    // Y
    double bottom = *std::min_element(y.begin(), y.end());
    double top = *std::max_element(y.begin(), y.end());

    // Z
    double rear = *std::min_element(z.begin(), z.end());
    double front = *std::max_element(z.begin(), z.end());

    this->_corners = {
        Vertex(left, bottom, rear),
        Vertex(left, bottom, front),
        Vertex(left, top, rear),
        Vertex(left, top, front),
        Vertex(right, bottom, rear),
        Vertex(right, bottom, front),
        Vertex(right, top, rear),
        Vertex(right, top, front),
    };
}

__attribute__((annotate("oclint:suppress[bitwise operator in conditional]")))
std::array<BoundingBox *, 8>
BoundingBox::children() {
    if (std::any_of(
            this->_children.begin(),
            this->_children.end(),
            [](BoundingBox *box) {
                return box != nullptr;
            }
        ) ||
        this->_level >= DEPTH_LIMIT)
        return this->_children;

    Boundaries parent_boundaries(
        this->_corners[0].x,
        this->_corners[RIGHT].x,
        this->_corners[0].y,
        this->_corners[TOP].y,
        this->_corners[0].z,
        this->_corners[FRONT].z
    );

    for (int i = 0; i < 8; i++) {
        auto [_left, _right, _bottom, _top, _rear, _front] =
            this->_child_boundaries(i, parent_boundaries);

        const std::array<Vertex, 8> child_corners = {
            Vertex(_left, _bottom, _rear),
            Vertex(_left, _bottom, _front),
            Vertex(_left, _top, _rear),
            Vertex(_left, _top, _front),
            Vertex(_right, _bottom, _rear),
            Vertex(_right, _bottom, _front),
            Vertex(_right, _top, _rear),
            Vertex(_right, _top, _front),
        };

        this->_children[i] = new BoundingBox(child_corners, this->_level + 1);
    }

    return this->_children;
}

const Boundaries __attribute__((
    annotate("oclint:suppress[bitwise operator in conditional]")
))
BoundingBox::_child_boundaries(int index, const Boundaries &parent_boundaries) {
    auto [left, right, bottom, top, rear, front] = parent_boundaries;
    auto __center = this->center();

    Boundaries child_boundaries;
    auto &[_left, _right, _bottom, _top, _rear, _front] = child_boundaries;
    if ((index & RIGHT) == RIGHT) {
        _left = __center.x;
        _right = right;
    } else {
        _left = left;
        _right = __center.x;
    }

    if ((index & TOP) == TOP) {
        _top = top;
        _bottom = __center.y;
    } else {
        _top = __center.y;
        _bottom = bottom;
    }

    if ((index & FRONT) == FRONT) {
        _front = front;
        _rear = __center.z;
    } else {
        _front = __center.z;
        _rear = rear;
    }

    return child_boundaries;
}

const std::array<Vertex, 8> &BoundingBox::corners() const {
    return this->_corners;
}

std::vector<BoundingBox *>
BoundingBox::split(int level, const std::vector<Triangle> &triangles) {
    std::deque<BoundingBox *> queue;
    auto this_children = this->children();
    std::for_each(
        this_children.begin(),
        this_children.end(),
        [&queue, &triangles](BoundingBox *child) {
            queue.push_back(child);
            std::copy_if(
                triangles.begin(),
                triangles.end(),
                std::back_inserter(child->members()),
                [&child](const Triangle &triangle) {
                    return child->contains(triangle);
                }
            );
        }
    );

    while (queue.front()->level() != level) {
        std::array<BoundingBox *, 8> box_children = queue.front()->children();
        std::for_each(
            box_children.begin(),
            box_children.end(),
            [&queue](BoundingBox *child) {
                queue.push_back(child);
                std::copy_if(
                    queue.front()->members().begin(),
                    queue.front()->members().end(),
                    std::back_inserter(child->members()),
                    [&child](const Triangle &triangle) {
                        return child->contains(triangle);
                    }
                );
            }
        );
        queue.pop_front();
    }

    std::vector<BoundingBox *> retval(std::begin(queue), std::end(queue));

    return retval;
}

/*
   XYZ
   RTF
   000 left bottom rear
   001 left bottom front
   010 left top rear
   011 left top front
   100 right bottom rear
   101 right bottom front
   110 right top rear
   111 right top front
*/
