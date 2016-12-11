cmake_minimum_required (VERSION 3.4)

# Set up sources/headers
set(Sources
    MoveMain.cpp
)

set(Headers
    Collection.hpp
)

add_executable(MoveSymmantics ${Headers} ${Sources})
target_link_libraries(MoveSymmantics Utilities)
add_dependencies(MoveSymmantics Utilities)
