
# # ============= Chart Base Library =================
# # # this is a header-only library
# SET(CHART_BASE_LIB_NAME chartbase)
# SET(LIB_HEADERS src/base/chart-interface.hpp
#                 src/base/chart-loaders.inl
#                 # src/base/readers.inl
#                 # src/base/writers.inl
#     )
# # SET(LIB_SOURCES src/base/writers.cpp
#                 # )

# # # Generate the static library from the sources
# # add_library(${CHART_BASE_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})
# # TARGET_LINK_LIBRARIES(${CHART_BASE_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})
# # SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_BASE_LIB_NAME})

# # ============= Chart Geometry Library =================
# SET(CHART_GEOMETRY_LIB_NAME chartgeom)
# SET(LIB_HEADERS src/geometry/bounds.hpp
#                 src/geometry/interpolate.hpp src/geometry/interpolate.inl
#                 src/geometry/path.hpp
#                 src/geometry/polygon.hpp
#                 src/geometry/raster.hpp
#                 )
# SET(LIB_SOURCES src/geometry/bounds.cpp
#                 src/geometry/path.cpp
#                 src/geometry/polygon.cpp
#                 )
# add_library(${CHART_GEOMETRY_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

# TARGET_LINK_LIBRARIES(${CHART_GEOMETRY_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})
# SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_GEOMETRY_LIB_NAME})


# # ============= Chart Grid Library =================
# SET(CHART_GRID_LIB_NAME chartgrid)
# SET(LIB_HEADERS src/grid/fixed-grid.hpp 
#                 src/grid/rolling-grid.hpp
#     )
# SET(LIB_SOURCES src/grid/fixed-grid.cpp
#                 src/grid/rolling-grid.cpp
#     )
# add_library(${CHART_GRID_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

# TARGET_LINK_LIBRARIES(${CHART_GRID_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})

# SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_GRID_LIB_NAME})

# # ============= Chart Index Library =================
# # Because this is a header only, don't bother to generate a compiled lib***.a
# SET(CHART_INDEX_LIB_NAME chartindex)
# SET(LIB_HEADERS src/index/index2u.hpp
#                 src/index/row-index.hpp
#                 )
# SET(LIB_SOURCES src/index/index2u.cpp )

# add_library(${CHART_INDEX_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

# TARGET_LINK_LIBRARIES(${CHART_INDEX_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})

# SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_INDEX_LIB_NAME})

# # ============= Chart Search Library =================
# SET(CHART_SEARCH_LIB_NAME chartsearch)
# SET(LIB_HEADERS src/search/a-star.hpp
#                 src/search/a-star.inl
#                 src/search/cost.hpp
#                 # src/search/rrt-star.hpp
#                 # src/search/rrt-star.inl
#     )
# SET(LIB_SOURCES src/search/cost.cpp )

# add_library(${CHART_SEARCH_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

# TARGET_LINK_LIBRARIES(${CHART_SEARCH_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})

# SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_SEARCH_LIB_NAME})

# # ============= Build Project Tests  =================
# # https://cmake.org/cmake/help/v3.0/module/FindGTest.html 
SET(TEST_EXE testall)
# SET(TEST_SOURCES    src/geometry/bounds.test.cpp
#                     src/geometry/interpolate.test.cpp
#                     src/geometry/path.test.cpp
#                     src/geometry/polygon.test.cpp
#                     src/index/row-index.test.cpp
#                     src/index/z-index.test.cpp
#                     src/grid/rolling-grid.test.cpp
#                     src/grid/fixed-grid.test.cpp
#                     src/search/a-star.test.cpp
#                     # src/search/rrt-star.test.cpp
#     )

SET(TEST_LINKAGE ${CHART_GRID_LIB_NAME} ${LIBRARY_LINKAGE} ${CONAN_LIBS} gtest gtest_main pthread)

MESSAGE( STATUS "Generating all tests ...")
MESSAGE( STATUS "    with name: ${TEST_EXE}")
MESSAGE( STATUS "    with sources: ${TEST_SOURCES}")
MESSAGE( STATUS "    with linkage: ${TEST_LINKAGE}")

ADD_EXECUTABLE( ${TEST_EXE} ${TEST_SOURCES}) 
ADD_TEST(AllTestsInFoo ${TEST_EXE}) 

TARGET_LINK_LIBRARIES(${TEST_EXE} ${TEST_LINKAGE})


# ============= Build Profiling Program  =================
SET(PROFILE_EXE profile)
SET(PROFILE_SOURCES tools/profile/main.cpp)

MESSAGE( STATUS "Generating profiling program: ${PROFILE_EXE}")
MESSAGE( STATUS "    with sources: ${PROFILE_SOURCES}")
MESSAGE( STATUS "    with linkage: ${PROFILE_LINKAGE}") 

# # DEVEL
# ADD_EXECUTABLE( ${PROFILE_EXE} ${PROFILE_SOURCES})
# TARGET_INCLUDE_DIRECTORIES(${PROFILE_EXE} PRIVATE vendor)

# TARGET_LINK_LIBRARIES(${PROFILE_EXE} PRIVATE ${YGGDRASIL_LIB_NAME} ${PROFILE_LINKAGE}) 
