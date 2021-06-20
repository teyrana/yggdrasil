
# ============= Build Profiling Program  =================
SET(EXE_NAME profile)
SET(EXE_SOURCES main.cpp)

MESSAGE( STATUS "Generating Profile program: ${EXE_NAME}")
MESSAGE( STATUS "    with sources: ${EXE_SOURCES}")
MESSAGE( STATUS "    with linkage: ${EXE_LINKAGE}") 

# DEVEL
ADD_EXECUTABLE( ${EXE_NAME} ${EXE_SOURCES})
TARGET_INCLUDE_DIRECTORIES(${EXE_NAME} PRIVATE vendor)
TARGET_LINK_LIBRARIES(${EXE_NAME} PRIVATE ${EXE_LINKAGE}) 
