// GPL v3 (c) 2020, Daniel Williams 

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"

#include "z-index.hpp"
using chart::index::ZOrderCurveIndex;

using std::cerr;
using std::endl;
using std::isnan;

typedef uint8_t cell_t;
typedef std::array<cell_t, 16> array_t;
typedef std::vector<cell_t> vector_t;

TEST(ZOrderIndex, IndexZOrderCurveConst) {
    ASSERT_EQ(sizeof(uint64_t), 8); // should be guaranteed, by spec
    ASSERT_EQ(sizeof(size_t), 8);   // not strictly necessary

    array_t grid = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    const ZOrderCurveIndex<4, cell_t> index(grid);

    ASSERT_EQ(index.padding, 60);

    EXPECT_EQ(index(0, 0), 0);
    EXPECT_EQ(index(1, 0), 1);
    EXPECT_EQ(index(0, 1), 2);
    EXPECT_EQ(index(1, 1), 3);

    EXPECT_EQ(index(2, 0), 4);
    EXPECT_EQ(index(3, 0), 5);
    ASSERT_EQ(index(2, 1), 6);
    ASSERT_EQ(index(3, 1), 7);

    EXPECT_EQ(index(0, 2), 8);
    EXPECT_EQ(index(1, 2), 9);
    ASSERT_EQ(index(0, 3), 10);
    ASSERT_EQ(index(1, 3), 11);

    EXPECT_EQ(index(2, 2), 12);
    EXPECT_EQ(index(3, 2), 13);
    ASSERT_EQ(index(2, 3), 14);
    ASSERT_EQ(index(3, 3), 15);
}

TEST(ZOrderIndex, IndexZOrderCurveMutable) {
    ASSERT_EQ(sizeof(uint64_t), 8); // should be guaranteed, by spec
    ASSERT_EQ(sizeof(size_t), 8);   // not strictly necessary

    array_t grid = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    ZOrderCurveIndex<4, cell_t> index(grid);

    ASSERT_EQ(index.padding, 60);

    EXPECT_EQ(index(0, 0), 0);
    EXPECT_EQ(index(1, 0), 1);
    EXPECT_EQ(index(0, 1), 2);
    EXPECT_EQ(index(1, 1), 3);

    // modify grid, and ensure that it still retrieves the value
    index(0, 0) = 100;
    index(1, 0) = 101;
    index(0, 1) = 102;
    index(1, 1) = 103;

    EXPECT_EQ(index(0, 0), 100);
    EXPECT_EQ(index(1, 0), 101);
    EXPECT_EQ(index(0, 1), 102);
    EXPECT_EQ(index(1, 1), 103);
}
