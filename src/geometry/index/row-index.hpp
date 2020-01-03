// GPL v3 (c) 2020

#ifndef _INDEX_HPP_
#define _INDEX_HPP_


namespace yggdrasil::geometry::index {

/// \brief Defines the mapping function from 2d -> 1d (i.e. how i,j coordinates map into a single chunk of memory)
/// \param dimension - count of number of data cells in each direction of the square data
/// \param cell_t - data type stored at each grid cell.
/// \param storage_t - type of the backing data storage.  requires an 'operator[]'
template<size_t dimension, typename cell_t, typename storage_t = std::array<cell_t, dimension*dimension> >
class RowMajorIndex {
public:
    RowMajorIndex() = delete;
    RowMajorIndex(storage_t& _store): store(_store) {}

    constexpr cell_t& operator()( const uint32_t i, const uint32_t j){ return store[ i + (j*dimension)]; }
    constexpr const cell_t& operator()( const uint32_t i, const uint32_t j) const { return store[ i + (j*dimension)]; }

private:
    storage_t& store;
};

} // namespace yggdrasil::geometry::index

#endif // #ifdef _GEOMETRY_INDEX_HPP_
