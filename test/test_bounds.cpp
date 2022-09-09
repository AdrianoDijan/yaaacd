#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../include/hashmap.hpp"
#include "../include/octree.hpp"
#include "catch2/catch_test_macros.hpp"

constexpr int RIGHT = 0b100;
constexpr int TOP = 0b010;
constexpr int FRONT = 0b001;

using namespace YAAACD;

TEST_CASE("Test bounding box center", "[bounding_box]") {
    std::vector<Vertex> vertices{
        Vertex(-10, 0, 0),
        Vertex(10, 0, 0),
        Vertex(0, -10, 0),
        Vertex(0, 10, 0),
        Vertex(0, 0, -10),
        Vertex(0, 0, 10),
    };
    BoundingBox box(vertices);

    REQUIRE(box.center() == Vertex(0, 0, 0));
}

TEST_CASE("Test bounding box corners", "[bounding_box]") {
    std::vector<Vertex> vertices{
        Vertex(-10, 0, 0),
        Vertex(10, 0, 0),
        Vertex(0, -10, 0),
        Vertex(0, 10, 0),
        Vertex(0, 0, -10),
        Vertex(0, 0, 10),
    };
    BoundingBox box(vertices);
    const std::array<Vertex, 8> corners = box.corners();

    for (int i = 0; i < 8; i++) {
        int i1 = ((i & RIGHT) == RIGHT) ? 10 : -10;
        int i2 = ((i & TOP) == TOP) ? 10 : -10;
        int i3 = ((i & FRONT) == FRONT) ? 10 : -10;

        REQUIRE(corners[i] == Vertex(i1, i2, i3));
    }
}

TEST_CASE("Test bounding box children", "[bounding_box]") {
    std::vector<Vertex> vertices{
        Vertex(-10, 0, 0),
        Vertex(10, 0, 0),
        Vertex(0, -10, 0),
        Vertex(0, 10, 0),
        Vertex(0, 0, -10),
        Vertex(0, 0, 10),
    };
    BoundingBox box(vertices);

    const std::array<BoundingBox*, 8>& children = box.children();

    for (int i = 0; i < 8; i++) {
        REQUIRE(children[i]->level() == 1);

        const std::array<Vertex, 8> corners = children[i]->corners();
        for (int j = 0; j < 8; j++) {
            int x = 0;
            int y = 0;
            int z = 0;

            if ((i & RIGHT) == RIGHT) {
                if ((j & RIGHT) == RIGHT) {
                    x = 10;
                } else {
                    x = 0;
                }
            } else {
                if ((j & RIGHT) == RIGHT) {
                    x = 0;
                } else {
                    x = -10;
                }
            }

            if ((i & TOP) == TOP) {
                if ((j & TOP) == TOP) {
                    y = 10;
                } else {
                    y = 0;
                }
            } else {
                if ((j & TOP) == TOP) {
                    y = 0;
                } else {
                    y = -10;
                }
            }

            if ((i & FRONT) == FRONT) {
                if ((j & FRONT) == FRONT) {
                    z = 10;
                } else {
                    z = 0;
                }
            } else {
                if ((j & FRONT) == FRONT) {
                    z = 0;
                } else {
                    z = -10;
                }
            }

            const auto [cx, cy, cz] = corners[j].coordinates();
            const auto [vx, vy, vz] = Vertex(x, y, z).coordinates();

            std::stringstream log_format;
            log_format << "Actual: [" << cx << ", " << cy << ", " << cz
                       << "] // Expected: [" << vx << ", " << vy << ", " << vz
                       << "]\n"
                       << "i: " << i << " j: " << j;
            INFO(log_format.str());
            REQUIRE(corners[j] == Vertex(x, y, z));
        }
    }
}

TEST_CASE("Test bounding box intersection", "[bounding_box]") {
    std::vector<Vertex> v1{
        Vertex(-10, 0, 0),
        Vertex(10, 0, 0),
        Vertex(0, -10, 0),
        Vertex(0, 10, 0),
        Vertex(0, 0, -10),
        Vertex(0, 0, 10),
    };
    BoundingBox b1(v1);

    std::vector<Vertex> v2;
    std::transform(
        v1.begin(),
        v1.end(),
        std::back_inserter(v2),
        [](const Vertex& vertex) {
            return Vertex(vertex.x + 1, vertex.y + 1, vertex.z + 1);
        }
    );

    BoundingBox b2(v2);

    REQUIRE(b1.intersects(b2));

    std::vector<Vertex> v3;
    std::transform(
        v1.begin(),
        v1.end(),
        std::back_inserter(v3),
        [](const Vertex& vertex) {
            return Vertex(vertex.x + 30, vertex.y + 30, vertex.z + 30);
        }
    );
    BoundingBox b3(v3);

    REQUIRE_FALSE(b1.intersects(b3));
}
