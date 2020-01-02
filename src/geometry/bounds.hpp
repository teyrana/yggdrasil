// GPL v3 (c) 2020

#ifndef _BOUNDING_BOX_HPP_
#define _BOUNDING_BOX_HPP_

#include <Eigen/Geometry>
using Eigen::Vector2d;

namespace yggdrasil::geometry {

class Bounds {
    Vector2d southwest;
    Vector2d northeast;

public:
    Bounds();
    Bounds( const Vector2d& SW, const Vector2d& NE );

    // synonym
    Vector2d anchor() const;
    Vector2d max() const;
    Vector2d min() const;
    Vector2d size() const;

    void grow(const Vector2d& p);
    void reset();

}; // class Bounds

}; // namespace yggdrasil::geometry
#endif // #ifndef _BOUNDING_BOX_HPP_
