// GPL v3 (c) 2020, Daniel Williams 

#ifndef _GEOMETRY_FLIP_HPP_
#define _GEOMETRY_FLIP_HPP_

// #include <cstddef>
#include <vector>

namespace chart::geometry {

///! \brief  flips a grid of cells from memory-order to raster order
///
/// Memory order:  7 8 9
///                4 5 6
///                1 2 3
/// 
/// Raster order:  1 2 3
///                4 5 6
///                7 8 9
///
template<typename T>
std::vector<T> vflip(const std::vector<T>& source, const size_t row_width) {
    const size_t row_count = source.size() / row_width;

    std::vector<T> sink(source.size());

    for (size_t read_row = 0; read_row < row_count; ++read_row) {
        size_t write_row = row_count - read_row-1;

        for (size_t column = 0; column < row_width; ++column) {
            sink[write_row*row_width + column] = source[read_row*row_width + column];
        }
    }

    return sink;
}

}; // namespace chart::geometry

#endif // #ifdef _CHART_WRITERS_HPP_
