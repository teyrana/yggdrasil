// GPL v3 (c) 2020

#ifndef _INDEX_HPP_
#define _INDEX_HPP_


namespace yggdrasil::geometry {

/// \brief Defines the mapping function from 2d -> 1d (i.e. how i,j coordinates map into a single chunk of memory)
/// \param dimension - count of number of data cells in each direction of the square data
/// \param cell_t - data type stored at each grid cell.
/// \param storage_t - type of the backing data storage.  requires an 'operator[]'
template<size_t dimension, typename cell_t, typename storage_t = std::array<cell_t, dimension*dimension> >
class RowMajorIndex {
public:
    RowMajorIndex() = delete;
    RowMajorIndex(storage_t& values);

    constexpr cell_t& lookup( const uint32_t i, const uint32_t j);
    // constexpr const cell_t& lookup( const uint32_t i, const uint32_t j) const ;

    constexpr cell_t& operator()( const uint32_t i, const uint32_t j);
    constexpr const cell_t& operator()( const uint32_t i, const uint32_t j) const;

private:
    storage_t& data;
};

} // namespace yggdrasil::geometry

#include "row_index.inl"

#endif // #ifdef _GEOMETRY_INDEX_HPP_
