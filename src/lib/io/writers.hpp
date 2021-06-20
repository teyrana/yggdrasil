// GPL v3 (c) 2021, Daniel Williams 

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <Eigen/Geometry>

namespace chart::io {

#ifdef ENABLE_GDAL
///! \brief writes a png file to the given outstream
bool to_grayscale_png(const std::vector<std::byte>& source,
                      const std::string& filename);
#endif

}; // namespace chart::io
