// GPL v3 (c) 2020, Daniel Williams 

#include <cmath>
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include "nlohmann/json.hpp"

#include "geometry/bounds.hpp"
#include "geometry/polygon.hpp"
#include "geometry/raster.hpp"

#include "grid/fixed-grid.hpp"


using std::cerr;
using std::endl;
using std::isnan;
using std::string;

using Eigen::Vector2d;
using nlohmann::json;

using chart::grid::FixedGrid1k;

namespace chart::grid {

TEST( FixedGrid, ConstructDefault64) {
    FixedGrid64 g;
 
    auto& bds = g.bounds();
    EXPECT_DOUBLE_EQ( bds.max().x(),  4.);
    EXPECT_DOUBLE_EQ( bds.max().y(),  4.);
    EXPECT_DOUBLE_EQ( bds.min().x(), -4.);
    EXPECT_DOUBLE_EQ( bds.min().y(), -4.);

    EXPECT_DOUBLE_EQ( bds.size().x(), 8);
    EXPECT_DOUBLE_EQ( bds.size().y(), 8);

    EXPECT_DOUBLE_EQ( g.precision(), 1.0);
}

TEST( FixedGrid, ConstructDefault1k) {
    FixedGrid1k g;
 
    auto& bds = g.bounds();
    EXPECT_DOUBLE_EQ( bds.max().x(),  16.);
    EXPECT_DOUBLE_EQ( bds.max().y(),  16.);
    EXPECT_DOUBLE_EQ( bds.min().x(), -16.);
    EXPECT_DOUBLE_EQ( bds.min().y(), -16.);

    EXPECT_DOUBLE_EQ( bds.size().x(), 32);
    EXPECT_DOUBLE_EQ( bds.size().y(), 32);

    EXPECT_DOUBLE_EQ( g.precision(), 1.0);
}

TEST( FixedGrid, ConstructFromSquareBounds) {
    FixedGrid1k g(Bounds({0., 1.},{32., 33.}));

    auto& bds = g.bounds();
    EXPECT_DOUBLE_EQ( bds.max().x(), 32.);
    EXPECT_DOUBLE_EQ( bds.max().y(), 33.);
    EXPECT_DOUBLE_EQ( bds.min().x(), 0.);
    EXPECT_DOUBLE_EQ( bds.min().y(), 1.);

    EXPECT_DOUBLE_EQ( bds.size().x(), 32.);
    EXPECT_DOUBLE_EQ( bds.size().y(), 32.);

    EXPECT_DOUBLE_EQ( g.precision(), 1.0);
}

TEST( FixedGrid, ConstructFromUnevenBounds) {
    FixedGrid1k g(Bounds({-2., -4.},{2., 4.}));

    auto& bds = g.bounds();
    EXPECT_DOUBLE_EQ( bds.max().x(), 16.);
    EXPECT_DOUBLE_EQ( bds.max().y(), 16.);
    EXPECT_DOUBLE_EQ( bds.min().x(), -16.);
    EXPECT_DOUBLE_EQ( bds.min().y(), -16.);
    
    EXPECT_DOUBLE_EQ( bds.size().x(), 32.);
    EXPECT_DOUBLE_EQ( bds.size().y(), 32.);

    EXPECT_DOUBLE_EQ( g.precision(), 1.0);
}

TEST( FixedGrid, ConstructFromOversizeBounds) {
    FixedGrid1k g(Bounds({-2,-55},{2., 55.}));

    EXPECT_DOUBLE_EQ( g.precision(), 1.0);

    auto& b = g.bounds();
    EXPECT_DOUBLE_EQ( b.max().x(), 16.);
    EXPECT_DOUBLE_EQ( b.max().y(), 16.);
    EXPECT_DOUBLE_EQ( b.min().x(), -16.);
    EXPECT_DOUBLE_EQ( b.min().y(), -16.);

    EXPECT_DOUBLE_EQ( b.size().x(), 32.);
    EXPECT_DOUBLE_EQ( b.size().y(), 32.);
}

TEST( FixedGrid, UpdateBounds) {
    const Bounds b0({-2., -4.},{2., 4.});
    FixedGrid1k g(b0);

    auto& b1 = g.bounds();
    EXPECT_DOUBLE_EQ( b1.min().x(), -16.);
    EXPECT_DOUBLE_EQ( b1.min().y(), -16.);
    EXPECT_DOUBLE_EQ( b1.max().x(), 16.);
    EXPECT_DOUBLE_EQ( b1.max().y(), 16.);
    
    g.set_bounds(Bounds({10,12},{42,44}));

    EXPECT_DOUBLE_EQ( g.precision(), 1.);
    auto& b3 = g.bounds();
    EXPECT_DOUBLE_EQ( b3.min().x(), 10.);
    EXPECT_DOUBLE_EQ( b3.min().y(), 12.);
    EXPECT_DOUBLE_EQ( b3.max().x(), 42.);
    EXPECT_DOUBLE_EQ( b3.max().y(), 44.);
}

static const std::vector<uint8_t> default_tile = chart::geometry::vflip<uint8_t>({
    66, 67, 67, 67,  68, 68, 68, 69,
    65, 66,  0,  0,   0,  0, 69, 70,
    65,  0, 66,  0,   0, 69,  0, 70,
    65,  0,  0, 66,  69,  0,  0, 70,

    76,  0,  0, 75,  72,  0,  0, 71,
    76,  0, 75,  0,   0, 72,  0, 71,
    76, 75,  0,  0,   0,  0, 72, 71,
    75, 74, 74, 74,  73, 73, 73, 72}, 8);

TEST( FixedGrid, LoadFromVector) {
    FixedGrid64 g;

    ASSERT_EQ(default_tile.size(), g.size());

    ASSERT_TRUE( g.fill(default_tile) );

    // // DEBUG
    // cerr << g.to_string() << endl;

    ASSERT_EQ( g.get_cell(0,0), 75);
    ASSERT_EQ( g.get_cell(1,1), 75);
    ASSERT_EQ( g.get_cell(2,2), 75);
    ASSERT_EQ( g.get_cell(3,3), 75);
    ASSERT_EQ( g.get_cell(4,4), 69);
    ASSERT_EQ( g.get_cell(5,5), 69);
    ASSERT_EQ( g.get_cell(6,6), 69);
    ASSERT_EQ( g.get_cell(7,7), 69);

    ASSERT_EQ( g.get_cell(3,0), 74);
    ASSERT_EQ( g.get_cell(3,1),  0);
    ASSERT_EQ( g.get_cell(3,2),  0);
    ASSERT_EQ( g.get_cell(3,3), 75);
    ASSERT_EQ( g.get_cell(3,4), 66);
    ASSERT_EQ( g.get_cell(3,5),  0);
    ASSERT_EQ( g.get_cell(3,6),  0);
    ASSERT_EQ( g.get_cell(3,7), 67);

    ASSERT_EQ( g.get_cell(4,4), 69);
    ASSERT_EQ( g.get_cell(4,5),  0);
    ASSERT_EQ( g.get_cell(4,6),  0);
    ASSERT_EQ( g.get_cell(4,7), 68);
}

TEST( FixedGrid, ClassifyIntoVector) {
    FixedGrid64 g;

    g.set_bounds({{32,32}, {64,64}});

    EXPECT_DOUBLE_EQ( g.precision(), 4. );
    EXPECT_DOUBLE_EQ( g.width(), 32.0 );
    EXPECT_DOUBLE_EQ( g.bounds().min().x(), 32.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(), 32.);
    EXPECT_DOUBLE_EQ( g.bounds().max().x(), 64.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(), 64.);

    ASSERT_TRUE( g.fill(default_tile) );

    // // DEBUG
    // cerr << g.to_string() << endl;

    // OOB
    ASSERT_EQ( g.classify({  1.0, -1.0}), 255);
    ASSERT_EQ( g.classify({ -1.0,  1.0}), 255);
    ASSERT_EQ( g.classify({ -1.0, -1.0}), 255);

    // first cell (first in memory; NW in nav-space)
    // Coordinates are in x,y, from the SouthWest corner...
    ASSERT_EQ( g.classify({ 32.01, 32.01}), 75);
    ASSERT_EQ( g.classify({ 32.1,  32.1}),  75);
    ASSERT_EQ( g.classify({ 33.0,  33.5}),  75);
    ASSERT_EQ( g.classify({ 34.0,  34.5}),  75);
    ASSERT_EQ( g.classify({ 35.9,  35.9}),  75);
    ASSERT_EQ( g.classify({ 35.99, 35.99}), 75);

    // cell at (2,0)
    ASSERT_EQ( g.classify({ 43.9, 36.1}), 0);
    ASSERT_EQ( g.classify({ 43.0, 37.0}), 0);
    ASSERT_EQ( g.classify({ 42.0, 38.0}), 0);
    ASSERT_EQ( g.classify({ 41.0, 39.0}), 0);
    ASSERT_EQ( g.classify({ 40.1, 39.9}), 0);

    // cell @ 4,3
    ASSERT_EQ( g.classify({ 50.0, 46}), 72);

    // cell @ 3,7
    ASSERT_EQ( g.classify({ 45, 62}), 67);
}

TEST( FixedGrid, StoreReadLoop) {
    FixedGrid64 g({{32,32}, {64,64}});

    g.fill(55);
    EXPECT_EQ( g.classify({50,46}), 55);

    EXPECT_TRUE( g.store({50, 46}, 88) );
    ASSERT_EQ( g.classify({50, 46}), 88);
}

TEST( FixedGrid, LoadMalformedSource){
    FixedGrid64 g;

    EXPECT_DOUBLE_EQ( g.precision(),  1.);
    EXPECT_DOUBLE_EQ( g.bounds().max().x(),  4.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(),  4.);
    EXPECT_DOUBLE_EQ( g.bounds().min().x(), -4.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(), -4.);

    // this is simply a malformed document.  It should not parse.
    std::string source(R"({"bounds": {"x": 100, "y": 100, "width": )");
    
    // this should fail. Gracefully.
    EXPECT_FALSE( g.load_json(source));

    // these tests should be *exactly* the same as before the 'load' call
    EXPECT_DOUBLE_EQ( g.precision(),  1.);
    EXPECT_DOUBLE_EQ( g.bounds().max().x(),  4.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(),  4.);
    EXPECT_DOUBLE_EQ( g.bounds().min().x(), -4.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(), -4.);
}

TEST( FixedGrid, LoadValidBoundsFromJSON){
    FixedGrid1k g;

    EXPECT_DOUBLE_EQ( g.bounds().max().x(),  16.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(),  16.);
    EXPECT_DOUBLE_EQ( g.bounds().min().x(), -16.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(), -16.);

    // construct a valid document, with correct fields, but missing required fields:
    std::string source(R"({"bounds": {"center": [100, 100], "width": 64}} )");
    
    EXPECT_TRUE( g.load_json(source) );

    // these tests should be *exactly* the same as before the 'load' call
    EXPECT_DOUBLE_EQ( g.bounds().max().x(), 132.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(), 132.);
    EXPECT_DOUBLE_EQ( g.bounds().min().x(),  68.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(),  68.);
    EXPECT_DOUBLE_EQ( g.precision(), 2 );
}

TEST( FixedGrid, LoadGridFromJSON) {
    FixedGrid64 g;

    std::string text(R"({
        "grid": [ 88, 88, 88, 88, 88, 88, 88, 88,
                  88, 88, 88,  0,  0, 88, 88, 88,
                  88, 88,  0,  0,  0,  0, 88, 88,
                  88,  0,  0,  0,  0,  0,  0, 88,
                  88, 88, 88, 88,  0,  0,  0, 88,
                  88, 88, 88, 88,  0,  0, 88, 88,
                  88, 88, 88, 88,  0, 88, 88, 88,
                  88, 88, 88, 88, 88, 88, 88, 88 ]})");

