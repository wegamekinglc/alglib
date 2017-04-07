#######################################################
# e.g. 
# cmake -G "Visual Studio 14 2015 Win64"
#######################################################

cmake_minimum_required(VERSION 2.8)

PROJECT(alglib)

set(CMAKE_USE_MKL_LIB "False")
set(BUILD_SHARED_LIB "1")

set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set(LIBRARY_OUTPUT_PATH ${BUILD_DIR})

# optimization for Intel cpu for alglib
add_definitions(-DAE_CPU=AE_INTEL)
add_definitions(-DHAVE_CONFIG_H)


if(${BUILD_SHARED_LIB} STREQUAL "1")
    set(LIBRARY_BUILD_TYPE SHARED)
else()
    set(LIBRARY_BUILD_TYPE STATIC)
endif()

if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DMKL_ILP64")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ipopt/include/coin)
        set(IPOPT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ipopt/lib/x64/ReleaseMKL)
        link_directories(${IPOPT_LIB_DIR})
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/alglib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Eigen/include/eigen3)

add_subdirectory(alglib)
add_subdirectory(CppAD)
add_subdirectory(examples)
add_subdirectory(optimizer)
