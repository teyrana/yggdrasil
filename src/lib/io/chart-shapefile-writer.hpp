// GPL v3 (c) 2021, Daniel Williams 
#pragma once

#include 

#include "chart-base-loader.hpp"


namespace chartbox::io {

template< typename layer_t >
class ShapefileLoader : ChartBaseLoader<layer_t, ShapefileLoader<layer_t> > {
public:

    DebugLoader( layer_t& destination_layer ) { 
        : layer_(_destination_layer)
        {}

    bool load_file(const std::string& filename);

    bool load_text(const std::string& source);

private:
    layer_t& layer_;

}; // class ShapefileLoader

} // namespace chartbox::io

#include "chart-shapefile-loader.inl"