    ASSERT_TRUE( g.load_json(text) );

    // // DEBUG
    // cerr << g.to_string() << endl;

    ASSERT_EQ( g.get_cell(0,0), 88);
    ASSERT_EQ( g.get_cell(1,1), 88);
    ASSERT_EQ( g.get_cell(2,2), 88);
    ASSERT_EQ( g.get_cell(2,3), 88);
    ASSERT_EQ( g.get_cell(2,4),  0);

    ASSERT_EQ( g.get_cell(3,0), 88);
    ASSERT_EQ( g.get_cell(3,1), 88);
    ASSERT_EQ( g.get_cell(3,2), 88);
    ASSERT_EQ( g.get_cell(3,3), 88);
    ASSERT_EQ( g.get_cell(3,4),  0);
    ASSERT_EQ( g.get_cell(3,5),  0);
    ASSERT_EQ( g.get_cell(3,6),  0);
    ASSERT_EQ( g.get_cell(3,7), 88);

    ASSERT_EQ( g.get_cell(0,7), 88);
    ASSERT_EQ( g.get_cell(1,6), 88);
    ASSERT_EQ( g.get_cell(2,5),  0);
}

TEST( FixedGrid, FillSimplePolygon) {
    FixedGrid64 g(Bounds({0,0},{8,8}));
    EXPECT_DOUBLE_EQ( g.bounds().max().x(), 8.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(), 8.);
    EXPECT_DOUBLE_EQ( g.bounds().min().x(), 0.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(), 0.);
    EXPECT_DOUBLE_EQ( g.precision(), 1. );

    ASSERT_TRUE( g.fill(0x99) );

    constexpr double diamond_width = 6.;
    Polygon diamond = geometry::Polygon::make_diamond(diamond_width)
                                        .move(g.bounds().center());
    ASSERT_TRUE( g.fill(diamond, (uint8_t)0) );

    // // DEBUG
    // cerr << diamond.to_yaml() << endl;
    // cerr << g.to_string() << endl;

    ASSERT_EQ( g.get_cell(3, 7), 0x99);
    ASSERT_EQ( g.get_cell(3, 6), 0x99);
    ASSERT_EQ( g.get_cell(3, 5),    0);
    ASSERT_EQ( g.get_cell(3, 4),    0);
    ASSERT_EQ( g.get_cell(3, 3),    0);
    ASSERT_EQ( g.get_cell(3, 2),    0);
    ASSERT_EQ( g.get_cell(3, 1), 0x99);
    ASSERT_EQ( g.get_cell(3, 0), 0x99);

    ASSERT_EQ( g.get_cell( 0, 5), 0x99);
    ASSERT_EQ( g.get_cell( 1, 5), 0x99);
    ASSERT_EQ( g.get_cell( 2, 5), 0x99);
    ASSERT_EQ( g.get_cell( 3, 5),    0);
    ASSERT_EQ( g.get_cell( 4, 5),    0);
    ASSERT_EQ( g.get_cell( 5, 5),    0);
    ASSERT_EQ( g.get_cell( 6, 5), 0x99);
    ASSERT_EQ( g.get_cell( 7, 5), 0x99);
}

