#######################################################
# e.g. 
# cmake -G "Visual Studio 14 2015 Win64"
#######################################################

cmake_minimum_required(VERSION 2.8)

PROJECT(alglib)

# optimization for Intel cpu for alglib
add_definitions(-DAE_CPU=AE_INTEL) 

set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set(LIBRARY_OUTPUT_PATH ${BUILD_DIR})
set(LIBRARY_BUILD_TYPE SHARED)
set(Boost_INCLUDE_DIR D:/dev/boost_1_60_0)
set(EIGEN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Eigen/include/eigen3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/alglib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Adept)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${EIGEN_INCLUDE_DIR})

add_subdirectory(alglib)
add_subdirectory(Adept)
add_subdirectory(CppAD)
add_subdirectory(examples)
add_subdirectory(optimizer)