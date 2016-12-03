cmake_minimum_required (VERSION 3.4)

# Set up sources/headers
set(Sources
    SfinaeMain.cpp
)

set(Headers
)

add_executable(Sfinae ${Sources})
add_dependencies(Sfinae Utilities)
include_directories(${Utilities_Include_Directory})
