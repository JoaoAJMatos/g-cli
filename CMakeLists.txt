cmake_minimum_required(VERSION 3.10)

# Definitions
set(PROJECT_NAME gcli)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

project(${PROJECT_NAME} VERSION 0.1.0)

# Include paths
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")

# Assign include directories
include_directories(${INCLUDE_DIRS})

# Find library sources
file(GLOB_RECURSE LIB_SRC_FILES "src/*.c")

# Build static library
add_library(${PROJECT_NAME}_LIB STATIC ${LIB_SRC_FILES})

# Install library
install(TARGETS ${PROJECT_NAME}_LIB DESTINATION lib)

# Install headers
file(GLOB_RECURSE LIB_INC_FILES "include/*.h")
install(FILES ${LIB_INC_FILES} DESTINATION include)

set(SOURCE_FILES main.c)
add_executable(gcli ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_LIB)