#######################################################
# e.g.
# cmake -G "Visual Studio 14 2015 Win64"
#######################################################

cmake_minimum_required(VERSION 2.8)

PROJECT(alglib)

set(CMAKE_USE_MKL_LIB "False")
set(BUILD_SHARED_LIB "0")

if(${BUILD_SHARED_LIB} STREQUAL "1")
    set(LIBRARY_BUILD_TYPE SHARED)
    set(Boost_USE_STATIC_LIBS OFF)
else()
    set(LIBRARY_BUILD_TYPE STATIC)
    set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost REQUIRED COMPONENTS chrono system)
find_package(CUDA REQUIRED)

set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set(LIBRARY_OUTPUT_PATH ${BUILD_DIR})

# optimization for Intel cpu for alglib
add_definitions(-DAE_CPU=AE_INTEL)
add_definitions(-DHAVE_CONFIG_H)

if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DMKL_ILP64")
    endif()
endif()

if(MSVC)
  set(IPOPT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ipopt/lib/x64)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ipopt/include/coin)
  link_directories(${IPOPT_LIB_DIR})
endif()

set(IPOPT_ROOT_DIR /home/wegamkeinglc/Documents/dev/svn/CoinIpopt)
include_directories(${IPOPT_ROOT_DIR}/include/coin)
link_directories(${IPOPT_ROOT_DIR}/lib)
link_directories(/usr/local/lib)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${CUDA_TOOLKIT_INCLUDE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/alglib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Eigen/include/eigen3)

add_subdirectory(alglib)
add_subdirectory(CppAD)
add_subdirectory(examples)
add_subdirectory(optimizer)
