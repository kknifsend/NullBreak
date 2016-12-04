cmake_minimum_required (VERSION 3.4)

# Set up sources/headers
set(Sources
    SfinaeMain.cpp
)

set(Headers
)

add_executable(Sfinae ${Sources})
target_link_libraries(Sfinae Utilities)
add_dependencies(Sfinae Utilities)
