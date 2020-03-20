// GPL v3 (c) 2020, Daniel Williams 

#include <cmath>

#include "search/cost.hpp"

using chart::index::Index2u;

// ====== ====== ====== Cost Functions (free)  ====== ====== ======
float chart::search::euclideanCost(const Index2u& at, const Index2u& goal){
    return at.norm2(goal);
}
