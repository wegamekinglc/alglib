file(GLOB_RECURSE ALGLIB_FILES "*.h" "*.cpp")
add_library(alglib STATIC ${ALGLIB_FILES})