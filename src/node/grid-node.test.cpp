// GPL v3 (c) 2020

#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

// #include <nlohmann/json.hpp>

#include "grid_node.hpp"
#include "tile_node.hpp"

using std::cerr;
using std::endl;
using std::isnan;
using std::string;

// using Eigen::Vector2d;
// using nlohmann::json;

namespace chart::node {

// #include "node/tile_node.hpp"

typedef GridNode<4, TileNode<uint8_t, 4>, uint8_t> Grid4L1;

// typedef GridNode<32, TileNode<uint8_t, 32>, uint8_t> Grid32L1;

// typedef GridNode<16, Grid16L1, uint8_t> Grid16L2;
// typedef GridNode<32, Grid16L2, uint8_t> Grid32L2;

TEST(Grid, ConstructDefaultGrid4) {
    EXPECT_EQ(Grid4L1::size, 16);
    EXPECT_DOUBLE_EQ(Grid4L1::scale, 4.);
    EXPECT_DOUBLE_EQ(Grid4L1::width, 4.);

    Grid4L1 tile;

    EXPECT_EQ(tile.size, 16);
    // Not available as a member -- static or instance.
    // EXPECT_EQ(tile.dimension, 32);
    EXPECT_DOUBLE_EQ(tile.scale, 4.);
    EXPECT_DOUBLE_EQ(tile.width, 4.);

    // EXPECT_DOUBLE_EQ(tile.anchor.x(), 0.);
    // EXPECT_DOUBLE_EQ(tile.anchor.y(), 0.);
}

// TEST(Grid, ConstructDefault1M) {
//     EXPECT_EQ(Tile1M::size, 1048576);
//     EXPECT_DOUBLE_EQ(Tile1M::scale, 1.);
//     EXPECT_DOUBLE_EQ(Tile1M::width, 1024.);

//     Tile1M tile;

//     EXPECT_EQ(tile.size, 1048576);
//     EXPECT_DOUBLE_EQ(tile.scale, 1.);
//     EXPECT_DOUBLE_EQ(tile.width, 1024.);

//     EXPECT_DOUBLE_EQ(tile.anchor.x(), 0.);
//     EXPECT_DOUBLE_EQ(tile.anchor.y(), 0.);
// }

// TEST(Grid, ConstructAtLocation) {
//     const Vector2d anchor{754500, 2972000};

//     Tile1k tile(anchor);

//     EXPECT_EQ(tile.size, 1024);
//     EXPECT_DOUBLE_EQ(tile.scale, 1.);
//     EXPECT_DOUBLE_EQ(tile.width, 32.);

//     EXPECT_DOUBLE_EQ(tile.anchor.x(), anchor.x());
//     EXPECT_DOUBLE_EQ(tile.anchor.y(), anchor.y());
// }

// static const Vector2d default_anchor = {4.4, 4.6};
// static const std::vector<uint8_t> default_tile_contents = {
//     66, 67, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 68, 69, 65, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 70, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,
//     0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  0,  0,  99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 0,  0,  0,  0,  0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,
//     0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,
//     0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,
//     0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  99, 99, 99,
//     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 76, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 99, 99, 0,  0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99,
//     99, 99, 99, 71, 75, 74, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,
//     0,  0,  0,  0,  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 73, 72};

// TEST(Grid, ContainsPoints) {
//     // step 1:
//     Tile1k tile(default_anchor);
//     tile.fill(default_tile_contents);

//     // cerr << tile.to_string() << endl; // DEBUG

//     ASSERT_NEAR(tile.anchor.x(), 4.4, 1e-6);
//     ASSERT_NEAR(tile.anchor.y(), 4.6, 1e-6);

//     EXPECT_FALSE(tile.contains({0., 0.}));
//     EXPECT_FALSE(tile.contains({1., 1.}));
//     EXPECT_FALSE(tile.contains({2., 2.}));
//     EXPECT_FALSE(tile.contains({3., 3.}));
//     EXPECT_FALSE(tile.contains({4., 4.}));
//     EXPECT_FALSE(tile.contains({4.5, 4.5}));
//     // lower-left corner
//     EXPECT_TRUE(tile.contains({4.6, 4.6}));
//     EXPECT_TRUE(tile.contains({5., 5.}));
//     EXPECT_TRUE(tile.contains({10., 10.}));
//     EXPECT_TRUE(tile.contains({20., 20.}));
//     EXPECT_TRUE(tile.contains({30., 30.}));
//     EXPECT_TRUE(tile.contains({35., 35.}));
//     EXPECT_TRUE(tile.contains({36.2, 36.2}));
//     // upper-right corner
//     ASSERT_FALSE(tile.contains({36.4, 35.4}));
//     ASSERT_FALSE(tile.contains({36.5, 35.5}));
//     ASSERT_FALSE(tile.contains({37.6, 37.6}));
//     ASSERT_FALSE(tile.contains({37., 37}));
//     ASSERT_FALSE(tile.contains({40., 40.}));
// }

// TEST(Grid, Classify) {
//     Tile1k tile(default_anchor);
//     tile.fill(default_tile_contents);

//     // cerr << tile.to_string() << endl; // DEBUG

//     ASSERT_NEAR(tile.anchor.x(), 4.4, 1e-6);
//     ASSERT_NEAR(tile.anchor.y(), 4.6, 1e-6);

//     EXPECT_EQ(tile.classify({0., 0.}), 42);
//     EXPECT_EQ(tile.classify({1., 1.}), 42);
//     EXPECT_EQ(tile.classify({2., 2.}), 42);
//     EXPECT_EQ(tile.classify({3., 3.}), 42);
//     EXPECT_EQ(tile.classify({4., 4.}), 42);
//     EXPECT_EQ(tile.classify({4.5, 4.5}), 42);
//     // lower-left corner
//     EXPECT_EQ(tile.classify({4.6, 4.6}), 75);
//     EXPECT_EQ(tile.classify({5., 5.}), 75);
//     EXPECT_EQ(tile.classify({10., 10.}), 99);
//     EXPECT_EQ(tile.classify({15., 15.}), 99);
//     EXPECT_EQ(tile.classify({18., 18.}), 0);
//     EXPECT_EQ(tile.classify({20., 20.}), 0);
//     EXPECT_EQ(tile.classify({25., 25.}), 0);
//     EXPECT_EQ(tile.classify({22., 22.}), 0);
//     EXPECT_EQ(tile.classify({30., 30.}), 99);
//     EXPECT_EQ(tile.classify({35., 35.}), 99);
//     EXPECT_EQ(tile.classify({36., 36.}), 69);
//     EXPECT_EQ(tile.classify({36.3, 36.3}), 69);
//     // upper-right corner
//     EXPECT_EQ(tile.classify({36.4, 35.4}), 42);
//     EXPECT_EQ(tile.classify({36.5, 35.5}), 42);
//     EXPECT_EQ(tile.classify({37.6, 37.6}), 42);
//     EXPECT_EQ(tile.classify({37., 37.}), 42);
//     EXPECT_EQ(tile.classify({40., 40.}), 42);
// }

// TEST(Grid, FlatbufferRoundTrip) {
//     ASSERT_EQ(sizeof(uint8_t), sizeof(std::byte));
//     ASSERT_EQ(sizeof(uint8_t*), sizeof(std::byte*));

//     // step 1:
//     Tile1k write_tile(default_anchor);
//     write_tile.fill(default_tile_contents);

//     // // DEBUG
//     // cerr << write_tile->to_string() << endl;

//     { // quickly test the original tile
//         EXPECT_NEAR(write_tile.anchor.x(), 4.4, 1e-6);
//         EXPECT_NEAR(write_tile.anchor.y(), 4.6, 1e-6);

//         ASSERT_EQ(write_tile.classify({0., 0.}), 42);
//         ASSERT_EQ(write_tile.classify({10., 10.}), 99);
//         ASSERT_EQ(write_tile.classify({20., 20.}), 0);
//         ASSERT_EQ(write_tile.classify({30., 30.}), 99);
//         ASSERT_EQ(write_tile.classify({40., 40.}), 42);
//     }

//     // target first half: write to cache
//     auto buf = write_tile.to_flatbuffer();
//     EXPECT_NE(nullptr, buf);

//     // Perform intermediate tests on the buffer
//     auto tile_cache = GetTileCache(buf);
//     EXPECT_NEAR(tile_cache->x(), 4.4, 1e-6);
//     EXPECT_NEAR(tile_cache->y(), 4.6, 1e-6);

//     // // Not implemented.  Not sure how the builder knows how many total bytes
//     // are available, in this case. EXPECT_EQ( tile_cache->GetSize(), 1072);  //
//     // serialized size, including packing

//     // target second half: write to cache
//     const auto read_tile = Tile1k::build_from_flatbuffer(buf);
//     ASSERT_TRUE(read_tile);

//     EXPECT_NEAR(read_tile->anchor.x(), 4.4, 1e-6);
//     EXPECT_NEAR(read_tile->anchor.y(), 4.6, 1e-6);

//     // // DEBUG
//     // cerr << read_tile->to_string() << endl;

//     { // quickly test the loaded tile
//         EXPECT_EQ(read_tile->classify({0., 0.}), 42);
//         EXPECT_EQ(read_tile->classify({10., 10.}), 99);
//         EXPECT_EQ(read_tile->classify({20., 20.}), 0);
//         EXPECT_EQ(read_tile->classify({30., 30.}), 99);
//         EXPECT_EQ(read_tile->classify({40., 40.}), 42);
//     }
// }

// TEST(Grid, JSONRoundTrip) {
//     ASSERT_EQ(sizeof(uint8_t), sizeof(std::byte));
//     ASSERT_EQ(sizeof(uint8_t*), sizeof(std::byte*));

//     // step 1:
//     Tile1k write_tile(default_anchor);
//     write_tile.fill(default_tile_contents);

//     // // DEBUG
//     // cerr << write_tile->to_string() << endl;

//     { // quickly test the original tile
//         EXPECT_NEAR(write_tile.anchor.x(), 4.4, 1e-6);
//         EXPECT_NEAR(write_tile.anchor.y(), 4.6, 1e-6);

//         ASSERT_EQ(write_tile.classify({0., 0.}), 42);
//         ASSERT_EQ(write_tile.classify({10., 10.}), 99);
//         ASSERT_EQ(write_tile.classify({20., 20.}), 0);
//         ASSERT_EQ(write_tile.classify({30., 30.}), 99);
//         ASSERT_EQ(write_tile.classify({40., 40.}), 42);
//     }

//     // target first half: write to cache
//     const std::string& buffer = write_tile.to_json();

//     // target second half: write to cache
//     const auto read_tile = Tile1k::build_from_json(buffer);
//     ASSERT_TRUE(read_tile);

//     EXPECT_NEAR(read_tile->anchor.x(), 4.4, 1e-6);
//     EXPECT_NEAR(read_tile->anchor.y(), 4.6, 1e-6);

//     // // DEBUG
//     // cerr << read_tile->to_string() << endl;

//     { // quickly test the loaded tile
//         EXPECT_EQ(read_tile->classify({0., 0.}), 42);
//         EXPECT_EQ(read_tile->classify({10., 10.}), 99);
//         EXPECT_EQ(read_tile->classify({20., 20.}), 0);
//         EXPECT_EQ(read_tile->classify({30., 30.}), 99);
//         EXPECT_EQ(read_tile->classify({40., 40.}), 42);
//     }
// }

// TEST(Grid, LoadPolygonFromVector) {
//     Tile1k tile;
//     EXPECT_DOUBLE_EQ(tile.anchor.x(), 0.);
//     EXPECT_DOUBLE_EQ(tile.anchor.y(), 0.);

//     // reset grid
//     tile.fill(88); // == 'X'

//     Polygon shape = {{2, 2}, {12, 12}, {24, 6}, {30, 16}, {20, 30}, {5, 24}};
//     tile.fill(shape, 46); // == '.'

//     // // DEBUG
//     // cerr << tile.to_string() << endl;

//     EXPECT_EQ(tile.get_cell(0, 0), 88);
//     EXPECT_EQ(tile.get_cell(1, 1), 88);
//     EXPECT_EQ(tile.get_cell(2, 2), 46);
//     EXPECT_EQ(tile.get_cell(3, 3), 46);
//     EXPECT_EQ(tile.get_cell(4, 4), 46);

//     EXPECT_EQ(tile.get_cell(12, 10), 88);
//     EXPECT_EQ(tile.get_cell(12, 11), 88);
//     EXPECT_EQ(tile.get_cell(12, 12), 46);
//     EXPECT_EQ(tile.get_cell(12, 13), 46);
//     EXPECT_EQ(tile.get_cell(12, 14), 46);

//     // EXPECT_EQ( tile.get_cell( 9, 9), 88);
//     // EXPECT_EQ( tile.get_cell(15,15), 88);
// }

// TEST(Grid, LoadShapefile) {
//     // fyi, this is not a _great_ teste, because a single tile is small,
//     // compared to the whole map...
//     const Vector2d anchor{771635, 2961225};
//     Tile1k tile(anchor);

//     const string shapefile("data/Somerville/CityLimits.shp");

//     // vvvv test target vvvv
//     ASSERT_TRUE(tile.load_from_shapefile(shapefile));
//     // ^^^^ test target ^^^^

//     // // DEBUG
//     // cerr << tile.to_string() << endl;

//     { // quickly test the loaded tile
//         EXPECT_EQ(tile.get_cell(0, 0), 153);
//         EXPECT_EQ(tile.get_cell(1, 1), 153);
//         EXPECT_EQ(tile.get_cell(2, 2), 153);
//         EXPECT_EQ(tile.get_cell(3, 3), 153);

//         EXPECT_EQ(tile.get_cell(4, 4), 153);
//         EXPECT_EQ(tile.get_cell(4, 5), 153);
//         EXPECT_EQ(tile.get_cell(4, 6), 0);
//         EXPECT_EQ(tile.get_cell(4, 7), 0);
//         EXPECT_EQ(tile.get_cell(4, 8), 0);
//         EXPECT_EQ(tile.get_cell(4, 9), 0);
//         EXPECT_EQ(tile.get_cell(4, 10), 0);

//         EXPECT_EQ(tile.get_cell(20, 15), 0);
//         EXPECT_EQ(tile.get_cell(21, 15), 0);
//         EXPECT_EQ(tile.get_cell(22, 15), 153);
//         EXPECT_EQ(tile.get_cell(23, 15), 153);
//         EXPECT_EQ(tile.get_cell(24, 15), 153);
//     }
// }

// TEST(Grid, ToPNG) {
//     // step 1:
//     Tile1k tile(default_anchor);
//     tile.fill(default_tile_contents);

//     // DEBUG
//     cerr << tile->to_string() << endl; // DEBUG

//     const std::vector<std::byte> data = tile->to_raster();
//     const string outpath("test.tile.png");
//     ASSERT_TRUE( chart::io::to_png( data, outpath ) );
// }

}; // namespace chart::node
