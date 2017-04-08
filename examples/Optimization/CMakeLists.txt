if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")
    add_definitions(-DUSE_MKL_LIB)
endif()

file(GLOB OPTIMIZATION_FILES "*.hpp" "*.cpp")
add_executable(Optimization ${OPTIMIZATION_FILES})

target_link_libraries(Optimization alglib)
target_link_libraries(Optimization ${CUDA_cublas_LIBRARY})
target_link_libraries(Optimization ${CUDA_CUDART_LIBRARY})

if(MSVC)
    target_link_libraries(Optimization IpOptFSS)
    target_link_libraries(Optimization IpOpt-vc8)
else()
    target_link_libraries(Optimization ipopt)
endif()

target_link_libraries(Optimization ${Boost_CHRONO_LIBRARY})
target_link_libraries(Optimization ${Boost_SYSTEM_LIBRARY})

if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")
    if(MSVC)
      if(${BUILD_SHARED_LIB} STREQUAL "1")
          target_link_libraries(Optimization mkl_intel_ilp64_dll.lib)
          target_link_libraries(Optimization mkl_core_dll.lib)
          target_link_libraries(Optimization mkl_intel_thread_dll.lib)
          target_link_libraries(Optimization libiomp5md.lib)
      else()
          target_link_libraries(Optimization mkl_intel_ilp64.lib)
          target_link_libraries(Optimization mkl_core.lib)
          target_link_libraries(Optimization mkl_intel_thread.lib)
          target_link_libraries(Optimization libiomp5md.lib)
      endif()
  endif()
endif()

if(MSVC)
    add_custom_command(TARGET Optimization POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${IPOPT_LIB_DIR}/IpOptFSS.dll"
            ${BUILD_DIR}/Release)

    add_custom_command(TARGET Optimization POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${IPOPT_LIB_DIR}/IpOpt-vc8.dll"
            ${BUILD_DIR}/Release)
endif()
