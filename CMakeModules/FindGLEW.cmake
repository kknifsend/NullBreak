
#Find glew header
find_path(GLEW_INCLUDE_DIR "GL/glew.h"
  HINTS ${LIBRARY_SEARCH_PATH}/GLEW
  PATH_SUFFIXES ${CMAKE_PREFIX_PATH}
)

#Find glew library
find_library(GLEW_LIBRARY 
  NAMES glew32s
  
  HINTS ${LIBRARY_SEARCH_PATH}/GLEW
  PATH_SUFFIXES ${CMAKE_PREFIX_PATH}
)

#Message(${GLEW_INCLUDE_DIR})
#Message(${GLEW_LIBRARY})

# Handle the QUIETLY and REQUIRED arguments and set FREEGLUT_FOUND to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)

#check that we have found everything
IF(NOT GLEW_FOUND)
    set(GLEW_LIBRARY)
    set(GLEW_INCLUDE_DIR)
ENDIF(NOT GLEW_FOUND)

#Mark options as advanced
#mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)