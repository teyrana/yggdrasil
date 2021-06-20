// GPL v3 (c) 2021, Daniel Williams 

#pragma once

#include <Eigen/Geometry>

// #include "geometry/bounds.hpp"
// #include "geometry/path.hpp"
// #include "geometry/polygon.hpp"
// #include "index/index2u.hpp"

namespace chartbox {

// base class of a CRTP pattern, as described here:
// https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/
// https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c
template<typename cell_t, typename layer_t>
class ChartLayerInterface {
protected:
    ChartLayerInterface() = default;

public:
    /// \brief Retrieve the value at an (x, y) Eigen::Vector2d
    ///
    /// \param Eigen::Vector2d - the x,y coordinates to search at
    /// \return the cell value
    cell_t get(const Eigen::Vector2d& p) const { return layer().get(p); }

    // { return static_cast<chart_t*>(this)->classify(p, default_value); }

    // /// \brief Retrieve the value at an (x, y) Eigen::Vector2d
    // ///
    // /// \param Eigen::Vector2d - the x,y coordinates to search at
    // /// \note  this version has a default value of '0xFF'
    // /// \return the cell value
    // cell_t classify(const Eigen::Vector2d& p) const {
    //     return static_cast<chart_t*>(this)->classify(p); }

    // /// \brief Test if this data structer contains this index-point
    // /// \param The 2d-index to test
    // /// \return true if this data structure contains this index
    // bool contains(const index::Index2u& index) const {

    std::string name() const { return layer().name_; }

    bool fill(const cell_t value) const { return layer().fill(value); }

    void reset() { layer().reset(); }

    std::string type() const { return layer().type_name_; }

    // /// \brief Fills the _interior_ of the given polygon with the given value.
    // /// 
    // /// \param source - polygon defining the fill araea. Assumed to be closed, CCW, and non-intersecting
    // /// \param fill_value -fill value for area
    // bool fill(const geometry::Polygon& area, const cell_t value);

    // bool fill(const std::vector<cell_t>& source) {
    //     return static_cast<chart_t*>(this)->fill_from_buffer(source); }

    // /// \brief Access the value at an (x, y) Eigen::Vector2d
    // ///!
    // /// \param Eigen::Vector2d - the x,y coordinates to search at:
    // /// \return reference to the cell value
    // bool store(const Eigen::Vector2d& p, const cell_t value) 
    //     { return static_cast<chart_t*>(this)->store(p, value); }

    // bool load_json(const std::string& source);

    // void fill_from_polygon(target_t& t, const chart::geometry::Polygon& poly,
    //                     const cell_t fill_value);

    // ///! \brief loads all the allowed and blocked areas
    // ///! @param allow - a (json) list of allowed areas, as defined by polygons, as
    // /// defined by a list of points. ! @param block - a (json) list of blocked
    // /// areas, as defined by polygons, as defined by a list of points.
    // bool load_areas_from_json(target_t& chart, nlohmann::json allow,
    //                         nlohmann::json block);

#ifdef ENABLE_GDAL
    ///! \brief load a .shp file into this chart.
    // virtal inline geometry::Polygon _fill_interior(const OGRLinearRing& source, cell_t value ) = 0;

    ///! \brief load a .shp file into this chart.
    // bool load_from_shape_file(target_t& chart, const std::string& filepath);
#endif

protected:

    layer_t& layer() {
        return *static_cast<layer_t*>(this);
    }

    const layer_t& layer() const {
        return *static_cast<const layer_t*>(this);
    }

    ~ChartLayerInterface() = default;
    
}; // class ChartLayerInterface< cell_t, layer_t >

} // namespace chart
