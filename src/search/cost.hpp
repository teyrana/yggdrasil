// GPL v3 (c) 2020, Daniel Williams 

#ifndef _SEARCH_COST_HPP_
#define _SEARCH_COST_HPP_

#include <cmath>

#include "index/index2u.hpp"

namespace chart::search {

float euclideanCost(const index::Index2u& at, const index::Index2u& goal);

}; // namespace chart::search



#endif // #ifndef _SEARCH_COST_HPP_