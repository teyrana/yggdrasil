// GPL v3 (c) 2020, Daniel Williams 

#ifndef _CHART_SEARCH_A_STAR_HPP_
#define _CHART_SEARCH_A_STAR_HPP_

#include <cmath>
#include <memory>
#include <cstdlib>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include "nlohmann/json.hpp"

#include "geometry/path.hpp"
#include "geometry/bounds.hpp"
#include "geometry/polygon.hpp"
#include "index/row-index.hpp"

#include "search/reachable-cell.hpp"

namespace chart::search {

template<typename chart_t>
class AStar {
public:

    AStar() = delete;
    
    AStar( const chart_t & chart );
        
    /// \brief 
    /// 
    /// https://gabrielgambetta.com/astar-demystified.html
    ///
    /// \param start - location to start searching from
    /// \param end  - location to searching to
    chart::geometry::Path compute( const Eigen::Vector2d& start, const Eigen::Vector2d& end ) const;

private: 
    std::array<ReachableCell, 8> get8Neighbors(const ReachableCell& center);
    std::array<ReachableCell, 4> get4Neighbors(const ReachableCell& center);

public:
    const chart_t & chart_;

};

}; // namespace chart::search

#include "search/a-star.inl"

#endif // #define _CHART_SEARCH_A_STAR_HPP_
