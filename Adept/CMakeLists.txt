file(GLOB_RECURSE ADEPT_FILES "*.h" "*.cpp")
add_library(adept STATIC ${ADEPT_FILES})