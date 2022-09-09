#include "../include/hashmap.hpp"

#include <CGAL/intersections.h>

#include <vector>

#include "../include/common.hpp"

using namespace YAAACD;

SpatialHashMap::SpatialHashMap(
    const std::vector<Triangle>& triangles,
    int levels
) {
    this->_levels = levels;
    std::vector<Vertex> vertices;

    for (const Triangle& triangle : triangles)
        std::for_each(
            triangle.begin(),
            triangle.end(),
            [&vertices](const Vertex& vertex) {
                vertices.push_back(vertex);
            }
        );

    BoundingBox root(vertices);
    std::vector<BoundingBox*> children = root.split(levels, triangles);

    std::for_each(children.begin(), children.end(), [&](BoundingBox* box) {
        this->insert(box);
    });
}

int SpatialHashMap::_hash(BoundingBox* box) {
    Vertex center = box->center();

    return (static_cast<int>(center.x / CELL_SIZE * P1) ^
            static_cast<int>(center.y / CELL_SIZE * P2) ^
            static_cast<int>(center.z / CELL_SIZE * P3)) %
           TABLE_SIZE;
}

void SpatialHashMap::insert(BoundingBox* box) {
    int hash = this->_hash(box);

    if (!box->members().size()) return;

    std::map<int, std::vector<Triangle>>::iterator cell = this->_map.find(hash);
    if (cell != this->_map.end()) {
        std::copy(
            box->members().begin(),
            box->members().end(),
            std::back_inserter(cell->second)
        );
        return;
    }

    this->_map.insert_or_assign(hash, box->members());
}

bool SpatialHashMap::collides(const std::vector<Triangle>& triangles) {
    std::vector<Vertex> vertices;

    for (const Triangle& triangle : triangles)
        std::for_each(
            triangle.begin(),
            triangle.end(),
            [&vertices](const Vertex& vertex) {
                vertices.push_back(vertex);
            }
        );

    BoundingBox root(vertices);
    std::vector<BoundingBox*> children = root.split(this->_levels, triangles);

    return std::any_of(children.begin(), children.end(), [&](BoundingBox* box) {
        int hash = this->_hash(box);

        auto cell = this->_map.find(hash);
        if (cell != this->_map.end()) {
            return helpers::primes_intersect({cell->second, box->members()});
        }

        return false;
    });
}
