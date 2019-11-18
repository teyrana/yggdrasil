CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT( libchartbox )

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake) 
MESSAGE(STATUS "bindir: " ${CMAKE_BINARY_DIR})

#=============================================================================
# Set the output directories for the binary and library files
#=============================================================================

#=============================================================================
# unknown block purpose
#=============================================================================
# set(CMakeScripts_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# set(CMAKE_MODULE_PATH ${CMakeScripts_MODULE_DIR})

# ============= Conan-Managed Dependencies =================
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CONAN_LIBS})

# ============= GDal =================
# Used for most file I/O
SET(GDAL_PATH "/usr/local/Cellar/gdal/2.4.2/lib/")
# SET(GDAL_LINKAGE "/usr/local/Cellar/gdal/2.4.2/lib/libgdal.dylib")
SET(GDAL_LINKAGE gdal)
SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${GDAL_LINKAGE})

# ADD_DEFINITIONS(-DENABLE_GDAL)

MESSAGE( STATUS "Configured GDal... ")
MESSAGE( STATUS "    with path: ${GDAL_PATH}")

# Set Compile Flags
#=============================================================================
# specify as CMAKE_CXX_FLAGS, instead of 'TARGET_COMPILE_OPTIONS(...' because we have multiple compilation targets.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17  -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic ")

set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Og -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")

MESSAGE( STATUS ">> Configuring Build Type: ${CMAKE_BUILD_TYPE}")
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE( STATUS ".... Configuring Debug Mode.")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g")
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
    MESSAGE( STATUS ".... Configuring Release Mode.")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

# ============= Enable Testing  =================
#ExternalProject_Add(GTest
#    TMP_DIR ${PROJECT_SOURCE_DIR}/vendor/googletest 
#    GIT_REPOSITORY https://github.com/google/googletest.git) 
#
# INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}) 

enable_testing()

INCLUDE_DIRECTORIES(${CONAN_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(extern)
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(src)

# ============= Chart Base Library =================
# # this is a header-only library
SET(CHART_BASE_LIB_NAME chartbase)
SET(LIB_HEADERS src/base/chart-interface.hpp
                src/base/chart-loaders.inl
                # src/base/readers.hpp src/base/readers.inl
                # src/base/writers.hpp
                # src/node/grid_node.hpp
                # src/node/quad_node.hpp
                # src/node/tile_node.hpp
                # src/quadtree/tree.hpp
                # include/world_tree.hpp
                )
# SET(LIB_SOURCES src/base/writers.cpp
                # src/node/grid-node.cpp
                # src/node/quad/quad_node.cpp
                # src/node/tile_node.cpp
                # src/quadtree/tree.cpp
                # )

# # Generate the static library from the sources
# add_library(${CHART_BASE_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})
# TARGET_LINK_LIBRARIES(${CHART_BASE_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})
# SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_BASE_LIB_NAME})

# ============= Chart Geometry Library =================
SET(CHART_GEOMETRY_LIB_NAME chartgeom)
SET(LIB_HEADERS src/geometry/bounds.hpp
                src/geometry/interpolate.hpp src/geometry/interpolate.inl
                src/geometry/path.hpp
                src/geometry/polygon.hpp
                src/geometry/raster.hpp
                )
SET(LIB_SOURCES src/geometry/bounds.cpp
                src/geometry/path.cpp
                src/geometry/polygon.cpp
                )
add_library(${CHART_GEOMETRY_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

TARGET_LINK_LIBRARIES(${CHART_GEOMETRY_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})
SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_GEOMETRY_LIB_NAME})


# ============= Chart Grid Library =================
SET(CHART_GRID_LIB_NAME chartgrid)
SET(LIB_HEADERS src/grid/fixed-grid.hpp )
SET(LIB_SOURCES src/grid/fixed-grid.cpp )

add_library(${CHART_GRID_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

TARGET_LINK_LIBRARIES(${CHART_GRID_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})

SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_GRID_LIB_NAME})

# ============= Chart Index Library =================
# Because this is a header only, don't bother to generate a compiled lib***.a
SET(CHART_INDEX_LIB_NAME chartindex)
SET(LIB_HEADERS src/index/index2u.hpp
                src/index/row-index.hpp
                )
SET(LIB_SOURCES src/index/index2u.cpp )

add_library(${CHART_INDEX_LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

TARGET_LINK_LIBRARIES(${CHART_INDEX_LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})

SET(LIBRARY_LINKAGE ${LIBRARY_LINKAGE} ${CHART_INDEX_LIB_NAME})

# ============= Build Project Tests  =================
# https://cmake.org/cmake/help/v3.0/module/FindGTest.html 
SET(TEST_EXE testall)
SET(TEST_SOURCES    src/geometry/bounds.test.cpp
                    src/geometry/interpolate.test.cpp
                    src/geometry/path.test.cpp
                    src/geometry/polygon.test.cpp
                    src/index/row-index.test.cpp
                    src/index/z-index.test.cpp
                    src/grid/fixed-grid.test.cpp
                    # src/search/a-star.test.cpp
                    )

SET(TEST_LINKAGE ${CHART_GRID_LIB_NAME} ${LIBRARY_LINKAGE} ${CONAN_LIBS} gtest gtest_main )

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
