# Copyright (C) 2007-2009 LuaDist.
# Created by Peter Kapec <kapecp@gmail.com>
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
#	Note:
#		Searching headers and libraries is very simple and is NOT as powerful as scripts
#		distributed with CMake, because LuaDist defines directories to search for.
#		Everyone is encouraged to contact the author with improvements. Maybe this file
#		becomes part of CMake distribution sometimes.

# - Find FreeGLUT
# Find the native FreeGLUT headers and libraries.
#
#  FREEGLUT_INCLUDE_DIRS - where to find freeglut.h, etc.
#  FREEGLUT_LIBRARIES    - List of libraries when using FreeGLUT.
#  FREEGLUT_FOUND        - True if FreeGLUT found.

# Look for header files
list(APPEND CMAKE_PREFIX_PATH "include" "lib" "bin")

# Look for the header file.
find_path(FREEGLUT_INCLUDE_DIR NAMES GL/freeglut.h 
    HINTS ${LIBRARY_SEARCH_PATH}/freeglut
    PATH_SUFFIXES ${CMAKE_PREFIX_PATH}
)

# Look for the static library first
find_library(FREEGLUT_LIBRARY NAMES freeglut_static
    HINTS ${LIBRARY_SEARCH_PATH}/freeglut
    PATH_SUFFIXES ${CMAKE_PREFIX_PATH}
)

find_library(FREEGLUT_DEBUG_LIBRARY NAMES freeglut_staticd
    HINTS ${LIBRARY_SEARCH_PATH}/freeglut
    PATH_SUFFIXES ${CMAKE_PREFIX_PATH}
)

# Handle the QUIETLY and REQUIRED arguments and set FREEGLUT_FOUND to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREEGLUT DEFAULT_MSG FREEGLUT_LIBRARY FREEGLUT_DEBUG_LIBRARY FREEGLUT_INCLUDE_DIR)

# Copy the results to the output variables.
IF(NOT FREEGLUT_FOUND)
    set(FREEGLUT_LIBRARY)
    set(FREEGLUT_DEBUG_LIBRARY)
	set(FREEGLUT_INCLUDE_DIR)
ENDIF(NOT FREEGLUT_FOUND)

#mark_as_advanced(FREEGLUT_INCLUDE_DIR FREEGLUT_LIBRARY FREEGLUT_DEBUG_LIBRARY)