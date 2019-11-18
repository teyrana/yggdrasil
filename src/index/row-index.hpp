// GPL v3 (c) 2020, Daniel Williams 

#ifndef _ROW_INDEX_HPP_
#define _ROW_INDEX_HPP_

#include "index2u.hpp"

namespace chart::index {

/// \brief Defines the mapping function from 2d -> 1d (i.e. how i,j coordinates
/// map into a single chunk of memory) 
/// \param dimension - count of number of data cells in each direction of the square data 
/// \param cell_t - data type stored at each grid cell. 
/// \param storage_t - type of the backing data storage.  requires an 'operator[]'
template <size_t dimension>
class RowMajorIndex {
public:
    constexpr static size_t lookup( const uint32_t i, const uint32_t j ){
        return i + (j * dimension);
    }

    constexpr static size_t lookup( const Index2u& at ){
        return at.i + (at.j * dimension);
    }

    constexpr size_t stride() const { return dimension; }
};

} // namespace chart::index

#endif // #ifdef _ROW_INDEX_HPP_
