
file(GLOB OPTIMIZATION_FILES "*.hpp" "*.cpp")
add_executable(Optimization ${OPTIMIZATION_FILES})
target_link_libraries(Optimization alglib)
target_link_libraries(Optimization adept)