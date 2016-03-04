
file(GLOB OPTIMIZATION_FILES ".h" "*.hpp" "*.cpp")
add_executable(Optimization ${OPTIMIZATION_FILES})
target_link_libraries(Optimization alglib)