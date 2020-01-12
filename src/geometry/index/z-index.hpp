// GPL v3 (c) 2020

#ifndef _Z_INDEX_HPP_
#define _Z_INDEX_HPP_

#include "typedef.hpp"

namespace yggdrasil::geometry::index {

inline constexpr uint8_t _calculate_padding(const size_t dimension);
inline uint64_t _interleave(const uint32_t input);

/// \brief hashes x,y ... by a Z-Order Curve
/// [1] http://en.wikipedia.org/wiki/Z-Order_curve
/// \brief tile in the world tree, containing a grid of data
/// \param T type of each grid cell
/// \param n dimension of the occupancy grid

template <size_t dimension, typename cell_t,
          typename storage_t = std::array<cell_t, dimension * dimension>>
class ZOrderCurveIndex {
  public:
    ZOrderCurveIndex() = delete;
    ZOrderCurveIndex(storage_t& _data);

    constexpr cell_t lookup(const uint32_t i, const uint32_t j);

    constexpr cell_t& operator()(const uint32_t i, const uint32_t j);
    constexpr const cell_t& operator()(const uint32_t i,
                                       const uint32_t j) const;

    constexpr static size_t index_bit_size = 64;
    constexpr static uint8_t padding = _calculate_padding(dimension);

  private:
    storage_t& data;
};

} // namespace yggdrasil::geometry::index

#include "z-index.inl"

#endif // #ifdef _Z_INDEX_HPP_
