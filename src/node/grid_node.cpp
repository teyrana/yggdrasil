// GPL v3 (c) 2020

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <Eigen/Geometry>
using Eigen::Vector2d;

#include <nlohmann/json.hpp>
using nlohmann::json;

#include "node/grid_node.hpp"
#include "node/tile_node.hpp"

using std::cerr;
using std::endl;
using std::ostream;
using std::string;
using std::unique_ptr;


namespace yggdrasil::node {

template<size_t n, typename child_t, typename cell_t>
GridNode<n,child_t,cell_t>::GridNode() : GridNode(0) {}

template<size_t n, typename child_t, typename cell_t>
GridNode<n,child_t,cell_t>::GridNode(const cell_t _value)
    : value(_value)
{
    for( auto& child : children ){
        child.reset();
    }
}

// template<size_t n, typename child_t>
// void GridNode<n,child_t>::draw(std::ostream& sink, const string& prefix, const string& as, const bool show_pointers) const {

//     sink << prefix << "[" << as << "]: ";
//     if (is_leaf()) {
//         sink << static_cast<int>(get_value());
//     }
//     // sink << ' ' << bounds.str();
//     if (show_pointers) {
//         sink << "   @:" << this << endl;
//     }
//     sink << endl;

//     if (!is_leaf()) {
//         auto next_prefix = prefix + "    ";
//         northeast->draw(sink, next_prefix, "NE", show_pointers);
//         northwest->draw(sink, next_prefix, "NW", show_pointers);
//         southwest->draw(sink, next_prefix, "SW", show_pointers);
//         southeast->draw(sink, next_prefix, "SE", show_pointers);
//     }
// }

template<size_t n, typename child_t, typename cell_t>
void GridNode<n,child_t,cell_t>::fill(const cell_t fill_value) {
    if (is_leaf()) {
        set_value(fill_value);
    } else {
        for( auto& child : children ){
            child->fill(fill_value);
        }
    }
}

// size_t GridNode<n,child_t,cell_t>::get_height() const {
//     if (is_leaf()) {
//         return 1;
//     } else {
//         const size_t ne_height = northeast->get_height();
//         const size_t nw_height = northwest->get_height();
//         const size_t se_height = southeast->get_height();
//         const size_t sw_height = southwest->get_height();

//         const size_t max_height = std::max(
//             ne_height, std::max(nw_height, std::max(se_height, sw_height)));
//         return max_height + 1;
//     }
// }

template<size_t n, typename child_t, typename cell_t>
bool GridNode<n,child_t,cell_t>::is_leaf() const { 
    return !bool(children[0]);
}

template<size_t n, typename child_t, typename cell_t>
cell_t GridNode<n,child_t,cell_t>::get_value() const { 
    return this->value;
}

template<size_t n, typename child_t, typename cell_t>
bool GridNode<n,child_t,cell_t>::operator==(const GridNode& other) const {
    return static_cast<const void*>(this) == static_cast<const void*>(&other);
}

template<size_t n, typename child_t, typename cell_t>
void GridNode<n,child_t,cell_t>::prune() {
    if (is_leaf()) {
        return;
    }

    cell_t test_value = children[0]->get_value();

    for( auto& child : children){
        // child->prune();
        // if(!child->is_leaf()){
        //     return;
        // }

        if(test_value != child->get_value()){
            return;
        }
    }

    reset();
    set_value(test_value);
}

template<size_t n, typename child_t, typename cell_t>
void GridNode<n,child_t,cell_t>::set_value(cell_t new_value) { this->value = new_value; }

template<size_t n, typename child_t, typename cell_t>
void GridNode<n,child_t,cell_t>::reset() {
    for( auto& child : children){
        child.release();
    }
}

template<size_t n, typename child_t, typename cell_t>
void GridNode<n,child_t,cell_t>::split() {
    if (is_leaf()) {
        value = NAN;
        // for( auto& child : children){
        //     // child.reset(new child_t(value));
        // }
    }
}

template<size_t n, typename child_t, typename cell_t>
void GridNode<n,child_t,cell_t>::split(const double precision, const double width) {
    if (precision >= width) {
        return;
    }

    if (is_leaf()) {
        split();
    }

    // const double half_width = width / 2;
    // for( auto& child : children){

    //     this->northeast->split(precision, half_width);
    // }
}

template<size_t n, typename child_t, typename cell_t>
std::string GridNode<n,child_t,cell_t>::to_string() const {
    std::ostringstream buf;
    // buf << this->to_json();
    return buf.str();
}

template<size_t n, typename child_t, typename cell_t>
GridNode<n,child_t,cell_t>::~GridNode() {
    for( unique_ptr<child_t>& child : children ){
        child.release();
    }
}

template class GridNode<4, TileNode<uint8_t, 4>, uint8_t>;

};  // namespace yggdrasil::node
