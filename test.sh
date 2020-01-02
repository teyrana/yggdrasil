#!/bin/bash

clear 

# Build Tests
# ======
./build.sh
SUCCESS=$?
if [ "$SUCCESS" -ne "0" ]; then
    exit $SUCCESS
fi

# Run Tests
# ======
if [ ! -z "$@" ]; then
    TEST_ARGS=--gtest_filter="$@"*
fi
build/bin/testall $TEST_ARGS
SUCCESS=$?
if [ "$SUCCESS" -ne "0" ]; then
    exit $SUCCESS
fi

