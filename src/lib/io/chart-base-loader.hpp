// GPL v3 (c) 2021, Daniel Williams 

// standard library includes
#include <iostream>


// // first-party includes
// #include "geometry/bounds.hpp"
// #include "geometry/polygon.hpp"

// using chart::base::ChartInterface;
// using chart::geometry::Bounds;
// using chart::geometry::Path;
// using chart::geometry::Polygon;

namespace chartbox::io {

template< typename layer_t, typename loader_t >
class ChartBaseLoader {
public:
    
    ChartBaseLoader( layer_t& destination ) = delete;

    bool load_file(const std::string& filename ) = delete;
    bool load_text(const std::string& source) = 0;
    // bool load_binary( const std::string& source) = 0;

private:
    layer_t& destination_;

};

} // namespace chartbox::io
