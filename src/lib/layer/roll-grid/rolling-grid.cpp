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

#include "grid/rolling-grid.hpp"

using Eigen::Vector2d;

using chart::index::Index2u;
using chart::geometry::Bounds;
using chart::geometry::Polygon;

namespace chart::grid {

template<typename cell_t, size_t dim>
Index2u RollingGrid<cell_t,dim>::as_index(const Eigen::Vector2d& location) const {
    auto local = bounds_.as_local(location);
    return Index2u( static_cast<uint32_t>(local.x() / precision_)
                  , static_cast<uint32_t>(local.y() / precision_) );
}

template<typename cell_t, size_t dim>
Vector2d  RollingGrid<cell_t,dim>::as_location(const index::Index2u& index) const {
    const Vector2d p_local = { (static_cast<float>(index.i) + 0.5) * precision_
                             , (static_cast<float>(index.j) + 0.5) * precision_ };
    return bounds_.as_global(p_local);
}

template<typename cell_t, size_t dim>
bool RollingGrid<cell_t,dim>::blocked(const index::Index2u& at) const {
    return (blocking_threshold <= operator[](at));
}

template<typename cell_t, size_t dim>
RollingGrid<cell_t,dim>::RollingGrid()
    : bounds_(Bounds::make_square({0,0},dim))
    , precision_(1.0)
{}

template<typename cell_t, size_t dim>
RollingGrid<cell_t,dim>::RollingGrid(const Bounds& _bounds)
    : bounds_(Bounds::make_square(_bounds.center(), dim))
    , precision_(width() / dim)
{}

template<typename cell_t, size_t dim>
const Bounds& RollingGrid<cell_t,dim>::bounds() const {
    return bounds_;
}

template<typename cell_t, size_t dim>
bool RollingGrid<cell_t,dim>::contains(const Vector2d& p) const {
    return bounds_.contains(p);
}

template<typename cell_t, size_t dim>
bool RollingGrid<cell_t,dim>::contains(const Index2u& index) const {
    if (index.i < dim && index.j < dim ){
        return true;
    }
    return false;        
}

template<typename cell_t, size_t dim>
cell_t RollingGrid<cell_t,dim>::classify(const Vector2d& p) const {
    return classify(p, 0xFF);
}

template<typename cell_t, size_t dim>
cell_t RollingGrid<cell_t,dim>::classify(const Vector2d& p, const cell_t default_value) const {
    if(bounds_.contains(p)){
        size_t i = index.lookup(as_index(p));
        return grid[i];
    }

    return default_value;
}

template<typename cell_t, size_t dim>
bool RollingGrid<cell_t,dim>::fill_uniform(const cell_t value){
    memset(grid.data(), value, size());
    return true;
}

template <typename cell_t, size_t dim>
bool RollingGrid<cell_t, dim>::fill_from_bounds(const geometry::Bounds& area, const cell_t value) {
    return false;
}

template <typename cell_t, size_t dim>
int RollingGrid<cell_t, dim>::fill_from_buffer(const std::vector<cell_t>& source) {
    if (source.size() != grid.size()) {
        return -1;
    }

    memcpy(grid.data(), source.data(), sizeof(cell_t) * source.size());
    return 0;
}

template<typename cell_t, size_t dim>
cell_t& RollingGrid<cell_t,dim>::get_cell(const size_t xi, const size_t yi) {
    Index2u location(xi,yi);
    return grid[index.lookup(location)];
}

template<typename cell_t, size_t dim>
cell_t RollingGrid<cell_t,dim>::get_cell(const size_t xi, const size_t yi) const {
    Index2u location(xi,yi);
    return grid[index.lookup(location)];
}

template<typename cell_t, size_t dim>
bool RollingGrid<cell_t,dim>::move(const Eigen::Vector2d& p) {
    return false;
}

template<typename cell_t, size_t dim>
cell_t& RollingGrid<cell_t,dim>::operator[](const index::Index2u& location) {
    return grid[index.lookup(location)];
}

template<typename cell_t, size_t dim>
cell_t RollingGrid<cell_t,dim>::operator[](const index::Index2u& location) const {
    return grid[index.lookup(location)];
}

template<typename cell_t, size_t dim>
double RollingGrid<cell_t,dim>::precision() const {
    return precision_;
}

template<typename cell_t, size_t dim>
size_t RollingGrid<cell_t,dim>::size() const {
    return grid.size();
}

template<typename cell_t, size_t dim>
void RollingGrid<cell_t,dim>::set_bounds(const Bounds& _bounds) {
    bounds_ = Bounds::make_square(_bounds);
    
    precision_ = static_cast<double>(width()) / static_cast<double>(dim);
}

template<typename cell_t, size_t dim>
bool RollingGrid<cell_t,dim>::store(const Vector2d& p, const cell_t new_value) {
    if(contains(p)){
        size_t i = index.lookup(as_index(p));
        grid[i] = new_value;
        return true;
    }

    return false;
}

template <typename cell_t, size_t dim>
std::string RollingGrid<cell_t, dim>::to_string() const {
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
size_t RollingGrid<cell_t,dim>::width() const {
    return bounds_.size().x();
}

template class RollingGrid<uint8_t, 8>;
template class RollingGrid<uint8_t, 32>;
template class RollingGrid<uint8_t, 64>;

}  // namespace chart::grid
