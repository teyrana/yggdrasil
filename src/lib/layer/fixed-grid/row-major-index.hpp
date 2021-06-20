// GPL v3 (c) 2021, Daniel Williams 

#pragma once

#include <cstdio>

namespace chartbox::layer {

/// \brief Defines the mapping function from 2d -> 1d (i.e. how i,j coordinates
/// map into a single chunk of memory) 
/// \param dimension - count of number of data cells in each direction of the square data 
template <size_t dimension, size_t scale_numerator, size_t scale_denominator>
class RowMajorIndex {
public: 
    typedef Eigen::Matrix<uint32_t,2,1> Vector2u;
    constexpr static double ratio = (scale_numerator / scale_denominator);
public:
    constexpr static size_t lookup( const uint32_t i, const uint32_t j ){
        return i + (j * dimension);
    }

    constexpr static size_t lookup( const Vector2u i ){
        return i[0] + (i[1] * dimension);
    }

    constexpr static size_t lookup( const Eigen::Vector2d i ){
        return lookup( RowMajorIndex::Vector2u( i[0]*ratio, i[1]*ratio ));
    }

    constexpr size_t stride() const { return dimension; }

    constexpr double scale() const { return ratio; }
};

} // namespace chart::index

