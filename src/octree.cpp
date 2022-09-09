#include "../include/octree.hpp"

#include <CGAL/intersections.h>

#include <array>
#include <iostream>
#include <memory>
#include <vector>

#include "../include/common.hpp"

using namespace YAAACD;

/* CHILDREN POSITIONS
 * When iterating through children to detect collision, it's neccessary to have
 * the information about whether nodes (and which ones) have any children. There
 * are 4 possible cases:
 *   1. CHILDREN_NONE   0b00  // Neither node has any children
 *   2. CHILDREN_1      0b01  // Only the first node has children
 *   3. CHILDREN_2      0b10  // Only the second node has children
 *   4. CHILDREN_BOTH   0b11  // Both nodes have children
 */

constexpr int CHILDREN_NONE = 0b00;
constexpr int CHILDREN_1 = 0b01;
constexpr int CHILDREN_2 = 0b10;
constexpr int CHILDREN_BOTH = 0b11;

Octree::Octree(
    const std::vector<Triangle>& triangles,
    Octree* root,
    int level
) {
    std::vector<Vertex> vertices;
    for (const Triangle& triangle : triangles)
        std::for_each(
            triangle.begin(),
            triangle.end(),
            [&vertices](const Vertex& vertex) {
                vertices.push_back(vertex);
            }
        );

    this->_bounds = BoundingBox(vertices);
    this->_members = triangles;
    this->_root = root ? root : this;
    this->_level = level;
}

std::array<Octree*, 8> Octree::children() {
    if (this->_level >= DEPTH_LIMIT ||
        std::any_of(
            this->_children.begin(),
            this->_children.end(),
            [](Octree* child) {
                return child != nullptr;
            }
        ) ||
        this->_members.size() < MIN_MEMBERS) {
        return this->_children;
    }

    std::array<BoundingBox*, 8> child_bounds = this->_bounds.children();
    for (int i = 0; i < 8; i++) {
        std::vector<Triangle> contained_objects;
        std::copy_if(
            this->_members.begin(),
            this->_members.end(),
            std::back_inserter(contained_objects),
            [&child_bounds, i](const Triangle& triangle) {
                for (const Vertex& vertex : triangle) {
                    if (child_bounds[i]->contains(vertex)) return true;
                }
                return false;
            }
        );
        if (contained_objects.size() < 0.9 * this->_members.size() &&
            contained_objects.size() >
                (0.025 / 100) * this->_root->_members.size())
            this->_children[i] = new Octree(
                contained_objects,
                this->_root ? this->_root : this,
                this->_level + 1
            );
        else
            this->_children[i] = nullptr;
    }

    return this->_children;
}

const BoundingBox& Octree::bounds() const {
    return this->_bounds;
}

bool Octree::has_children() {
    std::array<Octree*, 8> child_nodes = this->children();

    return std::any_of(
        child_nodes.begin(),
        child_nodes.end(),
        [](const Octree* child) {
            return child != nullptr;
        }
    );
}

int Octree::children_position(Octree* child1, Octree* child2) {
    int first = child1->has_children() ? CHILDREN_1 : 0;
    int second = child2->has_children() ? CHILDREN_2 : 0;

    return first | second;
}

bool Octree::collides(Octree* octree) {
    std::vector<Octree*> pairs = {this, octree};

    while (!pairs.empty()) {
        Octree* tree1 = pairs.back();
        pairs.pop_back();

        Octree* tree2 = pairs.back();
        pairs.pop_back();

        if (!tree1->bounds().intersects(tree2->bounds())) continue;

        switch (Octree::children_position(tree1, tree2)) {
            case CHILDREN_NONE:
                if (helpers::primes_intersect({tree1->_members, tree2->_members}
                    )) {
                    return true;
                }
                break;
            case CHILDREN_1:
                for (auto child : tree1->children())
                    if (child) {
                        pairs.push_back(child);
                        pairs.push_back(tree2);
                    }
                break;
            case CHILDREN_2:
                for (auto child : tree2->children())
                    if (child) {
                        pairs.push_back(tree1);
                        pairs.push_back(child);
                    }
                break;
            case CHILDREN_BOTH:
                for (auto child1 : tree1->children())
                    for (auto child2 : tree2->children())
                        if (child1 && child2) {
                            pairs.push_back(child1);
                            pairs.push_back(child2);
                        }
                break;
            default:
                break;
        }
    }

    return false;
}
