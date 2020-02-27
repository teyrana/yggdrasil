// GPL v3 (c) 2020, Daniel Williams 

#include <queue>

#include "search/cost.hpp"
#include "search/reachable-cell.hpp"

using Eigen::Vector2d;

using chart::base::ChartInterface;
using chart::geometry::Bounds;
using chart::geometry::Path;
using chart::index::Index2u;
using chart::search::AStar;
using chart::search::ReachableCell;

using chart::search::manhattanCost;

template<typename chart_t>
AStar<chart_t>::AStar( const chart_t& _chart ) : chart_(_chart) {}
        
template<typename chart_t>
Path AStar<chart_t>::compute( const Vector2d& start_point, const Vector2d& goal_point ) const {
    typedef typename chart_t::cell_type cell_t;

    // if this bit is set, this cell is visited
    // constexpr cell_t visited = 0x80;

    std::priority_queue<ReachableCell> upcoming;

    const Index2u start_cell = chart_.discretize(start_point);
    const Index2u goal_cell = chart_.discretize(goal_point);

    upcoming.emplace( start_cell, manhattanCost(start_cell, goal_cell));
    upcoming.emplace( goal_cell, 0);

    // std::vector<Path> explored_paths; // TBD?

    cerr << ">> Starting search with " << upcoming.size() << " cells...\n";
    while( 0 < upcoming.size() ){

        // pop next node, closest available to our goal
        const ReachableCell& next = upcoming.top();
        upcoming.pop();
        cerr << "    @ " << next.i << ", " << next.j << " = " << next.cost << endl;
        
        // If we found the goal, build and return the path.
        if( 0 == next.cost ){
            // debug;
            return { start_point, goal_point };
            // return build_path(goal_node)
        }
   
        // // Add next nodes (neighbors) to our search list
        // auto neighbors = get4Neighbors(next);
        // for( auto& n : neighbors ){

        //     ??? the coordinate or the value ???
        //     auto& cell = chart_.get(next.at);
            
        //     if( ! chart_.contains(next.at) )
        //         continue; // out-of-bounds. ignore.

        //     if( cell & visited )
        //         continue; // already visited. ignore.

        //     upcoming.emplace(n, manhattanCost(cell, goal_cell));

        //     // Mark Visited
        //     chart_.get(next.at) |= visited;
        // }

        //     new_reachable = get_adjacent_nodes(node) - explored
        //     for adjacent in new_reachable:
        //         # First time we see this node?
        //         if adjacent not in reachable:
        //             reachable.add(adjacent)
            
        //         # If this is a new path, or a shorter path than what we have, keep it.
        //         if node.cost + 1 < adjacent.cost:
        //             adjacent.previous = node
        //             adjacent.cost = node.cost + 1

        //debug
        break;
    }

    // debug;
    return { start_point, goal_point };

    // If we get here, no path was found
    return Path::Empty;
}

// template<typename chart_t>
// std::array<ReachableCell, 8> AStar<chart_t>::get8Neighbors(const ReachableCell& center){
//     const uint32_t i = center.at.i;
//     const uint32_t j = center.at.j;
//     // allow the integer overflows, and bounds check at the call site
//     return [[i+1,j], [i+1,j+1], [i,j+1], [i-1,j+1]]
//             [i-1,j], [i-1,j-1], [i,j-1], [i+1,j-1]];

// }

// template<typename chart_t>
// std::array<ReachableCell, 4> AStar<chart_t>::get4Neighbors(const ReachableCell& center){
//     const uint32_t i = center.at.i;
//     const uint32_t j = center.at.j;
//     // allow the integer overflows, and bounds check at the call site
//     return [[i+1,j], [i,j+1], [i-1,j], [i,j-1]];
// }