cmake_minimum_required (VERSION 3.4)

find_package(EIGEN MODULE REQUIRED)
Message(${EIGEN_INCLUDE_DIR})
# Set up sources/headers
set(Sources
    TransformFittingMain.cpp
    TransformFit.cpp
)

set(Headers
    TransformFit.hpp
)

add_executable(TransformFitting ${Sources} ${Headers})
target_link_libraries(TransformFitting Utilities)
include_directories(${EIGEN_INCLUDE_DIR})
add_dependencies(TransformFitting Utilities)
