# ============= Chart Base Library =================
# # this is a header-only library ???
SET(LIB_NAME rollinggrid)
SET(LIB_HEADERS fixed-grid.hpp fixed-grid.cpp
                # rolling-grid.hpp rolling-grid.cpp
    )
SET(LIB_SOURCES fixed-grid.cpp
                # rolling-grid.cpp
                )

MESSAGE( STATUS "Generating GridChart Library: ${LIB_NAME}")
MESSAGE( STATUS "    with headers: ${LIB_HEADERS}")
MESSAGE( STATUS "    with sources: ${LIB_SOURCES}")

# internal library dependency
target_link_libraries(chartbox)

# Generate the static library from the sources
add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})
TARGET_LINK_LIBRARIES(${LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})

INCLUDE_DIRECTORIES(${CMAKE_SRC_DIRECTORY}/src/lib/grid)
