// GPL v3 (c) 2020, Daniel Williams 

#include "gtest/gtest.h"

#include "row-index.hpp"
using chart::index::Index2u;
using chart::index::RowMajorIndex;

TEST(RowMajorIndex, IndexByIntegerPair) {
    const RowMajorIndex<4> index;

    EXPECT_EQ( index.lookup(0, 0), 0);
    EXPECT_EQ( index.lookup(1, 0), 1);
    EXPECT_EQ( index.lookup(2, 0), 2);
    EXPECT_EQ( index.lookup(3, 0), 3);

    ASSERT_EQ( index.lookup(0, 1), 4);
    ASSERT_EQ( index.lookup(1, 1), 5);
    ASSERT_EQ( index.lookup(2, 1), 6);
    ASSERT_EQ( index.lookup(3, 1), 7);

    ASSERT_EQ( index.lookup(0, 2), 8);
    ASSERT_EQ( index.lookup(1, 2), 9);
    ASSERT_EQ( index.lookup(2, 2), 10);
    ASSERT_EQ( index.lookup(3, 2), 11);

    ASSERT_EQ( index.lookup(0, 3), 12);
    ASSERT_EQ( index.lookup(1, 3), 13);
    ASSERT_EQ( index.lookup(2, 3), 14);
    ASSERT_EQ( index.lookup(3, 3), 15);
}

TEST(RowMajorIndex, IndexWithIndex2u) {
    const RowMajorIndex<4> index;

    // and again, with the Index2u type:
    EXPECT_EQ( index.lookup(Index2u( 0, 0)),  0);
    EXPECT_EQ( index.lookup(Index2u( 1, 0)),  1);
    EXPECT_EQ( index.lookup(Index2u( 2, 0)),  2);
    EXPECT_EQ( index.lookup(Index2u( 3, 0)),  3);

    ASSERT_EQ( index.lookup(Index2u( 0, 1)),  4);
    ASSERT_EQ( index.lookup(Index2u( 1, 1)),  5);
    ASSERT_EQ( index.lookup(Index2u( 2, 1)),  6);
    ASSERT_EQ( index.lookup(Index2u( 3, 1)),  7);

    ASSERT_EQ( index.lookup(Index2u( 0, 2)),  8);
    ASSERT_EQ( index.lookup(Index2u( 1, 2)),  9);
    ASSERT_EQ( index.lookup(Index2u( 2, 2)), 10);
    ASSERT_EQ( index.lookup(Index2u( 3, 2)), 11);

    ASSERT_EQ( index.lookup(Index2u( 0, 3)), 12);
    ASSERT_EQ( index.lookup(Index2u( 1, 3)), 13);
    ASSERT_EQ( index.lookup(Index2u( 2, 3)), 14);
    ASSERT_EQ( index.lookup(Index2u( 3, 3)), 15);
}