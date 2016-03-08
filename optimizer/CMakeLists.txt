file(GLOB OPTIMIZER_FILES "*.hpp" "*.cpp")
add_library(optimizer ${LIBRARY_BUILD_TYPE} ${OPTIMIZER_FILES})
target_link_libraries(optimizer alglib)