// GPL v3 (c) 2020

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"

#include "row-index.hpp"
using chart::index::Index2u;
using chart::index::RowMajorIndex;

using std::cerr;
using std::endl;
using std::isnan;

typedef uint8_t cell_t;
typedef std::array<cell_t, 16> array_t;
typedef std::vector<cell_t> vector_t;

TEST(RowMajorIndex, IndexByUInt32) {
    array_t initial_grid = {0, 1, 2,  3,  4,  5,  6,  7,
                            8, 9, 10, 11, 12, 13, 14, 15};

    const RowMajorIndex<array_t, 4> index(initial_grid);

    EXPECT_EQ(index(0, 0), 0);
    EXPECT_EQ(index(1, 0), 1);
    EXPECT_EQ(index(2, 0), 2);
    EXPECT_EQ(index(3, 0), 3);

    ASSERT_EQ(index(0, 1), 4);
    ASSERT_EQ(index(1, 1), 5);
    ASSERT_EQ(index(2, 1), 6);
    ASSERT_EQ(index(3, 1), 7);

    ASSERT_EQ(index(0, 2), 8);
    ASSERT_EQ(index(1, 2), 9);
    ASSERT_EQ(index(2, 2), 10);
    ASSERT_EQ(index(3, 2), 11);

    ASSERT_EQ(index(0, 3), 12);
    ASSERT_EQ(index(1, 3), 13);
    ASSERT_EQ(index(2, 3), 14);
    ASSERT_EQ(index(3, 3), 15);
}

TEST(RowMajorIndex, IndexWithIndex2u) {
    array_t initial_grid = {0, 1, 2,  3,  4,  5,  6,  7,
                            8, 9, 10, 11, 12, 13, 14, 15};

    const RowMajorIndex<array_t, 4> index(initial_grid);

    // and again, with the Index2u type:
    Index2u i(0, 3);
    ASSERT_EQ(index(i), 12);
    i[0] = 1;
    ASSERT_EQ(index(i), 13);
    // ASSERT_EQ(index( {2,3} ), 14);
    // ASSERT_EQ(index( {3,3} ), 15);
}

TEST(RowMajorIndex, IndexByRowMajorModifyArray) {
    array_t initial_grid = {0, 1, 2,  3,  4,  5,  6,  7,
                            8, 9, 10, 11, 12, 13, 14, 15};

    RowMajorIndex<array_t, 4> index(initial_grid);

    EXPECT_EQ(index(0, 0), 0);
    EXPECT_EQ(index(1, 0), 1);
    EXPECT_EQ(index(2, 0), 2);
    EXPECT_EQ(index(3, 0), 3);

    // modify grid, and ensure that it still retrieves the value
    index(0, 0) = 100;
    index(1, 0) = 101;
    index(2, 0) = 102;
    index(3, 0) = 103;

    EXPECT_EQ(index(0, 0), 100);
    EXPECT_EQ(index(1, 0), 101);
    EXPECT_EQ(index(2, 0), 102);
    EXPECT_EQ(index(3, 0), 103);
}

TEST(RowMajorIndex, IndexByRowMajorVectorBacking) {
    vector_t buffer = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    RowMajorIndex<vector_t, 4> index(buffer);

    EXPECT_EQ(index(0, 0), 0);
    EXPECT_EQ(index(1, 0), 1);
    EXPECT_EQ(index(2, 0), 2);
    EXPECT_EQ(index(3, 0), 3);
}
