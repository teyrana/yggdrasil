// GPL v3 (c) 2020

#ifndef _TILE_NODE_HPP_
#define _TILE_NODE_HPP_

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "index/row-index.hpp"
// #include "index/z_index.hpp"
#include "geometry/polygon.hpp"

#include "node/node_status.hpp"

using std::cerr;
using std::endl;
using std::isnan;
using std::string;
using std::unique_ptr;

using nlohmann::json;

using chart::geometry::Polygon;

namespace chart::node {

template <typename cell_t, size_t dimension> class TileNode {
  public:
    TileNode();

    /**
     * @param layout which this grid will represent
     */
    TileNode(const Eigen::Vector2d& anchor_minimum);

    /**
     *  Releases all memory associated with this quad tree.
     */
    ~TileNode(){};

    ///! \brief loads a json document from the given buffer
    ///!
    ///! Note: this json document may contain either polygons or a grid, and
    ///! will be automatically loaded correctly
    static std::unique_ptr<TileNode> build_from_json(const std::string& buffer);

    ///! \brief retrieve the value at given world coordinate (x, y)
    ///!
    ///! \param p The x-y coordinates (in meters) to search for
    ///! \return The value of the node, if available; or the default value.
    cell_t classify(const Eigen::Vector2d& p) const;

    ///! \brief check if this tile contains a given point
    ///!
    ///! \param p The x-y coordinates (in meters) to test at
    ///! @return Whether the tree contains the given point
    bool contains(const Eigen::Vector2d& p) const;

    ///! \brief sets tile to all zeros
    void fill();

    ///! \brief sets tile to all 'value'
    ///! \param fill_value - fill value for entire grid
    void fill(const cell_t fill_value);

    ///! \brief sets tile contents
    ///! \param fill_value - fill value to _move_ into this tile.
    void fill(const std::vector<cell_t>& fill_data);

    ///! \brief Fills the _interior_ of the given polygon with the given value.
    ///!
    ///! @param source - polygon defining the fill araea. Assumed to be closed,
    /// CCW, and non-intersecting ! @param fill_value -fill value for area
    void fill(const Polygon& source, const cell_t fill_value);

    const geometry::Bounds get_bounds() const;

    cell_t get_value() const;

    /// \brief used for loading a flatbuffer data structure
    bool load_from_flatbuffer(const std::byte* const buffer);

    ///! \brief loads a .shp shapefile into this tile
    bool load_from_shapefile(const std::string& path);

    inline void reset() {}

    // void set(const double x, const double y, const T new_value);

    NodeStatus status() const;

    bool store(const Eigen::Vector2d& p, const cell_t new_value);

    ///! \brief writes a json document to given output stream
    std::string to_json() const;

    std::string to_string() const;

    std::vector<std::byte> to_raster() const;

    const std::byte* const to_flatbuffer();

  private:
    cell_t& get_cell(const size_t xi, const size_t yi);
    cell_t get_cell(const size_t xi, const size_t yi) const;

  private:
    // used for reading and write json documents:
    const static std::string anchor_key;
    const static std::string grid_key;
    const static std::string x_key;
    const static std::string y_key;

  public:
    constexpr static cell_t cell_default_value = 42; // == '*'

    constexpr static double width = dimension;
    constexpr static double scale = dimension / width;

    ///! \brief the _total_ number of cells in this grid === (width * height)
    constexpr static size_t size = dimension * dimension;

    /// \brief === lower-left / minimum-value point.
    const Eigen::Vector2d anchor;

  private:
    /// \brief contains the data for this tile
    std::array<cell_t, size> data;

    /// \brief defines lookup method for data in this tile
    chart::index::RowMajorIndex<decltype(data), dimension> index;

    NodeStatus status_;

  private:
    friend class Tile_CacheRoundTrip_Test;
    friend class Tile_LoadPolygonFromVector_Test;
    friend class Tile_LoadShapefile_Test;

}; // class TileNode

// All typedefs declared here have an explicit template specialization
// in the corresponding source file
typedef TileNode<uint8_t, 4> TestTile;
typedef TileNode<uint8_t, 32> Tile1k;
typedef TileNode<uint8_t, 1024> Tile1M;

}; // namespace chart::node

#endif // #ifdef _TILE_NODE_HPP_
