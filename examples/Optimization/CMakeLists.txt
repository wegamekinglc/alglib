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
    target_link_libraries(Optimization IpOpt-vc10)
else()
    target_link_libraries(Optimization ipopt)
    target_link_libraries(Optimization boost_system)
    target_link_libraries(Optimization boost_chrono)
endif()

if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")
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

if(MSVC)
    add_custom_command(TARGET Optimization POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${IPOPT_LIB_DIR}/IpOptFSS.dll"
            ${BUILD_DIR}/Release)

    add_custom_command(TARGET Optimization POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${IPOPT_LIB_DIR}/IpOpt-vc10.dll"
            ${BUILD_DIR}/Release)
endif()