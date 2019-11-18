// GPL v3 (c) 2020, Daniel Williams 

#include <cfloat>
#include <iostream>
#include <string>

#include <Eigen/Geometry>
using Eigen::Vector2d;

#include "bounds.hpp"
using chart::geometry::Bounds;

// used for reading and write json documents:
// need one of:
//   - southwest + northeast
//   - center + width
//   - anchor + width
const static inline std::string anchor_key("anchor");
const static inline std::string center_key("center");
const static inline std::string northeast_key("northeast");
const static inline std::string southwest_key("southwest");
const static inline std::string width_key("width");


const Bounds Bounds::limit;
const Bounds Bounds::nan(Vector2d({NAN,NAN}),Vector2d({NAN,NAN}));
const Bounds Bounds::zero({0,0},{0,0});

Bounds::Bounds() { reset(); }

Bounds::Bounds(const Vector2d& SW, const Vector2d& NE)
    : southwest_(std::min(SW.x(), NE.x()), std::min(SW.y(), NE.y())),
      northeast_(std::max(SW.x(), NE.x()), std::max(SW.y(), NE.y())) {}

Vector2d Bounds::center() const {
    return Vector2d( (southwest_.x() + northeast_.x())/2, (southwest_.y() + northeast_.y())/2 );
}

bool Bounds::contains(const Vector2d& p) const {
    if( p.x() < northeast_.x() &&
        p.x() > southwest_.x() &&
        p.y() < northeast_.y() &&
        p.y() > southwest_.y()){
            return true;
    }
    return false;
}

Vector2d Bounds::anchor() const { return southwest_; }

double Bounds::constrain_x(const double value) const {
    return std::max(southwest_.x(), std::min(value, northeast_.x()));
}

double Bounds::constrain_y(const double value) const {
    return std::max(southwest_.y(), std::min(value, northeast_.y()));
}

Eigen::Vector2d Bounds::get_point(const nlohmann::json& obj){
    if(obj.is_array() && 2 == obj.size()){
        return {obj[0].get<double>(), obj[1].get<double>()};
    }
    return {0,0};
}

void Bounds::grow(const Vector2d& p) {
    southwest_[0] = std::min(southwest_[0], p.x());
    northeast_[0] = std::max(northeast_[0], p.x());
    southwest_[1] = std::min(southwest_[1], p.y());
    northeast_[1] = std::max(northeast_[1], p.y());
}

bool Bounds::is_square() const {
    const auto widths = size();
    return bool(widths.x() == widths.y());
}

bool Bounds::is_valid() const {
    return ! (isnan(southwest_.x()) || isnan(southwest_.y()) 
                || isnan(northeast_.x()) || isnan(northeast_.y()));
}

Bounds Bounds::make_from_corners(const Vector2d SW, const Vector2d NE){
    return {SW, NE};
}

Bounds Bounds::make_from_center(const Vector2d& center, const Vector2d& dimensions){
    const Vector2d& SW = center - dimensions/2;
    const Vector2d& NE = center + dimensions/2;
    return {SW,NE};
}

Bounds Bounds::make_from_json(const nlohmann::json& obj){
    // keys: "southwest", "northeast"
    if(obj.contains(southwest_key) && obj.contains(northeast_key)){
        const Vector2d southwest = Bounds::get_point(obj[southwest_key]);
        const Vector2d northeast = Bounds::get_point(obj[northeast_key]);
        return make_from_corners(southwest,northeast);

    // keys: "anchor", "width"
    } else if(obj.contains(anchor_key) && obj.contains(width_key) && obj[width_key].is_number()){
        const Vector2d anchor = Bounds::get_point(obj[anchor_key]);
        const double width = obj[width_key].get<double>();
        const Vector2d widths = {width, width};
        const Vector2d center = anchor + widths/2;
        return make_from_center(center, widths);

    // keys: "center", "width"
    } else if(obj.contains(center_key) && obj.contains(width_key) && obj[width_key].is_number()){
        const Vector2d center = Bounds::get_point(obj[center_key]);
        const double width = obj[width_key].get<double>();
        const Vector2d widths = {width, width};
        return make_from_center(center, widths);
    }

    return Bounds::nan;
}

Bounds Bounds::make_square(const Bounds& _bounds) {
    const Vector2d size = _bounds.size();
    return Bounds::make_square( _bounds.center(), std::max(size.x(), size.y()));
}

Bounds Bounds::make_square(const Vector2d& center, const double width){
    const Vector2d dimensions = {width, width};

    const Vector2d SW = center - dimensions/2;
    const Vector2d NE = center + dimensions/2;

    return {SW,NE};
}

Vector2d Bounds::max() const { return northeast_; }

Vector2d Bounds::min() const { return southwest_; }

Vector2d Bounds::northeast() const { return northeast_; }

Vector2d Bounds::size() const {
    return {northeast_.x() - southwest_.x(), northeast_.y() - southwest_.y()};
}

Vector2d Bounds::southwest() const { return southwest_; }

Eigen::Vector2d Bounds::translate(const Eigen::Vector2d& p0) const {
    return p0 - southwest_;
}

void Bounds::reset() {
    southwest_ = {DBL_MAX, DBL_MAX};
    northeast_ = {-DBL_MAX, -DBL_MAX};
}
