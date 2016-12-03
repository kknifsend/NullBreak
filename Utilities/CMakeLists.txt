cmake_minimum_required (VERSION 3.4)

set(TargetName Utilities)
set(TargetName_CreateTest ${TargetName}_CreateTest)
set(${TargetName}_Dir "Inc/${TargetName}")
set(${TargetName}_Include_Directory "${TargetName}/Inc" PARENT_SCOPE)

set(Sources
    Src/Timer.cpp
)

set(Headers
    ${${TargetName}_Dir}/Timer.hpp
)

# Utilities library
add_library(${TargetName} STATIC ${Headers} ${Sources})
include_directories(Inc)

set(${TargetName_CreateTest} OFF CACHE BOOL "Enable '${TargetName}' test code.")
if(${TargetName_CreateTest})
    # Test code
    set(TestNames TimerTest)
    
    foreach(TestName ${TestNames})
        set(TestTargetName ${TargetName}_${TestName})
        add_executable(${TestTargetName} Test/${TestName}.cpp)
        include_directories(Inc ${EIGEN_INCLUDE_DIR})
        target_link_libraries(${TestTargetName} ${TargetName})
        add_dependencies(${TestTargetName} ${TargetName})
    endforeach(TestName ${Emulators_Test})
endif(${TargetName_CreateTest})

set(${${TargetName}_Include_Directory} ${TargetName}/${TargetName}_Include_Directory PARENT_SCOPE)