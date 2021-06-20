// GPL v3 (c) 2021, Daniel Williams 

// standard library includes
#include <string>
#include <vector>


// #include <nlohmann/json.hpp>

// // first-party includes
// #include "geometry/bounds.hpp"
// #include "geometry/polygon.hpp"

// using chart::base::ChartInterface;
// using chart::geometry::Bounds;
// using chart::geometry::Path;
// using chart::geometry::Polygon;

#include "chart-base-loader.hpp"
#include "chart-json-keys.hpp"

namespace chartbox::io {

template< typename layer_t >
class JSONLoader : ChartBaseLoader<layer_t, JSONLoader<layer_t> > {
public:

    JSONLoader( layer_t& destination );

    bool load_file(const std::string& filename);
    bool load_text(const std::string& source);
    // bool load_binary( const void& source);

private:
    layer_t& destination_;

    bool load_json(const nlohmann::json doc);

};


///! \brief  flips a grid of cells from memory-order to raster order
///
/// Note: free-function.
/// Memory order:  7 8 9
///                4 5 6
///                1 2 3
/// 
/// Raster order:  1 2 3
///                4 5 6
///                7 8 9
///
template<typename T>
std::vector<T> vflip(const std::vector<T>& source, const size_t row_width);

} // namespace chart::io