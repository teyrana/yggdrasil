// GPL v3 (c) 2020, Daniel Williams 

#ifndef _CHART_GRID_HPP_
#define _CHART_GRID_HPP_

#include <cmath>
#include <memory>
#include <cstdlib>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include "nlohmann/json.hpp"

#include "base/chart-interface.hpp"
#include "geometry/polygon.hpp"
#include "geometry/bounds.hpp"
#include "index/row-index.hpp"
#include "index/index2u.hpp"

namespace chart::grid {

template<typename cell_t, size_t dim>
class FixedGrid : public ::chart::base::ChartInterface< cell_t, FixedGrid<cell_t,dim> > {
public:

    /**
     * Constructs a new quad tree, centered at 0,0 and 1024 units wide, square
     * 
     * Use this constructor if the tree will be loaded from a config file, and the initialization values do not matter.
     */
    FixedGrid();
    
    /**
     * Constructs a new 2d square grid
     */
    FixedGrid(const geometry::Bounds& _bounds);

    FixedGrid(double precision, const geometry::Bounds& _bounds);

    /**
     *  Releases all memory associated with this quad tree.
     */
    ~FixedGrid(){};

    bool blocked(const index::Index2u& at) const;

    /**
     * Get the overall bounds of this tree
     *
     * @return Bounds object describing the tree's overall bounds.
     */
    const Bounds& bounds() const;

    /// \brief Retrieve the value at an (x, y) Eigen::Vector2d
    ///
    /// \param Eigen::Vector2d - the x,y coordinates to search at
    /// \param default_value to return on out-of-bounds
    /// \return the cell value
    cell_t classify(const Eigen::Vector2d& p, const cell_t default_value) const;

    /// note: this version has a default value of '0xFF'
    cell_t classify(const Eigen::Vector2d& p) const;

    // Overide from ChartInterface
    bool contains(const Eigen::Vector2d& p) const;

    // Overide from ChartInterface
    bool contains(const index::Index2u& i) const;

    constexpr static size_t dimension() { return dim; }

    /// \brief convert a nav-space location to a storage-space index
    /// \warning behavior is undefined if `location` is outside of chart bounds
    /// \warning rounds up, on borders
    index::Index2u as_index(const Eigen::Vector2d& location) const;

    /// \brief convert a storage-space location to a navigation-space location
    /// \warning behavior is undefined if `index` is outside of chart bounds 
    Vector2d as_location(const index::Index2u& index) const;

    /// \brief sets the entire grid to the given value
    /// \param fill_value - fill value for entire grid
    bool fill_uniform(const cell_t fill_value);

    /// \brief Fill the given area with the given value.
    /// 
    /// param source - bounds defining the fill araea.
    /// \param fill_value - value to write inside the area
    bool fill_from_bounds(const geometry::Bounds& area, const cell_t value);

    /// \brief Load data into the grid, in memory-order
    /// \param load n cells from the given pointer 
    int fill_from_buffer(const std::vector<cell_t>& source);

    /// \brief simply returns the value or reference to the internal data
    /// \warning !! DOES NOT CHECK BOUNDS !!
    cell_t& get_cell(const size_t xi, const size_t yi);
    cell_t get_cell(const size_t xi, const size_t yi) const ;

    cell_t& operator[](const index::Index2u& i);
    cell_t operator[](const index::Index2u& i) const ;

    /// the spacing of each cell === center-to-center distance. === cell-width.
    double precision() const;

    /// \brief the _total_ number of cells in this grid === (width * height)
    size_t size() const;

    void set_bounds(const geometry::Bounds& newBounds);

    /// \brief Access the value at an (x, y) Eigen::Vector2d
    ///!
    /// \param Eigen::Vector2d - the x,y coordinates to search at:
    /// \return reference to the cell value
    bool store(const Eigen::Vector2d& p, const cell_t new_value);

    bool to_png(const std::string filename) const;

    /// \brief Draws a simple debug representation of this grid to stderr
    std::string to_string() const;

    /// \brief the _total_ number of cells in this grid === (width * height)
    size_t width() const;

public:
    /// \brief the data layout this grid represents
    Bounds bounds_;

    /// how wide each cell is, in real units
    double precision_;

    /// \brief contains the data for this tile
    // raw array:  2D addressing is performed through the index, below
    std::array<cell_t, dim*dim> grid;

    /// \brief defines lookup method for data in this tile
    chart::index::RowMajorIndex<dim> index;

    constexpr static cell_t blocking_threshold = 'A'; // == debugging value

};

typedef FixedGrid<uint8_t, 8> FixedGrid64;
typedef FixedGrid<uint8_t, 32> FixedGrid1k;

} // namespace chart::grid

#endif // _GRID_HPP_
