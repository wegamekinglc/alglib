if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEF:${CMAKE_CURRENT_SOURCE_DIR}/alglib_symbols.def")
endif()

file(GLOB_RECURSE ALGLIB_FILES "*.h" "*.cpp")
add_library(alglib ${LIBRARY_BUILD_TYPE} ${ALGLIB_FILES})