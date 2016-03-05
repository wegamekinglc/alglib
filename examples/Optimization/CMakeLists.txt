
file(GLOB OPTIMIZATION_FILES "*.hpp" "*.cpp")
add_executable(Optimization ${OPTIMIZATION_FILES} CostCalculator_adept.hpp CostCalculator_adept.cpp ParameterReader.hpp ParameterReader.cpp)
target_link_libraries(Optimization alglib)
target_link_libraries(Optimization adept)