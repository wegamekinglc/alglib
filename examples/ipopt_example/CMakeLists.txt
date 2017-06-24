file(GLOB EXAMPLE_FILES "*.hpp" "*.cpp")
add_executable(ipopt_example ${EXAMPLE_FILES})

target_link_libraries(ipopt_example ipopt)
target_link_libraries(ipopt_example ${Boost_CHRONO_LIBRARY})
target_link_libraries(ipopt_example ${Boost_SYSTEM_LIBRARY})
target_link_libraries(ipopt_example alglib)