TEST( FixedGrid, FillHoledPolygon) {
    FixedGrid1k g;
    EXPECT_DOUBLE_EQ( g.bounds().max().x(),  16.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(),  16.);
    EXPECT_DOUBLE_EQ( g.bounds().min().x(), -16.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(), -16.);
    EXPECT_DOUBLE_EQ( g.precision(), 1.0);

    ASSERT_TRUE( g.fill(126) );

    const std::string source = R"({
        "bounds": { "southwest": [0,0],
                    "northeast": [32,32]},
        "allow": [[ [  1,  1],
                    [  1, 28],
                    [ 28, 28],
                    [ 28,  1]]],
        "block": [[ [  4,  4],
                    [  4, 24],
                    [  8, 24],
                    [  8,  4]],
                  [ [ 12, 12],
                    [ 12, 20],
                    [ 20, 20],
                    [ 20, 12]]]})";

    EXPECT_TRUE( g.load_json(source) );

    EXPECT_DOUBLE_EQ( g.bounds().max().x(), 32.);
    EXPECT_DOUBLE_EQ( g.bounds().max().y(), 32.);
    EXPECT_DOUBLE_EQ( g.bounds().min().x(),  0.);
    EXPECT_DOUBLE_EQ( g.bounds().min().y(),  0.);
    EXPECT_DOUBLE_EQ( g.precision(), 1. );

    // // DEBUG
    // cerr << g.to_string() << endl;

