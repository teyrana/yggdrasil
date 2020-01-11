#!/bin/bash


SEARCH_ROOT=src
FORMAT_FILE=.clang-format

find ${SEARCH_ROOT} -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.inl" \) -exec clang-format -i {} \;