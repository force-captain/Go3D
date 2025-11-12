cmake_minimum_required(VERSION 3.10)
project(Go3D CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Automatically collect all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/*.h"
)

add_executable(go3d ${SOURCES} ${HEADERS})

# Add include directory
target_include_directories(go3d PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Find GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

if (GLFW_FOUND)
    target_include_directories(go3d PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(go3d PRIVATE ${GLFW_LIBRARIES})
else()
    message(FATAL_ERROR "GLFW not found")
endif()

target_link_libraries(go3d PRIVATE GL dl m)
