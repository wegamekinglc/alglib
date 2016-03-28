#######################################################
# e.g. 
# cmake -G "Visual Studio 14 2015 Win64"
#######################################################

cmake_minimum_required(VERSION 2.8)

PROJECT(alglib)

set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set(LIBRARY_OUTPUT_PATH ${BUILD_DIR})

# optimization for Intel cpu for alglib
set(CMAKE_USE_MKL_LIB "TRUE")
add_definitions(-DAE_CPU=AE_INTEL)

set(BUILD_SHARED_LIB "1")
if(${BUILD_SHARED_LIB} STREQUAL "1")
    set(LIBRARY_BUILD_TYPE SHARED)
else()
    set(LIBRARY_BUILD_TYPE STATIC)
endif()

set(Boost_INCLUDE_DIR D:/dev/boost_1_60_0)

if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DMKL_ILP64")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMKL_ILP64")
    endif()
    include_directories("D:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/include")
    link_directories("D:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64_win")
    link_directories("D:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/compiler/lib/intel64_win")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/alglib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Eigen/include/eigen3)

add_subdirectory(alglib)
add_subdirectory(CppAD)
add_subdirectory(examples)
add_subdirectory(optimizer)
