if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")
    add_definitions(-DUSE_MKL_LIB)
endif()

file(GLOB OPTIMIZER_FILES "*.hpp" "*.cpp")
add_library(optimizer ${LIBRARY_BUILD_TYPE} ${OPTIMIZER_FILES})

target_link_libraries(optimizer alglib)

if(${CMAKE_USE_MKL_LIB} STREQUAL "TRUE")

    if(${BUILD_SHARED_LIB} STREQUAL "1")
        target_link_libraries(optimizer mkl_intel_ilp64_dll.lib)
        target_link_libraries(optimizer mkl_core_dll.lib)
        target_link_libraries(optimizer mkl_intel_thread_dll.lib)
        target_link_libraries(optimizer libiomp5md.lib)
    else()
        target_link_libraries(optimizer mkl_intel_ilp64.lib)
        target_link_libraries(optimizer mkl_core.lib)
        target_link_libraries(optimizer mkl_intel_thread.lib)
        target_link_libraries(optimizer libiomp5md.lib)
    endif()
endif()