// GPL v3 (c) 2020, Daniel Williams 

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

using std::cerr;
using std::endl;
using std::string;
using std::unique_ptr;

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "index/index2u.hpp"
#include "geometry/bounds.hpp"

#include "grid/fixed-grid.hpp"

using Eigen::Vector2d;

using chart::index::Index2u;
using chart::geometry::Bounds;
using chart::geometry::Polygon;

namespace chart::grid {

template<typename cell_t, size_t dim>
FixedGrid<cell_t,dim>::FixedGrid()
    : bounds_(Bounds::make_square({0,0},dim))
    , precision_(1.0)
{}

template<typename cell_t, size_t dim>
FixedGrid<cell_t,dim>::FixedGrid(const Bounds& _bounds)
    : bounds_(Bounds::make_square(_bounds.center(), dim))
    , precision_(width() / dim)
{}

template<typename cell_t, size_t dim>
const Bounds& FixedGrid<cell_t,dim>::bounds() const {
    return bounds_;
}

template<typename cell_t, size_t dim>
bool FixedGrid<cell_t,dim>::contains(const Vector2d& p) const {
    return bounds_.contains(p);
}

template<typename cell_t, size_t dim>
cell_t FixedGrid<cell_t,dim>::classify(const Vector2d& p) const {
    return classify(p, 0xFF);
}

template<typename cell_t, size_t dim>
cell_t FixedGrid<cell_t,dim>::classify(const Vector2d& p, const cell_t default_value) const {
    if(bounds_.contains(p)){
        size_t i = index.lookup(discretize(p));
        return grid[i];
    }

    return default_value;
}

template<typename cell_t, size_t dim>
Index2u FixedGrid<cell_t,dim>::discretize(const Eigen::Vector2d& pa) const {
    auto pr = bounds_.translate(pa);
    return Index2u(static_cast<uint32_t>(pr.x() / precision_), static_cast<uint32_t>(pr.y() / precision_));
}

template<typename cell_t, size_t dim>
bool FixedGrid<cell_t,dim>::fill_uniform(const cell_t value){
    memset(grid.data(), value, size());
    return true;
}

template <typename cell_t, size_t dim>
bool FixedGrid<cell_t, dim>::fill_from_bounds(const geometry::Bounds& area, const cell_t value) {
    return false;
}

template <typename cell_t, size_t dim>
bool FixedGrid<cell_t, dim>::fill_from_buffer(const std::vector<cell_t>& source) {
    if (source.size() != grid.size()) {
        return false;
    }

    memcpy(grid.data(), source.data(), sizeof(cell_t) * source.size());
    return true;
}

template<typename cell_t, size_t dim>
cell_t& FixedGrid<cell_t,dim>::get_cell(const size_t xi, const size_t yi) {
    Index2u location(xi,yi);
    return grid[index.lookup(location)];
}

template<typename cell_t, size_t dim>
cell_t FixedGrid<cell_t,dim>::get_cell(const size_t xi, const size_t yi) const {
    Index2u location(xi,yi);
    return grid[index.lookup(location)];
}

template<typename cell_t, size_t dim>
double FixedGrid<cell_t,dim>::precision() const {
    return precision_;
}

template<typename cell_t, size_t dim>
size_t FixedGrid<cell_t,dim>::size() const {
    return grid.size();
}

template<typename cell_t, size_t dim>
void FixedGrid<cell_t,dim>::set_bounds(const Bounds& _bounds) {
    bounds_ = Bounds::make_square(_bounds);
    
    precision_ = static_cast<double>(width()) / static_cast<double>(dim);
}

template<typename cell_t, size_t dim>
bool FixedGrid<cell_t,dim>::store(const Vector2d& p, const cell_t new_value) {
    if(contains(p)){
        size_t i = index.lookup(discretize(p));
        grid[i] = new_value;
        return true;
    }

    return false;
}

template <typename cell_t, size_t dim>
std::string FixedGrid<cell_t, dim>::to_string() const {
    std::ostringstream buf;
    static const std::string header("======== ======= ======= ======= ======= ======= ======= =======\n");
    buf << header;
    for (size_t j = dim - 1; j < dim; --j) {
        for (size_t i = 0; i < dim; ++i) {
            const auto value = grid[index.lookup(i, j)];
            buf << ' ';
            if (value < 32) {
                buf << ' ';
            } else {
                buf << static_cast<char>(value);
            }
        }
        buf << endl;
    }
    buf << header;

    return buf.str();
}

template<typename cell_t, size_t dim>
size_t FixedGrid<cell_t,dim>::width() const {
    return bounds_.size().x();
}

template class FixedGrid<uint8_t, 8>;
template class FixedGrid<uint8_t, 32>;

};  // namespace chart::grid
