file(GLOB_RECURSE ALGLIB_FILES "*.h" "*.cpp")
add_library(alglib ${LIBRARY_BUILD_TYPE} ${ALGLIB_FILES})