// GPL v3 (c) 2020, Daniel Williams 

#include <cmath>
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include "nlohmann/json.hpp"

#include "geometry/bounds.hpp"
#include "geometry/path.hpp"

#include "grid/fixed-grid.hpp"
#include "search/a-star.hpp"
#include "search/reachable-cell.hpp"

using std::cerr;
using std::endl;
using std::isnan;
using std::string;

using Eigen::Vector2d;
using nlohmann::json;

using chart::geometry::Path;
using chart::grid::FixedGrid1k;
using chart::search::AStar;
using chart::search::ReachableCell;


TEST( SearchAStar, ConstructDefault64) {
    FixedGrid1k g;
    AStar<FixedGrid1k> search(g);
}

TEST(SearchAStar, DirectPath){
    FixedGrid1k g(Bounds({0,0}, {32,32}));  // start with a 1-cell-to-1-meter spacing.
    AStar<FixedGrid1k> search(g);

    g.fill('.');

    Path foundPath = search.compute( {2,2}, {24,24} );

    g.fill(foundPath, 'o');

    // DEBUG
    cerr << g.to_string() << endl;

}
