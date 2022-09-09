#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_3.h>

#include "../include/common.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel CGALKernel;

using namespace YAAACD;

/**
 * @brief Convert Vertex to CGAL Point_3
 *
 * @param vertex vertex to convert
 * @return const CGALKernel::Point_3
 */
const CGALKernel::Point_3 YAAACD::converters::to_Point_3(const Vertex& vertex) {
    return CGALKernel::Point_3(vertex.x, vertex.y, vertex.z);
}

/**
 * @brief Convert Triangle to CGAL Triangle_3
 *
 * @param triangle triangle to convert
 * @return const CGALKernel::Triangle_3
 */
const CGALKernel::Triangle_3 YAAACD::converters::to_Triangle_3(
    const YAAACD::Triangle& triangle
) {
    return CGALKernel::Triangle_3(
        to_Point_3(triangle[0]),
        to_Point_3(triangle[1]),
        to_Point_3(triangle[2])
    );
}

bool YAAACD::helpers::primes_intersect(
    const std::array<const std::vector<Triangle>, 2>& sets
) {
    for (const auto& triangle1 : sets[0])
        for (const auto& triangle2 : sets[1])
            if (CGAL::intersection(
                    converters::to_Triangle_3(triangle1),
                    converters::to_Triangle_3(triangle2)
                ))
                return true;

    return false;
}

bool YAAACD::bruteforce_collides(
    const std::vector<Triangle>& triangles1,
    const std::vector<Triangle>& triangles2
) {
    for (auto triangle1 : triangles1)
        if (std::any_of(
                triangles2.begin(),
                triangles2.end(),
                [&triangle1](const Triangle& triangle) {
                    return CGAL::intersection(
                        converters::to_Triangle_3(triangle1),
                        converters::to_Triangle_3(triangle)
                    );
                }
            ))
            return true;

    return false;
}
