// GPL v3 (c) 2020

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "polygon.hpp"

using std::cerr;
using std::endl;
using std::max;
using std::min;
using std::ostream;
using std::string;

using Eigen::Vector2d;

using yggdrasil::geometry::Bounds;
using yggdrasil::geometry::Polygon;

//---------------------------------------------------------------
// Constructor
//
Polygon::Polygon() { set_default(); }

Polygon::Polygon(const size_t initial_capacity) : points(initial_capacity) {
    points.resize(initial_capacity);
}

Polygon::Polygon(nlohmann::json doc) { load(doc); }

Polygon::Polygon(std::vector<Vector2d>& init) { load(init); }

Polygon::Polygon(std::initializer_list<Vector2d> init_list) {
    std::vector<Vector2d> pts = init_list;
    load(pts);
}

void Polygon::clear() { points.clear(); }

void Polygon::complete() {
    // NOTE: Yes, this function performs multiple passes of the point-vector...
    //       where it could calculate all desired info in one pass.
    //       This is done for purely for clarity.
    //       This function is not expected to be hot-path code.

    // cerr << "====== ====== ====== " << endl;
    // write_yaml(cerr, "    ");

    // we must enclose before the clockwise-check.
    enclose_polygon();

    recalculate_bounding_box();

    if (!is_right_handed()) {
        std::reverse(std::begin(points), std::end(points));
    }
}

void Polygon::emplace(const double x, const double y) {
    points.emplace_back(x, y);
}

void Polygon::enclose_polygon() {
    // ensure that polygon loops back
    const auto& first_Vector2d = points[0];
    const auto& last_Vector2d = points[points.size() - 1];

    if (!first_Vector2d.isApprox(last_Vector2d)) {
        points.emplace_back(first_Vector2d);
    }
}

Polygon Polygon::make_diamond(const double width) {
    return {{width, 0}, {0, width}, {-width, 0}, {0, -width}};
}

bool Polygon::is_right_handed() const {
    double sum = 0;

    // NOTE THE RANGE!:
    //   this needs to iterate over all point-pairs!
    for (uint i = 0; i < (points.size() - 1); ++i) {
        auto& p1 = points[i];
        auto& p2 = points[i + 1];
        sum += (p1[0] * p2[1]) - (p1[1] * p2[0]);
    }

    // // The shoelace formula includes a divide-by-2 step; ... but we don't
    // // care about the magnitude, just the sign.  So skip this step.
    // sum /= 2;

    if (0 > sum) {
        // left-handed / clockwise points
        return false;
    } else {
        // right-handed / counter-clockwise points
        return true;
    }
}

bool Polygon::load(std::vector<Vector2d> source) {
    // if the new polygon contains insufficient points, abort and clear.
    if (4 > source.size()) {
        return false;
    }

    points = std::move(source);

    complete();

    return true;
}

bool Polygon::load(nlohmann::json doc) {
    if (doc.is_array() && doc[0].is_array() && (4 <= doc.size())) {
        clear();
        points.resize(doc.size());

        size_t pair_index = 0;
        for (auto& pair : doc) {
            Vector2d p(pair[0].get<double>(), pair[1].get<double>());
            points[pair_index] = p;
            ++pair_index;
        }

        complete();

        return true;
    }

    return false;
}

const Bounds& Polygon::get_bounds() const { return bounds; }

void Polygon::move(const Vector2d& delta) {
    std::for_each(points.begin(), points.end(),
                  [&](Vector2d& p) { p += delta; });
}

Vector2d& Polygon::operator[](const size_t index) { return points[index]; }

const Vector2d& Polygon::operator[](const size_t index) const {
    return points[index];
}

void Polygon::push_back(const Vector2d p) { points.push_back(p); }

void Polygon::recalculate_bounding_box() {
    bounds.reset();
    for (auto& p : points) {
        bounds.grow(p);
    }
}

void Polygon::resize(size_t capacity) { points.resize(capacity); }

size_t Polygon::size() const { return points.size(); }

void Polygon::set_default() {
    points.clear();
    points.emplace_back(0, 0);
    points.emplace_back(1, 0);
    points.emplace_back(1, 1);
    points.emplace_back(0, 1);
    bounds = {{0, 0}, {1, 1}};
}

void Polygon::write_yaml(std::ostream& sink, string indent) const {
    sink << indent << "points: \n";
    for (uint i = 0; i < points.size(); ++i) {
        auto& p = points[i];
        sink << indent << "    - " << p[0] << ", " << p[1] << '\n';
    }
}