//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_precision(), 1.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_x(),         4.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_y(),         4.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_width(),     8.);
//     EXPECT_EQ( terrain.get_layout().get_dimension(), 8);
//     EXPECT_EQ( terrain.get_layout().get_size(),     64);

//     ASSERT_EQ( terrain.classify({ 0.2, 0.2}), 0x99);
//     ASSERT_EQ( terrain.classify({ 0.8, 0.8}), 0x99);
//     ASSERT_EQ( terrain.classify({ 1.1, 1.1}), 0);
//     ASSERT_EQ( terrain.classify({ 2.2, 2.2}), 0);
//     ASSERT_EQ( terrain.classify({ 3.2, 3.2}), 0);
//     ASSERT_EQ( terrain.classify({ 4.2, 4.2}), 0x99);
//     ASSERT_EQ( terrain.classify({ 4.2, 4.7}), 0x99);
//     ASSERT_EQ( terrain.classify({ 5.2, 5.2}), 0x99);
//     ASSERT_EQ( terrain.classify({ 5.2, 5.7}), 0x99);
//     ASSERT_EQ( terrain.classify({ 6.2, 6.2}), 0);
//     ASSERT_EQ( terrain.classify({ 6.7, 6.7}), 0);
//     ASSERT_EQ( terrain.classify({ 7.2, 7.2}), 0x99);
//     ASSERT_EQ( terrain.classify({ 7.7, 7.7}), 0x99);

//     // above-diagonal square
//     ASSERT_EQ( terrain.classify({ 2.2, 5.2}), 0x99);
//     ASSERT_EQ( terrain.classify({ 2.2, 5.7}), 0x99);
}

// TEST(Grid, LoadOffsetPolygon) {
//     Terrain<Grid> terrain;

