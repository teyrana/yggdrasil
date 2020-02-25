// GPL v3 (c) 2020

#ifndef _QUAD_TREE_HPP_
#define _QUAD_TREE_HPP_

#define QUAD_TREE_VERSION "0.0.1"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "geometry/cell_value.hpp"
#include "geometry/layout.hpp"
#include "geometry/sample.hpp"

#include "quadtree/node.hpp"

using std::unique_ptr;

using namespace yggdrasil::geometry;

namespace yggdrasil {

/**
 * Datastructure: A point Quad Tree for representing 2D data. Each
 * region has the same ratio as the bounds for the tree.
 * 
 * The implementation currently requires pre-determined bounds for data as it
 * can not rebalance itself to that degree.
 */
class WorldTree {
public:
    /**
     * Constructs a new quad tree, centered at 0,0 and 1024 units wide, square
     * 
     * Use this constructor if the tree will be loaded from a config file, and the initialization values do not matter.
     */
    WorldTree();
    
    /**
     * Constructs a new quad tree.
     *
     * @param x, y coordinates of tree's center point.
     * @param tree width.  Tree is square. (i.e. height === width)
     */
    WorldTree(const Layout& _layout);

    /**
     *  Releases all memory associated with this quad tree.
     */
    ~WorldTree();

    static size_t calculate_complete_tree(const size_t height);

    /**
     * Returns true if the point at (x, y) exists in the tree.
     *
     * @param {double} x The x-coordinate.
     * @param {double} y The y-coordinate.
     * @return {bool} Whether the tree contains a point at (x, y).
     */
    bool contains(const Eigen::Vector2d& p) const;

    /**
     * retrieve the value of the requested point `p`. 
     *
     * @param {double} x The x-coordinate.
     * @param {double} y The y-coordinate.
     * @param {cell_value_t} opt_default The default value to return if the node doesn't
     *                 exist.
     * @return {cell_value_t} The value of the node, if available; or the default value.
     */
    cell_value_t classify(const Eigen::Vector2d& p) const;

    /**
     * Draws a simple debug representation of this tree to the given
     * output stream. 
     *
     */
    void debug_tree(const bool show_pointer=false) const;

    /**
     * Gets the value of the point at (x, y).  If the point is not close to the center of a node, this function interpolates or extrapolates an appropriate value.
     *
     * @param {double} x The x-coordinate.
     * @param {double} y The y-coordinate.
     * @return {cell_value_t} The resultant value
     */
    cell_value_t interp(const Eigen::Vector2d& at) const;

    ///! \brief sets all leaf nodes to the given value
    ///! \param fill_value - value to write
    void fill(const cell_value_t fill_value);

    cell_value_t operator()(const double x, const double y);

    ///! \brief Get the overall layout of this tree
    ///! 
    ///! \return layout object describing the tree's overall bounds.
    ///!
    inline const Layout& get_layout() const { return layout; }

    size_t get_height() const;
    
    double get_load_factor() const;
    
    size_t get_memory_usage() const;

    /**
     * Loads the vector of points as a CCW polygon.
     *
     * @param {std::istream} input stream containing the serialization text
     */
    void load_polygon(const std::vector<Eigen::Vector2d>& source);

    /**
     * Loads a representation of a tree from the data source.  The the form source is assumed to
     * contain a serialization of a tree, as represented in valid json.  That is, in the same
     * format as Tree::serialize(...).
     *
     * @param tree - input json object containing a tree structure
     */
    bool load_tree(const nlohmann::json& tree);

    void prune();
    
    void reset();

    ///! \brief resets _the tree_ to fully populate the bounds at the given precision
    ///! 
    ///! \param bounds - new bounds to describe
    ///! \param precision - describe the bounds to at least this precision
    void reset(const Layout& new_layout);

    ///! \brief Classify what value the requested point `p` has.
    ///!
    ///! This method is designed for use with an interpolation algorithm.
    ///! 
    ///! \param location to sample near
    ///! @return the point-value-pair _actually_ contained in the tree.
    Sample sample(const Eigen::Vector2d& p) const;
    
    size_t size() const;

    ///! \brief store a value in the tree, at point `p`
    ///! 
    ///! This is the primary method to populate a useable tree.
    ///!
    ///! \param p - the x,y coordinates to write to
    ///! \param new_value - the value to write at point 'p'
    ///! \return success - fails if out-of-bounds.
    bool store(const Eigen::Vector2d& p, const cell_value_t new_value);

    ///! \brief generates a json structure, describing the tree itself
    nlohmann::json to_json_tree() const;

    bool write_png(const std::string filename) const;

private:
    ///! the data layout this tree represents
    geometry::Layout layout;

    unique_ptr<terrain::quadtree::Node> root;

private:
    friend class QuadTreeTest_ConstructDefault_Test;
    friend class QuadTreeTest_LoadValidTree_Test;
    friend class QuadTreeTest_CalculateLoadFactor_Test;
    friend class QuadTreeTest_LoadGridFromJSON_Test;
    friend class QuadTreeTest_WriteLoadCycle_Test;
    friend class QuadTreeTest_SearchExplicitTree_Test;
    friend class QuadTreeTest_InterpolateTree_Test;

};
} // namespace terrain::quadtree

#endif // _QUADTREE_QUAD_TREE_HPP_
