#include <memory>
#include <vector>

#include "../include/common.hpp"
#include "../include/hashmap.hpp"
#include "../include/objfile.hpp"
#include "../include/octree.hpp"
#include "catch2/catch_test_macros.hpp"

constexpr int RIGHT = 0b100;
constexpr int TOP = 0b010;
constexpr int FRONT = 0b001;

using namespace YAAACD;

TEST_CASE("Test octree collision", "[octree]") {
    std::vector<Triangle> triangles;

    std::vector<Vertex> vertices1{
        Vertex(0, 0, 0),
        Vertex(0, 0, 1),
        Vertex(0, 1, 0),
        Vertex(0, 1, 1),
        Vertex(1, 0, 0),
        Vertex(1, 0, 1),
        Vertex(1, 1, 0),
        Vertex(1, 1, 1),
    };

    std::vector<Vertex> vertices2;
    std::transform(
        vertices1.begin(),
        vertices1.end(),
        std::back_inserter(vertices2),
        [](const Vertex& vertex) {
            return Vertex(vertex.x + 0.5, vertex.y + 0.5, vertex.z + 0.5);
        }
    );

    std::vector<Triangle> cube1{
        {vertices1[0], vertices1[6], vertices1[4]},
        {vertices1[0], vertices1[2], vertices1[6]},
        {vertices1[0], vertices1[3], vertices1[2]},
        {vertices1[0], vertices1[1], vertices1[3]},
        {vertices1[2], vertices1[7], vertices1[6]},
        {vertices1[2], vertices1[3], vertices1[7]},
        {vertices1[4], vertices1[6], vertices1[7]},
        {vertices1[4], vertices1[7], vertices1[5]},
        {vertices1[0], vertices1[4], vertices1[5]},
        {vertices1[0], vertices1[5], vertices1[1]},
        {vertices1[1], vertices1[5], vertices1[7]},
        {vertices1[1], vertices1[7], vertices1[3]},
    };

    std::vector<Triangle> cube2{
        {vertices2[0], vertices2[6], vertices2[4]},
        {vertices2[0], vertices2[2], vertices2[6]},
        {vertices2[0], vertices2[3], vertices2[2]},
        {vertices2[0], vertices2[1], vertices2[3]},
        {vertices2[2], vertices2[7], vertices2[6]},
        {vertices2[2], vertices2[3], vertices2[7]},
        {vertices2[4], vertices2[6], vertices2[7]},
        {vertices2[4], vertices2[7], vertices2[5]},
        {vertices2[0], vertices2[4], vertices2[5]},
        {vertices2[0], vertices2[5], vertices2[1]},
        {vertices2[1], vertices2[5], vertices2[7]},
        {vertices2[1], vertices2[7], vertices2[3]},
    };

    Octree tree1(cube1);
    Octree tree2(cube2);

    bool collides = tree1.collides(&tree2);

    REQUIRE(collides);
}

TEST_CASE("Test octree not colliding", "[octree]") {
    std::vector<Triangle> triangles;

    std::vector<Vertex> vertices1{
        Vertex(0, 0, 0),
        Vertex(0, 0, 1),
        Vertex(0, 1, 0),
        Vertex(0, 1, 1),
        Vertex(1, 0, 0),
        Vertex(1, 0, 1),
        Vertex(1, 1, 0),
        Vertex(1, 1, 1),
    };

    std::vector<Vertex> vertices2;
    std::transform(
        vertices1.begin(),
        vertices1.end(),
        std::back_inserter(vertices2),
        [](const Vertex& vertex) {
            return Vertex(vertex.x + 5, vertex.y + 5, vertex.z + 5);
        }
    );

    std::vector<Triangle> cube1{
        {vertices1[0], vertices1[6], vertices1[4]},
        {vertices1[0], vertices1[2], vertices1[6]},
        {vertices1[0], vertices1[3], vertices1[2]},
        {vertices1[0], vertices1[1], vertices1[3]},
        {vertices1[2], vertices1[7], vertices1[6]},
        {vertices1[2], vertices1[3], vertices1[7]},
        {vertices1[4], vertices1[6], vertices1[7]},
        {vertices1[4], vertices1[7], vertices1[5]},
        {vertices1[0], vertices1[4], vertices1[5]},
        {vertices1[0], vertices1[5], vertices1[1]},
        {vertices1[1], vertices1[5], vertices1[7]},
        {vertices1[1], vertices1[7], vertices1[3]},
    };

    std::vector<Triangle> cube2{
        {vertices2[0], vertices2[6], vertices2[4]},
        {vertices2[0], vertices2[2], vertices2[6]},
        {vertices2[0], vertices2[3], vertices2[2]},
        {vertices2[0], vertices2[1], vertices2[3]},
        {vertices2[2], vertices2[7], vertices2[6]},
        {vertices2[2], vertices2[3], vertices2[7]},
        {vertices2[4], vertices2[6], vertices2[7]},
        {vertices2[4], vertices2[7], vertices2[5]},
        {vertices2[0], vertices2[4], vertices2[5]},
        {vertices2[0], vertices2[5], vertices2[1]},
        {vertices2[1], vertices2[5], vertices2[7]},
        {vertices2[1], vertices2[7], vertices2[3]},
    };

    Octree tree1(cube1);
    Octree tree2(cube2);

    bool collides = tree1.collides(&tree2);

    REQUIRE_FALSE(collides);
}
