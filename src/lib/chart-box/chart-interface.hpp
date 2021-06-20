


    /// \brief Retrieve the value at an (x, y) Eigen::Vector2d
    ///
    /// \param Eigen::Vector2d - the x,y coordinates to search at
    /// \param default_value to return on out-of-bounds
    /// \return the cell value
    cell_t classify(const Eigen::Vector2d& p, const cell_t default_value) const {
        return static_cast<chart_t*>(this)->classify(p, default_value); }

    /// \brief Retrieve the value at an (x, y) Eigen::Vector2d
    ///
    /// \param Eigen::Vector2d - the x,y coordinates to search at
    /// \note  this version has a default value of '0xFF'
    /// \return the cell value
    cell_t classify(const Eigen::Vector2d& p) const {
        return static_cast<chart_t*>(this)->classify(p); }
    bool fill(const geometry::Bounds& area, const cell_t value) {
        return static_cast<chart_t*>(this)->fill_from_bounds(area, value); }

    bool fill(const geometry::Path& source, const cell_t fill);

    /// \brief Fills the _interior_ of the given polygon with the given value.
    /// 
    /// \param source - polygon defining the fill araea. Assumed to be closed, CCW, and non-intersecting
    /// \param fill_value -fill value for area
    bool fill(const geometry::Polygon& area, const cell_t value);

    bool fill(const std::vector<cell_t>& source) {
        return static_cast<chart_t*>(this)->fill_from_buffer(source); }

    /// \brief Access the value at an (x, y) Eigen::Vector2d
    ///!
    /// \param Eigen::Vector2d - the x,y coordinates to search at:
    /// \return reference to the cell value
    bool store(const Eigen::Vector2d& p, const cell_t value) 
        { return static_cast<chart_t*>(this)->store(p, value); }

    // void fill_from_polygon(target_t& t, const chart::geometry::Polygon& poly,
    //                     const cell_t fill_value);

    // ///! \brief loads a generic / unspecified json document from the given input
    // /// stream
    // ///!
    // ///! Note: this json document may contain either polygons or a grid, and will be
    // /// automatically loaded correctly
    // bool load_from_json(target_t& chart, std::string doc);

    // ///! \brief loads content from a generic / unspecified json document
    // bool load_from_json(target_t& chart, nlohmann::json grid);

    // ///! \brief loads a generic / unspecified json document from the given input
    // /// stream
    // bool load_grid_from_json(target_t& chart, nlohmann::json grid);

    // ///! \brief loads all the allowed and blocked areas
    // ///! @param allow - a (json) list of allowed areas, as defined by polygons, as
    // /// defined by a list of points. ! @param block - a (json) list of blocked
    // /// areas, as defined by polygons, as defined by a list of points.
    // bool load_areas_from_json(target_t& chart, nlohmann::json allow,
    //                         nlohmann::json block);

    // ///! \brief loads list of polygons from json, into a structure
    // ///!
    // ///! @param allow - a (json) list of allowed areas, as defined by polygons, as
    // /// defined by a list of points. ! @return vector of polygons
    // inline std::vector<geometry::Polygon> make_polygons_from_json(nlohmann::json list);
