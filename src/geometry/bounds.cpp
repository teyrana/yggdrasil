// GPL v3 (c) 2020

#include <cfloat>

#include <Eigen/Geometry>
using Eigen::Vector2d;

#include "bounds.hpp"
using yggdrasil::geometry::Bounds;

Bounds::Bounds(){
    reset();
}

Bounds::Bounds( const Vector2d& SW, const Vector2d& NE ): 
    southwest(SW), northeast(NE)
{}

Vector2d Bounds::anchor() const {
    return southwest;
}

Vector2d Bounds::max() const { 
    return northeast;
}

Vector2d Bounds::min() const { 
    return southwest;
}

Vector2d Bounds::size() const { 
    return {northeast.x() - southwest.x(), northeast.y() - southwest.y()};
}

void Bounds::grow( const Vector2d& p){
    southwest[0] = std::min(southwest[0], p.x());
    northeast[0] = std::max(northeast[0], p.x());
    southwest[1] = std::min(southwest[1], p.y());
    northeast[1] = std::max(northeast[1], p.y());
}

void Bounds::reset(){ 
    southwest = {DBL_MAX, DBL_MAX};
    northeast = {-DBL_MAX, -DBL_MAX};
}

