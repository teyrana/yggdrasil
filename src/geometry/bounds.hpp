// GPL v3 (c) 2020

#ifndef _BOUNDING_BOX_HPP_
#define _BOUNDING_BOX_HPP_

#include <Eigen/Geometry>
using Eigen::Vector2d;

namespace chart::geometry {

class Bounds {
    Vector2d southwest;
    Vector2d northeast;

  public:
    Bounds();
    Bounds(const Vector2d& SW, const Vector2d& NE);

    Vector2d anchor() const;

    double constrain_x(const double value) const;

    double constrain_y(const double value) const;

    void grow(const Vector2d& p);

    static Bounds make_from_corners(const Vector2d& SW, const Vector2d& NE);
    static Bounds make_from_center(const Vector2d& center, const Vector2d& dimensions);

    Vector2d max() const;

    Vector2d min() const;

    void reset();

    Vector2d size() const;

private:
    friend class Bounds_ConstructConfiguration_Test;
    friend class Bounds_ConstructFromCorners_Test;
    friend class Bounds_MakeFromCorners_Test;
    friend class Bounds_MakeFromCenter_Test;

}; // class Bounds

};     // namespace chart::geometry
#endif // #ifndef _BOUNDING_BOX_HPP_
