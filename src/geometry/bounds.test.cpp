// GPL v3 (c) 2020

#include <iostream>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

#include "polygon.hpp"

// #include "geometry/layout.hpp"

using std::cerr;
using std::endl;

using Eigen::Vector2d;

using chart::geometry::Bounds;

namespace chart::geometry {

TEST(Bounds, ConstructConfiguration) {
    const Bounds b;
    ASSERT_DOUBLE_EQ(b.southwest.x(), DBL_MAX);
    ASSERT_DOUBLE_EQ(b.southwest.y(), DBL_MAX);
    ASSERT_DOUBLE_EQ(b.northeast.x(), -DBL_MAX);
    ASSERT_DOUBLE_EQ(b.northeast.y(), -DBL_MAX);
}

TEST(Bounds, ConstructFromCorners) {
    const Bounds b({5,6}, {22,57});
    ASSERT_DOUBLE_EQ(b.southwest.x(), 5);
    ASSERT_DOUBLE_EQ(b.southwest.y(), 6);
    ASSERT_DOUBLE_EQ(b.northeast.x(), 22);
    ASSERT_DOUBLE_EQ(b.northeast.y(), 57);
}

TEST(Bounds, MakeFromCorners){
    const Bounds b = Bounds::make_from_corners( {1,2}, {12,11} );
    ASSERT_DOUBLE_EQ(b.southwest.x(), 1);
    ASSERT_DOUBLE_EQ(b.southwest.y(), 2);
    ASSERT_DOUBLE_EQ(b.northeast.x(), 12);
    ASSERT_DOUBLE_EQ(b.northeast.y(), 11);
}

TEST(Bounds, MakeFromCenter) {
    const Vector2d center(0,0);
    const Vector2d widths(120,112);
    const Bounds b = Bounds::make_from_center(center,widths);

    ASSERT_DOUBLE_EQ(b.southwest.x(), -60);
    ASSERT_DOUBLE_EQ(b.southwest.y(), -56);
    ASSERT_DOUBLE_EQ(b.northeast.x(), 60);
    ASSERT_DOUBLE_EQ(b.northeast.y(), 56);
}

};