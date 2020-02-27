// GPL v3 (c) 2020, Daniel Williams 

#ifndef _SEARCH_REACHABLE_HPP_
#define _SEARCH_REACHABLE_HPP_

#include <cmath>

#include "index/index2u.hpp"

namespace chart::search {

// represents a node reachable, but not yet visited...
class ReachableCell {
public:
    uint32_t i;
    uint32_t j;
    uint32_t cost;

    // ReachableCell(const ReachableCell& orig) = default;  // copy constructor
    // ReachableCell(ReachableCell&& orig) = default;  // move constructor
    ReachableCell(const index::Index2u& p);
    ReachableCell(const index::Index2u& p, uint32_t cost);

    index::Index2u at();

    bool operator<( const ReachableCell& rhs) const;
    // friend bool operator > (const Person& lhs, const Person& rhs);
};

}; // namespace chart::search

#endif // _SEARCH_REACHABLE_HPP_