//     // =====
//     const json source = {
//             {"layout",  {{"precision", 32},
//                          {"x", 232000},
//                          {"y", 811000},
//                          {"width", 1024}}},
//             {"allow", {{{231600, 810800},
//                         {231800, 810600},
//                         {232440, 810600},
//                         {232440, 810850},
//                         {232220, 811000},
//                         {232440, 811150},
//                         {232440, 811400},
//                         {232200, 811400},
//                         {231600, 810800}}}}};
//     std::istringstream stream(source.dump());

//     EXPECT_TRUE( terrain::io::load_from_json_stream(terrain, stream) );
//     // print error, if it is set:
//     ASSERT_TRUE( terrain.get_error().empty() ) << terrain.get_error();

//     // DEBUG
//     // terrain.debug();
//     // terrain.print();

//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_precision(),    32.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_x(),        232000.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_y(),        811000.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_width(),      1024.);
//     EXPECT_EQ( terrain.get_layout().get_dimension(),  32);
//     EXPECT_EQ( terrain.get_layout().get_size(),     1024);

//     ASSERT_EQ( terrain.classify({ 231500, 811500}), 0x99);
//     ASSERT_EQ( terrain.classify({ 231600, 811400}), 0x99);
//     ASSERT_EQ( terrain.classify({ 231700, 811300}), 0x99);
//     ASSERT_EQ( terrain.classify({ 231800, 811200}), 0x99);
//     ASSERT_EQ( terrain.classify({ 231900, 811100}), 0x99);
//     ASSERT_EQ( terrain.classify({ 232000, 811000}), 0);
//     ASSERT_EQ( terrain.classify({ 232100, 810900}), 0);
//     ASSERT_EQ( terrain.classify({ 232200, 810800}), 0);
//     ASSERT_EQ( terrain.classify({ 232300, 810700}), 0);
//     ASSERT_EQ( terrain.classify({ 232400, 810600}), 0);
//     ASSERT_EQ( terrain.classify({ 232500, 810500}), 0x99);
    
// }

// TEST(Grid, LoadSomervilleShapeFile) {
//     Terrain<Grid> terrain;

//     string shapefile("data/Somerville/CityLimits.shp");

//     const bool load_result = terrain::io::load_shape_from_file(terrain, shapefile);
//     // terrain.debug();
//     // terrain.print();

//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_precision(),    16.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_x(),        763251.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_y(),       2969340.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_width(),     32768.);
//     EXPECT_EQ( terrain.get_layout().get_dimension(),  2048);
//     EXPECT_EQ( terrain.get_layout().get_size(),    4194304);

//     // Because this manually tested, comment this block until needed:
//     const string filename("test.somerville.shapefile.png");
//     terrain::io::to_png(terrain, filename);

//     ASSERT_TRUE( load_result );
// }

// TEST(Grid, LoadMassachusettsShapeFile) {
//     Terrain<Grid> terrain;

//     string shapefile("data/massachusetts/navigation_area_100k.shp");

//     const bool load_result = terrain::io::load_shape_from_file(terrain, shapefile);
//     // terrain.debug();
//     // terrain.print();

//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_precision(),  64.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_x(),        305000.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_y(),        861400.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_width(),    262144.);
//     // EXPECT_EQ( terrain.get_layout().get_dimension(),  1024);
//     // EXPECT_EQ( terrain.get_layout().get_size(),    1048576);
//     EXPECT_EQ( terrain.get_layout().get_dimension(),  4096);
//     EXPECT_EQ( terrain.get_layout().get_size(),   16777216);

//     // Because this manually tested, comment this block until needed:
//     const string filename("shapefile.test.png");
//     terrain::io::to_png(terrain, filename);

//     ASSERT_TRUE( load_result );
// }

// TEST(Grid, SavePNG) {
//     Terrain<Grid> terrain;
//     const json source = generate_diamond(  64.,   // boundary_width
//                                             1.0);  // desired_precision);
//     std::istringstream stream(source.dump());
//     ASSERT_TRUE( terrain::io::load_from_json_stream(terrain, stream) );

//     // // DEBUG
//     // terrain.debug();

//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_precision(), 1.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_x(),        32.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_y(),        32.);
//     EXPECT_DOUBLE_EQ( terrain.get_layout().get_width(),    64.);
//     EXPECT_EQ( terrain.get_layout().get_dimension(), 64);
//     EXPECT_EQ( terrain.get_layout().get_size(),    4096);

//     // Because this manually tested, comment this block until needed:
//     const string filename("grid.test.png");
//     terrain::io::to_png(terrain, filename);
// }

}; // namespace chart::grid
