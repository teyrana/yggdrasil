// GPL v3 (c) 2020, Daniel Williams 

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

#include <Eigen/Geometry>

#include "path.hpp"

using std::cerr;
using std::endl;
using std::max;
using std::min;
using std::ostream;
using std::string;

using Eigen::Vector2d;

using chart::geometry::Path;


//---------------------------------------------------------------
Path::Path()
    : length_(0)
{}

Path::Path(std::vector<Vector2d>& init) 
    : points(init)
    , length_(calculate_length())
{}

Path::Path(std::initializer_list<Vector2d> init_list) 
    : points(init_list)
    , length_(calculate_length())
{}

auto Path::begin() -> decltype(points.begin()) {
    return points.begin();
}

double Path::calculate_length() {
    length_ = 0;

    if(points.size() < 2){
        return length_;
    }

    for( size_t i = 0; i < points.size()-1; ++i){
        length_ += segment(i).norm();
    }

    return length_;
}

void Path::clear() { 
    points.clear();
}

void Path::emplace_back(const double x, const double y) {
    push_back({x,y});
}

bool Path::empty() const {
    return points.empty();
}

auto Path::end() -> decltype(points.end()) {
    return points.end();
}

Vector2d Path::segment(const size_t index) const {
    return points[index+1] - points[index];
}

double Path::length() const {
    return length_;
}

bool Path::load(std::vector<Vector2d> source) {
    // if the new polygon contains insufficient points, abort and clear.
    if (2 > source.size()) {
        return false;
    }

    points = std::move(source);

    return true;
}

Vector2d& Path::operator[](const size_t index) { 
    return points[index];
}

const Vector2d& Path::operator[](const size_t index) const {
    return points[index];
}

void Path::push_back(const Vector2d p) {
    if(0 < points.size()){
        length_ += (p - points.back()).norm();
    }else{
        length_ = 0;
    }
    
    points.push_back(p);
}

void Path::resize(size_t capacity) {
    points.resize(capacity);
}

size_t Path::size() const {
    return points.size();
}

string Path::to_yaml(const string& indent) const {
    std::ostringstream buf;

    buf << indent << "points: \n";
    for ( auto& p : points ) {
        buf << indent << "    - " << p[0] << ", " << p[1] << '\n';
    }
    return buf.str();
}
