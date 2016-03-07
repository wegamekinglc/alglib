file(GLOB_RECURSE ADEPT_FILES "*.h" "*.cpp")
add_library(adept SHARED ${ADEPT_FILES})