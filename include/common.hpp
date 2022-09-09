#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_3.h>

#include <array>
#include <memory>
#include <tuple>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel CGALKernel;
typedef std::tuple<double, double, double, double, double, double> Boundaries;

constexpr int DEPTH_LIMIT = 5;
constexpr int MIN_MEMBERS = 25;

namespace YAAACD {

class Vertex {
 public:
    double x;
    double y;
    double z;

    explicit Vertex(double x = 0, double y = 0, double z = 0);
    explicit Vertex(const std::array<double, 3> coordinates);

    friend bool operator==(const Vertex &lhs, const Vertex &rhs) {
        return lhs.coordinates() == rhs.coordinates();
    }

    const std::tuple<double, double, double> coordinates() const;
};

typedef std::array<Vertex, 3> Triangle;

class BoundingBox {
 private:
    std::array<Vertex, 8> _corners;
    std::array<BoundingBox *, 8> _children = {nullptr};
    Vertex *_center = nullptr;
    std::vector<Triangle> _members;  // only for sparial hashing
    int _level = 0;
    const Boundaries
    _child_boundaries(int index, const Boundaries &parent_boundaries);

 public:
    bool intersects(const BoundingBox &box) const;
    bool contains(const Triangle &triangle) const;
    const bool contains(const Vertex &vertex) const;
    std::array<BoundingBox *, 8> children();
    const std::array<Vertex, 8> &corners() const;
    std::vector<BoundingBox *>
    split(int level, const std::vector<Triangle> &triangles);
    std::vector<Triangle> &members() {
        return this->_members;
    }

    Vertex center();

    explicit BoundingBox(const std::vector<Vertex> &vertices);
    explicit BoundingBox(const std::array<Vertex, 8> &corners, int level);

    const int level() const {
        return this->_level;
    }
    BoundingBox() {}
};

bool bruteforce_collides(
    const std::vector<Triangle> &triangles1,
    const std::vector<Triangle> &triangles2
);

namespace converters {

const CGALKernel::Point_3 to_Point_3(const Vertex &vertex);
const CGALKernel::Triangle_3 to_Triangle_3(const Triangle &triangle);

};  // namespace converters

namespace helpers {
bool primes_intersect(const std::array<const std::vector<Triangle>, 2> &sets);
}

}  // namespace YAAACD
