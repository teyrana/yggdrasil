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

using chart::geometry::Bounds;

namespace chart::geometry {

TEST(Bounds, ConstructConfiguration) {
    const Bounds b;
    ASSERT_DOUBLE_EQ(b.southwest().x(), DBL_MAX);
    ASSERT_DOUBLE_EQ(b.southwest().y(), DBL_MAX);
    ASSERT_DOUBLE_EQ(b.northeast().x(), -DBL_MAX);
    ASSERT_DOUBLE_EQ(b.northeast().y(), -DBL_MAX);
}

TEST(Bounds, ConstructFromCorners) {
    const Bounds b({5,6}, {22,57});
    ASSERT_DOUBLE_EQ(b.southwest().x(), 5);
    ASSERT_DOUBLE_EQ(b.southwest().y(), 6);
    ASSERT_DOUBLE_EQ(b.northeast().x(), 22);
    ASSERT_DOUBLE_EQ(b.northeast().y(), 57);
}

TEST(Bounds, MakeFromCorners){
    const Bounds b = Bounds::make_from_corners( {1,2}, {12,11} );
    ASSERT_DOUBLE_EQ(b.southwest().x(), 1);
    ASSERT_DOUBLE_EQ(b.southwest().y(), 2);
    ASSERT_DOUBLE_EQ(b.northeast().x(), 12);
    ASSERT_DOUBLE_EQ(b.northeast().y(), 11);
}

TEST(Bounds, MakeFromCenter) {
    const Vector2d center(0,0);
    const Vector2d widths(120,112);
    const Bounds b = Bounds::make_from_center(center,widths);

    ASSERT_DOUBLE_EQ(b.southwest().x(), -60);
    ASSERT_DOUBLE_EQ(b.southwest().y(), -56);
    ASSERT_DOUBLE_EQ(b.northeast().x(), 60);
    ASSERT_DOUBLE_EQ(b.northeast().y(), 56);
}

TEST(Bounds, MakeSquare) {
    const Vector2d center(0,0);
    const Vector2d widths(20,50);
    const Bounds rectangle = Bounds::make_from_center(center,widths);

    ASSERT_FALSE(rectangle.is_square());
    ASSERT_DOUBLE_EQ(rectangle.southwest().x(), -10);
    ASSERT_DOUBLE_EQ(rectangle.southwest().y(), -25);
    ASSERT_DOUBLE_EQ(rectangle.northeast().x(),  10);
    ASSERT_DOUBLE_EQ(rectangle.northeast().y(),  25);

    // test target:
    const Bounds square = Bounds::make_square(rectangle);

    EXPECT_TRUE(square.is_square());
    ASSERT_DOUBLE_EQ(square.southwest().x(), -25);
    ASSERT_DOUBLE_EQ(square.southwest().y(), -25);
    ASSERT_DOUBLE_EQ(square.northeast().x(),  25);
    ASSERT_DOUBLE_EQ(square.northeast().y(),  25);
}


TEST(Bounds, LoadJSONMalformed) {
   nlohmann::json doc = { {"x", 100}, {"y", 100}, {"width", 64} };
    
    const Bounds rectangle = Bounds::make_from_json(doc);

    ASSERT_FALSE( rectangle.is_valid() );
}

TEST(Bounds, LoadJSONCorners) {
    nlohmann::json doc = { {"northeast",{40,42}}, {"southwest",{50,66}} };

    const Bounds rectangle = Bounds::make_from_json(doc);

    ASSERT_FALSE(rectangle.is_square());
    ASSERT_DOUBLE_EQ(rectangle.southwest().x(), 40);
    ASSERT_DOUBLE_EQ(rectangle.southwest().y(), 42);
    ASSERT_DOUBLE_EQ(rectangle.northeast().x(), 50);
    ASSERT_DOUBLE_EQ(rectangle.northeast().y(), 66);
}

TEST(Bounds, LoadJSONAnchorWidth) {
   nlohmann::json doc = {{"anchor", {20,22}},{"width",10}};
    
    const Bounds rectangle = Bounds::make_from_json(doc);

    ASSERT_TRUE(rectangle.is_square());
    ASSERT_DOUBLE_EQ(rectangle.southwest().x(), 20);
    ASSERT_DOUBLE_EQ(rectangle.southwest().y(), 22);
    ASSERT_DOUBLE_EQ(rectangle.northeast().x(), 30);
    ASSERT_DOUBLE_EQ(rectangle.northeast().y(), 32);
}

TEST(Bounds, LoadJSONCenterWidth) {
    nlohmann::json doc = {{"center", {50,50}},{"width",10}};
    
    const Bounds rectangle = Bounds::make_from_json(doc);

    ASSERT_TRUE(rectangle.is_square());
    ASSERT_DOUBLE_EQ(rectangle.southwest().x(), 45);
    ASSERT_DOUBLE_EQ(rectangle.southwest().y(), 45);
    ASSERT_DOUBLE_EQ(rectangle.northeast().x(), 55);
    ASSERT_DOUBLE_EQ(rectangle.northeast().y(), 55);
}

};