
set(EIGEN_INCLUDE_DIR ${LIBRARY_SEARCH_PATH}/Eigen CACHE PATH "Path to an include.")

# Handle the QUIETLY and REQUIRED arguments and set FREEGLUT_FOUND to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(EIGEN DEFAULT_MSG EIGEN_INCLUDE_DIR)

IF(NOT EIGEN_FOUND)
    set(EIGEN_INCLUDE_DIR)
ENDIF(NOT EIGEN_FOUND)
