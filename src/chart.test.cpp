// GPL v3 (c) 2020, Daniel Williams 

#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Geometry>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "geometry/layout.hpp"
#include "geometry/polygon.hpp"
#include "io/readers.hpp"
#include "io/writers.hpp"
#include "quadtree/tree.hpp"
#include "terrain.hpp"

using std::cerr;
using std::endl;
using std::string;
using std::vector;

using Eigen::Vector2d;

using nlohmann::json;

using yggdrasil::geometry::Layout;

namespace yggdrasil::quadtree {

static const Layout default_layout; // i.e. create with default constructor

void assert_layouts_match(const Layout& first, const Layout& second) {
    EXPECT_DOUBLE_EQ(first.get_precision(), second.get_precision());
    EXPECT_DOUBLE_EQ(first.get_x(), second.get_x());
    EXPECT_DOUBLE_EQ(first.get_y(), second.get_y());
    EXPECT_DOUBLE_EQ(first.get_width(), second.get_width());
    EXPECT_EQ(first.get_dimension(), second.get_dimension());
    EXPECT_EQ(first.get_size(), second.get_size());
}

TEST(QuadTreeTest, CalculateMemoryUsage) {
    Tree tree;
    Terrain terrain(tree);

    assert_layouts_match(terrain.get_layout(), default_layout);

    EXPECT_EQ(sizeof(Terrain<Tree>), 32);
    EXPECT_EQ(sizeof(Layout), 64);
    EXPECT_EQ(sizeof(Tree), 72); // composed of: Layout, root-pointer
    EXPECT_EQ(sizeof(Vector2d), 16);
    EXPECT_EQ(sizeof(Node), 40);
}

TEST(QuadTreeTest, ConstructDefault) {
    Tree tree;
    Terrain terrain(tree);

    assert_layouts_match(terrain.get_layout(), default_layout);

    EXPECT_TRUE(tree.root);
    EXPECT_TRUE(tree.root->is_leaf());
}

TEST(QuadTreeTest, ConstructByCenterAndSize) {
    Tree tree({1., 1, 1, 256});
    Terrain terrain(tree);

    EXPECT_DOUBLE_EQ(terrain.get_layout().get_precision(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_x(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_y(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_width(), 256.);
    EXPECT_EQ(terrain.get_layout().get_dimension(), 256);
    EXPECT_EQ(terrain.get_layout().get_size(), 65536);

    // functional tests:
    // (1) in bounds
    ASSERT_TRUE(tree.contains({0, 0}));
    // (2) out of bounds in x
    ASSERT_FALSE(tree.contains({200, 0}));
    // (3) out of bounds in y
    ASSERT_FALSE(tree.contains({0, 200}));
    // (4) out of bounds in both x AND y
    ASSERT_FALSE(tree.contains({130, 130}));
}

TEST(QuadTreeTest, ConstructAndSetBounds) {
    Tree tree({1., 5, 3, 17});
    Terrain terrain(tree);

    // pre-conditions
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_precision(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_x(), 5.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_y(), 3.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_width(), 32.);
    EXPECT_EQ(terrain.get_layout().get_dimension(), 32);
    EXPECT_EQ(terrain.get_layout().get_size(), 1024);

    const Layout new_layout = {32, 1., 1, 256};

    // test target
    tree.reset(new_layout);
    // test target

    // post-conditions
    assert_layouts_match(terrain.get_layout(), new_layout);
}

TEST(QuadTreeTest, LoadMalformedSource) {
    Tree tree;
    Terrain terrain(tree);

    assert_layouts_match(terrain.get_layout(), default_layout);

    // this is simply a malformed document.  It should not parse.
    std::istringstream source(R"({"bounds": {"x": 100, "y": 100, "width": )");

    // this should fail. Gracefully.
    EXPECT_FALSE(yggdrasil::io::load_from_json_stream(terrain, source));

    // these tests should be *exactly* the same as before the 'load' call
    assert_layouts_match(terrain.get_layout(), default_layout);
}

TEST(QuadTreeTest, LoadBoundsOnly) {
    Tree tree;
    Terrain terrain(tree);

    assert_layouts_match(terrain.get_layout(), default_layout);

    // construct a valid document, with correct fields, but simply .. missing
    // things:
    std::stringstream stream(
        R"({"bounds": {"x": 100, "y": 100, "width": 64}} )");

    // this should fail. (gracefully)
    EXPECT_FALSE(yggdrasil::io::load_from_json_stream(terrain, stream));

    // these tests should be *exactly* the same as before the 'load' call
    assert_layouts_match(terrain.get_layout(), default_layout);
}

TEST(QuadTreeTest, LoadValidTree) {
    Tree tree;
    Terrain terrain(tree);
    assert_layouts_match(terrain.get_layout(), default_layout);

    const json source = {
        {"NE", {{"NE", 0.0}, {"NW", 0.0}, {"SE", 0.0}, {"SW", 0.0}}},
        {"NW", 0.0},
        {"SE", 0.0},
        {"SW", 0.0}};

    tree.load_tree(source);

    // // debug
    // tree.debug_tree();

    // test shape
    ASSERT_FALSE(tree.root->is_leaf());
    ASSERT_FALSE(tree.root->get_northeast()->is_leaf());
    ASSERT_TRUE(tree.root->get_southwest()->is_leaf());
}

TEST(QuadTreeTest, CalculateFullLoading) {
    EXPECT_EQ(Tree::calculate_complete_tree(0), 1);
    EXPECT_EQ(Tree::calculate_complete_tree(1), 5);
    EXPECT_EQ(Tree::calculate_complete_tree(2), 21);
    ASSERT_EQ(Tree::calculate_complete_tree(3), 85);
    ASSERT_EQ(Tree::calculate_complete_tree(4), 341);
    ASSERT_EQ(Tree::calculate_complete_tree(5), 1365);
}

TEST(QuadTreeTest, CalculateLoadFactor) {
    Tree tree;
    Terrain terrain(tree);

    assert_layouts_match(terrain.get_layout(), default_layout);

    const json source = {
        {"NE",
         {{"NE", 0.0},
          {"NW", 0.0},
          {"SE", {{"NE", 0.0}, {"NW", 0.0}, {"SE", 0.0}, {"SW", 0.0}}},
          {"SW", 0.0}}},
        {"NW", 0.0},
        {"SE", 0.0},
        {"SW", 0.0}};

    tree.load_tree(source);

    // // debug
    // tree.debug_tree();

    // test shape
    ASSERT_FALSE(tree.root->is_leaf());
    ASSERT_FALSE(tree.root->get_northeast()->is_leaf());
    ASSERT_TRUE(tree.root->get_northeast()
                    ->get_southeast()
                    ->get_southeast()
                    ->is_leaf());
    ASSERT_TRUE(tree.root->get_southwest()->is_leaf());

    ASSERT_EQ(tree.get_height(), 3);
    ASSERT_EQ(tree.size(), 13);
    ASSERT_NEAR(tree.get_load_factor(), 0.03812, 1e-4);
}

TEST(QuadTreeTest, LoadGridFromJSON) {
    Tree tree;
    Terrain terrain(tree);

    assert_layouts_match(terrain.get_layout(), default_layout);

    std::istringstream stream(R"(
        {"layout": {"precision": 32.0, "x": 1, "y": 1, "width": 256},
         "grid":[[88, 88, 88, 88,  0, 88, 88, 88],
                 [88, 88, 88,  0,  0,  0, 88, 88],
                 [88, 88,  0,  0,  0,  0,  0, 88],
                 [88,  0,  0,  0,  0,  0,  0,  0],
                 [ 0,  0,  0,  0, 88, 88, 88, 88],
                 [88,  0,  0,  0, 88, 88, 88, 88],
                 [88, 88,  0,  0, 88, 88, 88, 88],
                 [88, 88, 88,  0, 88, 88, 88, 88]]} )");

    // test target
    ASSERT_TRUE(yggdrasil::io::load_from_json_stream(terrain, stream));
    // test target

    // // DEBUG
    // terrain.debug();

    EXPECT_DOUBLE_EQ(terrain.get_layout().get_precision(), 32.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_x(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_y(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_width(), 256.);
    EXPECT_EQ(terrain.get_layout().get_dimension(), 8);
    EXPECT_EQ(terrain.get_layout().get_size(), 64);

    { // test tree shape
        EXPECT_EQ(tree.get_height(), 3);
        EXPECT_EQ(tree.size(), 41);
        EXPECT_FALSE(tree.root->is_leaf());

        // spot check #1: RT-NE-SW-quadrant
        const auto* r_ne_sw = tree.root->get_northeast()->get_southwest();
        ASSERT_TRUE(r_ne_sw->is_leaf());
        ASSERT_DOUBLE_EQ(r_ne_sw->get_value(), 0);

        // spot check #2: RT-NW-NE-NW quadrant
        const auto* r_ne_nw =
            tree.root->get_northwest()->get_northeast()->get_northwest();
        ASSERT_TRUE(r_ne_nw->is_leaf());
        ASSERT_DOUBLE_EQ(r_ne_nw->get_value(), 88);
    }

    const Tree& frozen = tree;
    EXPECT_EQ(frozen.classify({16, 16}), 0);
    EXPECT_EQ(frozen.classify({48, 48}), 0);
    EXPECT_EQ(frozen.classify({72, 72}), 88);
    EXPECT_EQ(frozen.classify({104, 104}), 88);

    // [-127, -95, -63, -31, 1, 33, 65, 97, 129]
    EXPECT_EQ(frozen.classify({-70, 130}), 88);
    EXPECT_EQ(frozen.classify({-70, 129}), 88);
    EXPECT_EQ(frozen.classify({-70, 97}), 88);
    EXPECT_EQ(frozen.classify({-70, 65}), 88);
    EXPECT_EQ(frozen.classify({-70, 33}), 0);
    EXPECT_EQ(frozen.classify({-70, 1}), 0);
    EXPECT_EQ(frozen.classify({-70, -31}), 0);
    EXPECT_EQ(frozen.classify({-70, -63}), 88);
    EXPECT_EQ(frozen.classify({-70, -95}), 88);
    EXPECT_EQ(frozen.classify({-70, -127}), 88);
    EXPECT_EQ(frozen.classify({-70, -130}), 88);

    EXPECT_EQ(frozen.classify({15, 130}), 0);
    EXPECT_EQ(frozen.classify({15, 129}), 0);
    EXPECT_EQ(frozen.classify({15, 97}), 0);
    EXPECT_EQ(frozen.classify({15, 65}), 0);
    EXPECT_EQ(frozen.classify({15, 33}), 0);
    EXPECT_EQ(frozen.classify({15, 1}), 88);
    EXPECT_EQ(frozen.classify({15, -31}), 88);
    EXPECT_EQ(frozen.classify({15, -63}), 88);
    EXPECT_EQ(frozen.classify({15, -95}), 88);
    EXPECT_EQ(frozen.classify({15, -127}), 88);
    EXPECT_EQ(frozen.classify({15, -130}), 88);
}

TEST(QuadTreeTest, LoadPolygonFromJSON) {
    quadtree::Tree tree;
    Terrain terrain(tree);

    constexpr double desired_width = 16.; // overall boundary
    constexpr double desired_precision = 1.0;
    // =====
    json source = generate_diamond(desired_width, desired_precision);

    std::istringstream stream(source.dump());

    // // DEBUG
    // cerr << "======\n" << source.dump(4) << "\n======\n" << endl;

    ASSERT_TRUE(yggdrasil::io::load_from_json_stream(terrain, stream));

    // // DEBUG
    // terrain.debug();

    EXPECT_DOUBLE_EQ(terrain.get_layout().get_precision(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_x(), desired_width / 2);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_y(), desired_width / 2);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_width(), desired_width);
    EXPECT_EQ(terrain.get_layout().get_dimension(), 16);
    EXPECT_EQ(terrain.get_layout().get_size(), 256);

    ASSERT_EQ(tree.classify({4.5, 15.5}), 0x99);
    ASSERT_EQ(tree.classify({4.5, 14.5}), 0x99);
    ASSERT_EQ(tree.classify({4.5, 13.5}), 0x99);
    ASSERT_EQ(tree.classify({4.5, 12.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 11.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 10.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 9.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 8.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 7.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 6.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 4.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 3.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 2.5}), 0x99);
    ASSERT_EQ(tree.classify({4.5, 1.5}), 0x99);
    ASSERT_EQ(tree.classify({4.5, 0.5}), 0x99);

    ASSERT_EQ(tree.classify({0.5, 5.5}), 0x99);
    ASSERT_EQ(tree.classify({1.5, 5.5}), 0x99);
    ASSERT_EQ(tree.classify({2.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({3.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({4.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({5.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({6.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({7.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({8.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({9.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({10.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({11.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({12.5, 5.5}), 0);
    ASSERT_EQ(tree.classify({13.5, 5.5}), 0x99);
    ASSERT_EQ(tree.classify({14.5, 5.5}), 0x99);
    ASSERT_EQ(tree.classify({15.5, 5.5}), 0x99);
}

TEST(QuadTreeTest, SearchExplicitTree) {
    Tree tree({50, 0, 0, 100});
    Terrain terrain(tree);
    tree.root->split(50, 100);

    ASSERT_FALSE(tree.root->is_leaf());

    EXPECT_DOUBLE_EQ(terrain.get_layout().get_precision(), 32.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_x(), 0.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_y(), 0.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_width(), 128);
    EXPECT_EQ(terrain.get_layout().get_dimension(), 4);
    EXPECT_EQ(terrain.get_layout().get_size(), 16);

    cell_value_t false_value = 5;
    cell_value_t true_value = 14;

    // Set Quadrant I:
    tree.root->get_northeast()->set_value(true_value);
    // Set Quadrdant II:
    tree.root->get_northwest()->set_value(false_value);
    // Set Quadrant III:
    tree.root->get_southwest()->set_value(true_value);
    // Set Quadrant IV:
    tree.root->get_southeast()->set_value(false_value);

    // // DEBUG
    // tree.debug();

    // .... Out Of Bounds: snaps to nearest node...
    ASSERT_EQ(tree.classify({110, 110}), true_value);

    // functional tests:
    // .... Quadrant I:
    EXPECT_EQ(tree.classify({25, 25}), true_value);
    // .... Quadrant II:
    EXPECT_EQ(tree.classify({-25, 25}), false_value);
    // .... Quadrant III:
    EXPECT_EQ(tree.classify({-25, -25}), true_value);
    // .... Quadrant IV:
    EXPECT_EQ(tree.classify({25, -25}), false_value);
}

TEST(QuadTreeTest, SampleTree) {
    quadtree::Tree tree;
    Terrain terrain(tree);

    std::istringstream stream(R"(
        {"layout": {"precision": 1, "x": 2, "y": 2, "width": 4},
         "grid":[[  1,  2,  3,  4],
                 [  5,  6,  7,  8],
                 [  9, 10, 11, 12],
                 [ 13, 14, 15, 16]]} )");

    EXPECT_TRUE(yggdrasil::io::load_from_json_stream(terrain, stream));

    // // DEBUG
    // tree.debug_tree(true);
    // terrain.debug();

    // Out-Of-Bounds call -- returns nearest node.
    const Sample s0 = tree.sample({5, 5});
    ASSERT_TRUE(Vector2d(3.5, 3.5) == s0.at);
    ASSERT_EQ(s0.is, 4);

    const Sample s1 = tree.sample({0, 0});
    ASSERT_TRUE(Vector2d(0.5, 0.5) == s1.at);
    ASSERT_EQ(s1.is, 13);

    const Sample s2 = tree.sample({0.9, 0.9});
    ASSERT_TRUE(Vector2d(0.5, 0.5) == s2.at);
    ASSERT_EQ(s2.is, 13);

    const Sample s3 = tree.sample({2.4, 2.7});
    ASSERT_TRUE(Vector2d(2.5, 2.5) == s3.at);
    ASSERT_EQ(s3.is, 7);

    const Sample s4 = tree.sample({1.7, 3.3});
    ASSERT_TRUE(Vector2d(1.5, 3.5) == s4.at);
    ASSERT_EQ(s4.is, 2);
}

// TEST( QuadTreeTest, InterpolateTree){
//     Tree tree({{1,1}, 64}, 1.0);
//     Terrain terrain(tree);
//     tree.root->split();

//     // Set Quadrant I:
//     tree.root->get_northeast()->set_value(0);
//     // Set Quadrdant II:
//     tree.root->get_northwest()->set_value(50);
//     // Set Quadrant III:
//     tree.root->get_southwest()->set_value(100);
//     // Set Quadrant IV:
//     tree.root->get_southeast()->set_value(50);

//     vector<TestPoint> test_cases;
//     test_cases.emplace_back( -35,    4, cell_default_value);  // Start out of
//     bounds test_cases.emplace_back( -33,    4, cell_default_value);
//     test_cases.emplace_back( -32,    4, cell_default_value);
//     test_cases.emplace_back( -31,    4,   70);  // border of tree
//     test_cases.emplace_back( -30.9,  4,   70);
//     test_cases.emplace_back( -30,    4,   70);
//     test_cases.emplace_back( -20,    4,   70);
//     test_cases.emplace_back( -17,    4,   70);
//     test_cases.emplace_back( -16,    4,   70);
//     test_cases.emplace_back( -15.1,  4,   70);
//     test_cases.emplace_back( -15.0,  4,   70);  // breakpoint - center of
//     outer cell test_cases.emplace_back( -14.9,  4,   70);
//     test_cases.emplace_back( -10,    4,   62);
//     test_cases.emplace_back( - 5,    4,   54);
//     test_cases.emplace_back(   0,    4,   47);
//     test_cases.emplace_back(   1,    4,   45);  // midpoint
//     test_cases.emplace_back(   2,    4,   43);
//     test_cases.emplace_back(  10,    4,   31);
//     test_cases.emplace_back(  14,    4,   25);
//     test_cases.emplace_back(  15,    4,   23);
//     test_cases.emplace_back(  16,    4,   22);  // breakpoint - center of
//     outer cell test_cases.emplace_back(  17,    4,   20);
//     test_cases.emplace_back(  20,    4,   20);
//     test_cases.emplace_back(  30,    4,   20);
//     test_cases.emplace_back(  31,    4,   20);
//     test_cases.emplace_back(  32,    4,   20);
//     test_cases.emplace_back(  33,    4,   20);  // border of tree
//     test_cases.emplace_back(  34,    4, cell_default_value);
//     test_cases.emplace_back(  35,    4, cell_default_value);

//     // =====================================
//     for( const TestPoint& expect : test_cases){
//         const auto& actual_value = tree.interp({expect.x, expect.y});

//         std::ostringstream buf;
//         buf << "@@  x=" << expect.x << "  y=" << expect.y << "  v=" <<
//         expect.value << endl;

//         ASSERT_EQ(actual_value, expect.value) << buf.str();
//     }
// }

TEST(QuadTreeTest, SavePNG) {
    Terrain<Tree> terrain;
    const json source = generate_diamond(16.,  // boundary_width
                                         0.4); // desired_precision);

    std::istringstream stream(source.dump());

    ASSERT_TRUE(yggdrasil::io::load_from_json_stream(terrain, stream));

    // // DEBUG
    // terrain.debug();

    EXPECT_DOUBLE_EQ(terrain.get_layout().get_precision(), 1.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_x(), 8.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_y(), 8.);
    EXPECT_DOUBLE_EQ(terrain.get_layout().get_width(), 16);
    EXPECT_EQ(terrain.get_layout().get_dimension(), 16);
    EXPECT_EQ(terrain.get_layout().get_size(), 256);

    // // Because this manually tested, comment this block until needed:
    // const string filename("tree.test.png");
    // terrain.to_png(filename);
}

TEST(QuadTreeTest, WriteLoadCycle) {
    Tree source_tree({32, 11, 11, 128});
    Terrain source_terrain(source_tree);

    EXPECT_DOUBLE_EQ(source_terrain.get_layout().get_precision(), 32.);
    EXPECT_DOUBLE_EQ(source_terrain.get_layout().get_x(), 11.);
    EXPECT_DOUBLE_EQ(source_terrain.get_layout().get_y(), 11.);
    EXPECT_DOUBLE_EQ(source_terrain.get_layout().get_width(), 128);
    EXPECT_EQ(source_terrain.get_layout().get_dimension(), 4);
    EXPECT_EQ(source_terrain.get_layout().get_size(), 16);

    source_tree.root->split(32, 128);

    // Set interesting values
    source_tree.root->get_northeast()->get_northeast()->set_value(21);
    source_tree.root->get_northeast()->get_northwest()->set_value(22);
    source_tree.root->get_northeast()->get_southeast()->set_value(23);
    source_tree.root->get_northeast()->get_southwest()->set_value(24);

    source_tree.root->get_northwest()->get_northeast()->set_value(11);
    source_tree.root->get_northwest()->get_northwest()->set_value(11);
    source_tree.root->get_northwest()->get_southeast()->set_value(11);
    source_tree.root->get_northwest()->get_southwest()->set_value(11);

    source_tree.root->get_southwest()->get_northeast()->set_value(31);
    source_tree.root->get_southwest()->get_northwest()->set_value(32);
    source_tree.root->get_southwest()->get_southeast()->set_value(33);
    source_tree.root->get_southwest()->get_southwest()->set_value(34);

    source_tree.root->get_southeast()->get_northeast()->set_value(55);
    source_tree.root->get_southeast()->get_northwest()->set_value(55);
    source_tree.root->get_southeast()->get_southeast()->set_value(55);
    source_tree.root->get_southeast()->get_southwest()->set_value(55);

    // // DEBUG
    // source_tree.debug_tree();

    source_tree.root->prune();

    // // DEBUG
    // source_tree.debug_tree();

    // write tree #1 to the serialization buffer
    std::stringstream buffer;
    ASSERT_TRUE(yggdrasil::io::to_json(source_terrain, buffer));

    // // DEBUG
    // cerr << buffer.str() << endl;

    // load contents into another tree
    Tree load_tree;
    Terrain load_terrain(load_tree);

    buffer.seekg(0);
    ASSERT_TRUE(yggdrasil::io::load_from_json_stream(load_terrain, buffer));

    // // DEBUG
    // load_tree.debug_tree();

    // test contents of test_tree
    { // test layout:
        EXPECT_DOUBLE_EQ(load_terrain.get_layout().get_precision(), 32.);
        EXPECT_DOUBLE_EQ(load_terrain.get_layout().get_x(), 11.);
        EXPECT_DOUBLE_EQ(load_terrain.get_layout().get_y(), 11.);
        EXPECT_DOUBLE_EQ(load_terrain.get_layout().get_width(), 128);
        EXPECT_EQ(load_terrain.get_layout().get_dimension(), 4);
        EXPECT_EQ(load_terrain.get_layout().get_size(), 16);
    }
    { // test tree shape
        auto& root = load_tree.root;
        ASSERT_FALSE(root->is_leaf());
        {
            auto* ne_quad = root->get_northeast();
            ASSERT_FALSE(ne_quad->is_leaf());
            ASSERT_TRUE(ne_quad->get_northeast()->is_leaf());
            ASSERT_TRUE(ne_quad->get_northwest()->is_leaf());
            ASSERT_TRUE(ne_quad->get_southwest()->is_leaf());
            ASSERT_TRUE(ne_quad->get_southeast()->is_leaf());
        }
        ASSERT_TRUE(root->get_northwest()->is_leaf());
        ASSERT_TRUE(root->get_southeast()->is_leaf());
        {
            auto* sw_quad = root->get_northeast();
            ASSERT_FALSE(sw_quad->is_leaf());
            ASSERT_TRUE(sw_quad->get_northeast()->is_leaf());
            ASSERT_TRUE(sw_quad->get_northwest()->is_leaf());
            ASSERT_TRUE(sw_quad->get_southwest()->is_leaf());
            ASSERT_TRUE(sw_quad->get_southeast()->is_leaf());
        }
    }
}

} // namespace yggdrasil::quadtree
