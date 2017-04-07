include_directories(${CMAKE_SOURCE_DIR}/optimizer)

file(GLOB OPTIMIZERTEST_FILES "*.hpp" "*.cpp")
add_executable(optimizerTest ${OPTIMIZERTEST_FILES})
target_link_libraries(optimizerTest optimizer)