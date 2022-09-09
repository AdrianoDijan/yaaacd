#pragma once

#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "./common.hpp"

namespace YAAACD {

class Octree {
 private:
    BoundingBox _bounds;
    std::array<Octree*, 8> _children = {nullptr};
    std::vector<Triangle> _members;
    Octree* _root = nullptr;
    int _level = 0;

 public:
    explicit Octree(
        const std::vector<Triangle>& triangles,
        Octree* root = nullptr,
        int level = 0
    );
    std::array<Octree*, 8> children();
    const BoundingBox& bounds() const;
    int level() const;
    bool collides(Octree* octree);
    bool has_children();

    // helper functions
    static int children_position(Octree* child1, Octree* child2);
};

}  // namespace YAAACD
