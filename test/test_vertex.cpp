#include <tuple>

#include "../include/octree.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace YAAACD;

TEST_CASE("Test vertex coordinates tuple", "[vertex]") {
    REQUIRE(Vertex(1, 1, 1).coordinates() == std::tuple{1, 1, 1});
    REQUIRE_FALSE(Vertex(1, 2, 1).coordinates() == std::tuple{1, 1, 1});
}

TEST_CASE("Text vertex comparison", "[vertex]") {
    Vertex v1(1, 2, 3), v2(1, 2, 3), v3(9, 9, 9);

    REQUIRE(v1 == v2);
    REQUIRE_FALSE(v1 == v3);
}
