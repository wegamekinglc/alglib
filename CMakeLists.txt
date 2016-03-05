#######################################################
# e.g. 
# cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
#######################################################

cmake_minimum_required(VERSION 2.8)

PROJECT(alglib)

# optimization for Intel cpu for alglib
add_definitions(-DAE_CPU=AE_INTEL) 

set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set(LIBRARY_OUTPUT_PATH ${BUILD_DIR})
set(Boost_INCLUDE_DIR YOUR_BOOST_LIBRARY_DIR_HERE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/alglib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Adept)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(alglib)
add_subdirectory(Adept)
add_subdirectory(examples)