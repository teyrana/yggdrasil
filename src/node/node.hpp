// GPL v3 (c) 2020, Daniel Williams 

#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <cstdio>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "geometry/cell_value.hpp"
#include "geometry/layout.hpp"
#include "geometry/polygon.hpp"

namespace chart::node {

/// \brief defines a template interface for all nodes in the world tree.
/// 
/// note: This class employs the [CRTP]( https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern )
template <typename T> class Node {
  public:

    // default constructor:
    Node()
        { static_cast<T*>(this)(); }

    // copy constructor
    Node(const Node& t) = delete;

    // move constructor
    Node(Node&& t) = delete;

    // copy assignment operator
    Node& operator=(const Node& t) = delete;

    // move assignment operator
    Node& operator=(Node&& t) = delete;

    // ~Node();

    geometry::cell_value_t classify(const Eigen::Vector2d& p) const
        { return static_cast<T*>(this)->classify(p); }

    ///! \brief writes debug information to std err
    void to_debug() const
        { static_cast<T*>(this)->to_debug(); }

    // sets grid to all zeros
    void inline fill(const geometry::cell_value_t v)
        { static_cast<T*>(this)->fill(v); }

    /**
     * Loads the vector of points as a CCW polygon.
     *
     * @param {std::istream} input stream containing the serialization text
     */
    void inline fill(const geometry::Polygon& src, const geometry::cell_value_t val)
        { static_cast<T*>(this)->fill(src, val); }

    ///! \brief describes the number of divisions along each of the x, y
    /// dimensions
    size_t dimension() const
        { return static_cast<T*>(this)->dimension(); }

    // ///! \brief gets the total count of cells in this data structure
    // size_t size() const 

    void to_string() const
        { static_cast<T*>(this)->to_string(); }

    void inline reset()
        { static_cast<T*>(this)->reset(); }

    std::string summary() const
        { return static_cast<T*>(this)->to_summary(); }

}; // class node<T>

}; // namespace chart::node

#endif // #ifdef _NODE_HPP_
