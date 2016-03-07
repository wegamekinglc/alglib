file(GLOB_RECURSE ALGLIB_FILES "*.h" "*.cpp")
add_library(alglib SHARED ${ALGLIB_FILES})