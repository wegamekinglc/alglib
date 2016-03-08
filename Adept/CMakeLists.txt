file(GLOB_RECURSE ADEPT_FILES "*.h" "*.cpp")
add_library(adept ${LIBRARY_BUILD_TYPE} ${ADEPT_FILES})