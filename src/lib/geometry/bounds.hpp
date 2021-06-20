// GPL v3 (c) 2020, Daniel Williams 

#ifndef _BOUNDS_HPP_
#define _BOUNDS_HPP_

#include <Eigen/Geometry>
using Eigen::Vector2d;

#include <nlohmann/json.hpp>

namespace chart::geometry {

class Bounds {
public:
    Bounds();
    Bounds(const Bounds& other) = default; // copy constructor
    Bounds(const Vector2d& SW, const Vector2d& NE);

    Vector2d anchor() const;

    /// \brief translates a vector from the bounds-frame to global-frame
    Eigen::Vector2d as_global(const Eigen::Vector2d& p_local) const;

    /// \brief translates a vector from the global-frame to the bounds-frame
    Eigen::Vector2d as_local(const Eigen::Vector2d& p_global) const;

    Vector2d center() const;

    bool contains(const Vector2d& p) const;

    double constrain_x(const double value) const;

    double constrain_y(const double value) const;

    void grow(const Vector2d& p);

    bool is_square() const;

    bool is_valid() const;

    static Bounds make_from_corners(const Vector2d SW, const Vector2d NE);
    static Bounds make_from_center(const Vector2d& center, const Vector2d& dimensions);

    static Bounds make_from_json(const nlohmann::json& obj);

    static Bounds make_square(const Bounds& _bounds);
    static Bounds make_square(const Vector2d& center, const double width);

    Vector2d max() const;

    Vector2d min() const;

    Vector2d northeast() const;

    void reset();

    Vector2d southwest() const;

    Vector2d size() const;


protected:
    static Eigen::Vector2d get_point(const nlohmann::json& obj);

public:
    const static Bounds limit;
    const static Bounds nan;
    const static Bounds zero;
    
protected:
    Vector2d southwest_;
    Vector2d northeast_;

}; // class Bounds

}    // namespace chart::geometry

#endif // #ifndef _BOUNDS_HPP_