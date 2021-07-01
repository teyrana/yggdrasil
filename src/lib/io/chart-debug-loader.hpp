// GPL v3 (c) 2021, Daniel Williams 

// standard library includes
#include <string>
#include <vector>

#pragma once

#include "chart-base-loader.hpp"

namespace chartbox::io {

template< typename layer_t >
class DebugLoader : ChartBaseLoader<layer_t, DebugLoader<layer_t> > {
public:

    DebugLoader( layer_t& destination_layer ) { 
        : layer_(_destination_layer)
        {}


    bool load_file(const std::string& filename){ return true; }
    bool load_text(const std::string& source){ return true; }
    // bool load_binary( const std::stream& source);

private:
    layer_t& layer_;

};

} // namespace chart::io