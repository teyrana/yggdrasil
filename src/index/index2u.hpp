// GPL v3 (c) 2020, Daniel Williams 

#ifndef _INDEX_TYPEDEF_HPP_
#define _INDEX_TYPEDEF_HPP_

#include <Eigen/Core>

namespace chart::index {

typedef Eigen::Matrix<uint32_t, 2, 1> Vector2u;

typedef Eigen::Matrix<uint32_t, 2, 1> Index2u;

}; // namespace chart::index

#endif // #ifndef _INDEX_TYPEDEF_HPP_