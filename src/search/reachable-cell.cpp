// GPL v3 (c) 2020, Daniel Williams 

#include <cmath>

#include "search/reachable-cell.hpp"

using chart::index::Index2u;
using chart::search::ReachableCell;

// ====== ====== ====== Cost Functions (free)  ====== ====== ======
namespace chart::search {

uint32_t manhattanCost(const Index2u& p1, const Index2u& p2){
    return p1.norm1(p2);
}

uint32_t euclideanCost(const Index2u& p1, const Index2u& p2){
    return p1.norm2(p2);
}

// ====== ====== ====== Cost Functions (free)  ====== ====== ======
ReachableCell::ReachableCell(const index::Index2u& p)
    : i(p.i), j(p.j), cost(0)
{}

ReachableCell::ReachableCell(const index::Index2u& p, uint32_t _cost)
    : i(p.i), j(p.j), cost(_cost)
{}

bool ReachableCell::operator<(const ReachableCell& rhs) const {
    return (this->cost < rhs.cost);
}

};  // namespace chart::search
