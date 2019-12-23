// GPL v3 (c) 2020

namespace yggdrasil::geometry {

// ========= ========= ========= Row Major Index ========= ========= =========
template<size_t dimension, typename cell_t, typename storage_t>
RowMajorIndex<dimension,cell_t,storage_t>::RowMajorIndex( storage_t& _data):
    data(_data)
{}

template<size_t dimension, typename cell_t, typename storage_t>
constexpr cell_t& RowMajorIndex<dimension,cell_t,storage_t>::lookup( const uint32_t i, const uint32_t j) {
    return this->operator()(i,j);
}

template<size_t dimension, typename cell_t, typename storage_t>
constexpr cell_t& RowMajorIndex<dimension,cell_t,storage_t>::operator()( const uint32_t i, const uint32_t j) {
    return data[ i + ( j * dimension) ];
}

template<size_t dimension, typename cell_t, typename storage_t>
constexpr const cell_t& RowMajorIndex<dimension,cell_t,storage_t>::operator()( const uint32_t i, const uint32_t j) const {
    return data[ i + ( j * dimension) ];
}

} // namespace yggdrasil::geometry