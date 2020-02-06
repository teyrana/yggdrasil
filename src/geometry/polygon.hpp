// GPL v3 (c) 2020

#ifndef _GEOMETRY_POLYGON_HPP_
#define _GEOMETRY_POLYGON_HPP_

#include <initializer_list>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "geometry/bounds.hpp"

namespace chart::geometry {

class Polygon {
  public:
    Polygon();
    Polygon(size_t initial_capacity);
    Polygon(nlohmann::json doc);
    Polygon(std::vector<Eigen::Vector2d>& init);
    Polygon(std::initializer_list<Eigen::Vector2d> init);

    inline const auto begin() const { return points.cbegin(); }

    ///! \brief when manually constructing a polygon, call this to ensure it
    /// format-matches
    void complete();

    // clears the internal point vector
    void clear();

    void emplace(const double x, const double y);

    inline const auto end() const { return points.cend(); }

    static Polygon make_diamond(const double width);

    // Retrieves the precomputed center of the polygon:
    // Currently, this is a naive, unweighted average of the polygon points.
    // \return was the load successful?
    // \sidef
    bool load(std::vector<Eigen::Vector2d> source);

    bool load(nlohmann::json doc);

    const Bounds& get_bounds() const;

    void move(const Eigen::Vector2d& delta);

    Eigen::Vector2d& operator[](const size_t index);

    const Eigen::Vector2d& operator[](const size_t index) const;

    void push_back(const Eigen::Vector2d p);
    void resize(size_t capacity);
    size_t size() const;

    // dumps the contains points to stderr
    // \param title - text to print in the output header
    // \param pts - set of points to dump
    void write_yaml(std::ostream& sink, std::string indent = "") const;

  protected:
    // if necessary, adds an extra point to the end of the polygon-point vector
    // to wrap it back to the first point. this is not strictly operationally
    // necessary, but it simplifies most of the algorithms that need to iterate
    // over the points.
    void enclose_polygon();

    // \brief isRightHanded()
    // calculates if the polygon is defined in a right-handed // CCW direction.
    // runs in O(n).
    //
    // See Also: https://en.wikipedia.org/wiki/Shoelace_formula
    //
    // \returns true: polgyon is right-handed.  false: left-handed
    bool is_right_handed() const;

    void set_default();

    void recalculate_bounding_box();

  protected: // Configuration parameters
    std::vector<Eigen::Vector2d>
        points; ///< Main data store for this class.
                ///< Contains the vertices of the polygon
    Bounds bounds;

  private:
    friend class Polygon_DefaultConfiguration_Test;
    friend class Polygon_LoadList_5Point_Test;
    // friend class Polygon_LoadList_Diamond_Test;
};

} // namespace chart::geometry

#endif // #endif _GEOMETRY_POLYGON_HPP_
