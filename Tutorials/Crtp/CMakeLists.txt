cmake_minimum_required (VERSION 3.4)

# Set up sources/headers
set(Sources
    CrtpMain.cpp
)

set(Headers
)

add_executable(Crtp ${Sources})
target_link_libraries(Crtp Utilities)
add_dependencies(Crtp Utilities)
