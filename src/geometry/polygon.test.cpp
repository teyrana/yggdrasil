// GPL v3 (c) 2020, Daniel Williams 

#include <iostream>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

#include "polygon.hpp"

// #include "geometry/layout.hpp"

using std::cerr;
using std::endl;

using Eigen::Vector2d;

using chart::geometry::Polygon;
// using chart::geometry::Layout;

namespace chart::geometry {

TEST(Polygon, DefaultConfiguration) {
    const Polygon shape;

    const auto& points = shape.points;
    ASSERT_TRUE(points[0].isApprox(Vector2d(0, 0)));
    ASSERT_TRUE(points[1].isApprox(Vector2d(1, 0)));
    ASSERT_TRUE(points[2].isApprox(Vector2d(1, 1)));
    ASSERT_TRUE(points[3].isApprox(Vector2d(0, 1)));
}

TEST(Polygon, LoadList_5Point) {
    // Note:  this polygen is configured as CW:
    //    it will be enclosed, and reversed, internally
    Polygon shape({{3, 4}, {5, 11}, {12, 8}, {9, 5}, {5, 6}});

    // DEBUG
    // shape.write_yaml(std::cerr, "    ");

    ASSERT_EQ(shape[0], Vector2d(3, 4));
    ASSERT_EQ(shape[1], Vector2d(5, 6));
    ASSERT_EQ(shape[2], Vector2d(9, 5));
    ASSERT_EQ(shape[3], Vector2d(12, 8));
    ASSERT_EQ(shape[4], Vector2d(5, 11));
    ASSERT_EQ(shape[5], Vector2d(3, 4));
}

TEST(Polygon, LoadList_Diamond) {
    Polygon shape({{1, 0}, {0, 1}, {-1, 0}, {0, -1}});

    // // DEBUG
    // shape.write_yaml(std::cerr, "    ");

    ASSERT_EQ(shape[0], Vector2d(1, 0));
    ASSERT_EQ(shape[1], Vector2d(0, 1));
    ASSERT_EQ(shape[2], Vector2d(-1, 0));
    ASSERT_EQ(shape[3], Vector2d(0, -1));
    ASSERT_EQ(shape[4], Vector2d(1, 0));
}

TEST(Polygon, GenerateDiamond) {
    const auto shape = Polygon::make_diamond(2.);

    // DEBUG
    // cerr << shape.to_yaml("    ") << endl;

    ASSERT_EQ(shape.size(), 5);
    ASSERT_EQ(shape[0], Vector2d(1, 0));
    ASSERT_EQ(shape[1], Vector2d(0, 1));
    ASSERT_EQ(shape[2], Vector2d(-1, 0));
    ASSERT_EQ(shape[3], Vector2d(0, -1));
    ASSERT_EQ(shape[4], Vector2d(1, 0));
}

TEST(Polygon, Move) {
    auto shape = Polygon::make_diamond(4.);
    ASSERT_EQ(shape.size(), 5);

    shape.move({3, 3});

    EXPECT_DOUBLE_EQ(shape[0].x(), 5);
    EXPECT_DOUBLE_EQ(shape[0].y(), 3);

    EXPECT_DOUBLE_EQ(shape[1].x(), 3);
    EXPECT_DOUBLE_EQ(shape[1].y(), 5);

    EXPECT_DOUBLE_EQ(shape[2].x(), 1);
    EXPECT_DOUBLE_EQ(shape[2].y(), 3);

    EXPECT_DOUBLE_EQ(shape[3].x(), 3);
    EXPECT_DOUBLE_EQ(shape[3].y(), 1);

    EXPECT_DOUBLE_EQ(shape[4].x(), 5);
    EXPECT_DOUBLE_EQ(shape[4].y(), 3);
}

TEST(Polygon, MakeBounds) {
    Polygon shape({{5, 6}, {9, 5}, {12, 8}, {5, 11}, {3, 4}, {5, 6}});

    const auto bounds = shape.get_bounds();

    // stored
    EXPECT_DOUBLE_EQ(bounds.min().x(), 3.);
    EXPECT_DOUBLE_EQ(bounds.min().y(), 4.);
    EXPECT_DOUBLE_EQ(bounds.max().x(), 12.);
    EXPECT_DOUBLE_EQ(bounds.max().y(), 11.);
    // derived
    EXPECT_DOUBLE_EQ(bounds.size().x(), 9.);
    EXPECT_DOUBLE_EQ(bounds.size().y(), 7.);
}

}; // namespace chart::geometry
