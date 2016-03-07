file(GLOB OPTIMIZER_FILES "*.hpp" "*.cpp")
add_library(optimizer SHARED ${OPTIMIZER_FILES})
target_link_libraries(optimizer alglib)