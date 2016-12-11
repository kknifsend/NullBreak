cmake_minimum_required (VERSION 3.4)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

add_subdirectory(Sfinae)
add_subdirectory(Crtp)
add_subdirectory(MoveSymmantics)