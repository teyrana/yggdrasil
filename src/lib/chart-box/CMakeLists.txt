
# ============= Chart Base Library =================
# # this is a header-only library ???
SET(LIB_NAME chartbox)
SET(LIB_HEADERS chart-box.hpp
                chart-frame-mapping.hpp chart-frame-mapping.cpp
                chart-layer-interface.hpp chart-layer-interface.inl
                # chart-layer.hpp
                # src/base/chart-interface.hpp src/base/chart-loaders.inl
                # src/base/readers.inl
                # src/base/writers.inl
                )
SET(LIB_SOURCES chart-box.cpp
                )

MESSAGE( STATUS "Generating ChartBox Library: ${LIB_NAME}")
MESSAGE( STATUS "    with headers: ${LIB_HEADERS}")
MESSAGE( STATUS "    with sources: ${LIB_SOURCES}")

# Generate the static library from the sources
add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})
target_link_libraries(${LIB_NAME} PRIVATE ${LIBRARY_LINKAGE})
target_link_libraries(${LIB_NAME} CONAN_PKG::gdal )
target_link_libraries(${LIB_NAME} CONAN_PKG::fmt)
