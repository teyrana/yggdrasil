// GPL v3 (c) 2021, Daniel Williams

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "chart-layer-interface.hpp"

#include "layer/fixed-grid/fixed-grid.hpp"

namespace chartbox {

/// \brief A container of containers for various types of map data structures
class ChartBox {
public:
    ChartBox();
    
    int locate( const Eigen::Vector2d& center, const double width );

    //// \brief retrieve the value of the requested point `p`. 
    ///
    /// @param {double} x The x-coordinate; in real-world units; meters
    /// @param {double} y The y-coordinate; in real-world units; meters
    /// @return {cell_value_t} The value of the node, if available; or the default value.
    int classify(const Eigen::Vector2d& p) const;

    
    // bool write_png(const std::string filename) const;

    // \warn you may only create at the top of the layer stack.  plan ahead.
    // \warn you may only create via constructing-in-place / indirectly. 
    // \return index of created layer, or -1 for error
    // int add_grid_layer();
    // int add_tree_layer();

    // size_t get_layer_count();

    // /// \brief layer access
    // /// \param index -- index of desired layer; 0 is the first and bottom layer
    // /// \return request layer or null, if not available
    // void* get_layer_at_index( const size_t index);

    // void* operator[]( const size_t index );

    // TODO: implement me
    // auto get_boundary_layer();

    std::string summary() const;

    /// \brief Releases all memory associated with this quad tree.
    ~ChartBox();

private:
    // defines boundaries in local frame, physical space
    Eigen::AlignedBox2d bounds_;

    chartbox::layer::FixedGridLayer _grid_layer;
    // TreeLayer _tree_layer;

// private:
//     friend class QuadTreeTest_ConstructDefault_Test;
//     friend class QuadTreeTest_LoadValidTree_Test;
//     friend class QuadTreeTest_CalculateLoadFactor_Test;
//     friend class QuadTreeTest_LoadGridFromJSON_Test;
//     friend class QuadTreeTest_WriteLoadCycle_Test;
//     friend class QuadTreeTest_SearchExplicitTree_Test;
//     friend class QuadTreeTest_InterpolateTree_Test;

};
} // namespace chart

