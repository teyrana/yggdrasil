// GPL v3 (c) 2020

#include <cfloat>

#include <Eigen/Geometry>
using Eigen::Vector2d;

#include "bounds.hpp"
using chart::geometry::Bounds;

Bounds::Bounds() { reset(); }

Bounds::Bounds(const Vector2d& SW, const Vector2d& NE)
    : southwest(std::min(SW.x(), NE.x()), std::min(SW.y(), NE.y())),
      northeast(std::max(SW.x(), NE.x()), std::max(SW.y(), NE.y())) {}

Vector2d Bounds::anchor() const { return southwest; }
double Bounds::constrain_x(const double value) const {
    return std::max(southwest.x(), std::min(value, northeast.x()));
}

double Bounds::constrain_y(const double value) const {
    return std::max(southwest.y(), std::min(value, northeast.y()));
}

Bounds Bounds::make_from_corners(const Vector2d& SW, const Vector2d& NE){
    return {SW, NE};
}

Bounds Bounds::make_from_center(const Vector2d& center, const Vector2d& dimensions){
    const Vector2d& SW = center - dimensions/2;
    const Vector2d& NE = center + dimensions/2;
    return {SW,NE};
}

Vector2d Bounds::max() const { return northeast; }

Vector2d Bounds::min() const { return southwest; }

Vector2d Bounds::size() const {
    return {northeast.x() - southwest.x(), northeast.y() - southwest.y()};
}

void Bounds::grow(const Vector2d& p) {
    southwest[0] = std::min(southwest[0], p.x());
    northeast[0] = std::max(northeast[0], p.x());
    southwest[1] = std::min(southwest[1], p.y());
    northeast[1] = std::max(northeast[1], p.y());
}

void Bounds::reset() {
    southwest = {DBL_MAX, DBL_MAX};
    northeast = {-DBL_MAX, -DBL_MAX};
}
