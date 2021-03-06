// GPL v3 (c) 2021, Daniel Williams 
#pragma once

// standard library includes
#include <string>
#include <vector>

// GDAL
#include <cpl_json.h>

#include "chart-box.hpp"
#include "chart-base-loader.hpp"

namespace chartbox::io {

/// \brief Load a GeoJSON file into a layer
///
/// References:
///   - https://geojson.org/
///   - https://datatracker.ietf.org/doc/html/rfc7946
///   - https://gdal.org/drivers/vector/geojson.html
template< typename layer_t >
class GeoJSONLoader : ChartBaseLoader<layer_t, GeoJSONLoader<layer_t> > {
public:

    GeoJSONLoader( FrameMapping& _mapping, layer_t& _layer );

    bool load_file(const std::string& filename);
    bool load_text(const std::string& source);
    // bool load_binary( const void& source);

private:
    bool load_json(const CPLJSONObject& doc);

    ///  The values of a "bbox" array are "[west, south, east, north]", not
    ///     "[minx, miny, maxx, maxy]" (see Section 5).  --rfc 7946
    bool load_json_boundary_box(const CPLJSONObject& doc);
    bool load_json_boundary_polygon(const CPLJSONObject& doc);

private:
    FrameMapping& mapping_;
    layer_t& layer_;

};

} // namespace chart::io

#include "chart-geojson-loader.inl"