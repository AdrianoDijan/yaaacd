#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_3.h>

#include "../include/common.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel CGALKernel;

using namespace YAAACD;

/**
 * Parameterized constructor for objects of class Vertex
 *
 * @param x Value of type double, x coordinate
 * @param y Value of type double, y coordinate
 * @param y Value of type double, z coordinate
 */
Vertex::Vertex(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

/**
 * Get vertex coordinates.
 *
 * @return tuple
 */
const std::tuple<double, double, double> Vertex::coordinates() const {
    return {this->x, this->y, this->z};
}

Vertex::Vertex(std::array<double, 3> coordinates) {
    this->x = coordinates[0];
    this->y = coordinates[1];
    this->y = coordinates[2];
}
