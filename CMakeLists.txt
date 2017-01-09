cmake_minimum_required (VERSION 3.4)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
message(${CMAKE_MODULE_PATH})


set(LIBRARY_SEARCH_PATH
    "E:/Libraries/C++" CACHE STRING "Library to use when searching for libraries."
)

add_subdirectory(Tutorials)
add_subdirectory(Utilities)
