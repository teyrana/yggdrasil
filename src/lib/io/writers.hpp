// GPL v3 (c) 2020, Daniel Williams 

#ifndef _CHART_WRITERS_HPP_
#define _CHART_WRITERS_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include <nlohmann/json.hpp>

#include "io/json.hpp"

namespace chart::io {

#ifdef ENABLE_GDAL
///! \brief writes a png file to the given outstream
bool to_grayscale_png(const std::vector<std::byte>& source,
                      const std::string& filename);
#endif

}; // namespace chart::io

#endif // #ifdef _CHART_WRITERS_HPP_
