#pragma once

#include <array>
#include <map>
#include <vector>

#include "./common.hpp"
#include "./octree.hpp"

constexpr int TABLE_SIZE = 100;
constexpr int CELL_SIZE = 100;
constexpr int P1 = 131;
constexpr int P2 = 97;
constexpr int P3 = 137;

namespace YAAACD {

class SpatialHashMap {
 private:
    int _levels;
    std::map<int, std::vector<Triangle>> _map;
    int _hash(BoundingBox* box);
    void insert(BoundingBox* box);

 public:
    explicit SpatialHashMap(const std::vector<Triangle>& triangles, int levels);
    bool collides(const std::vector<Triangle>& triangles);
};

}  // namespace YAAACD
