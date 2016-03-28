if(WIN32)
    include_directories("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/include")
    LINK_DIRECTORIES("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/lib/x64")
else()
    include_directories(/usr/local/cuda-7.5/include)
    LINK_DIRECTORIES(/usr/local/cuda-7.5/lib64)
endif()

include_directories(${CMAKE_SOURCE_DIR}/optimizer)

file(GLOB OPTIMIZERTEST_FILES "*.hpp" "*.cpp")
add_executable(optimizerTest ${OPTIMIZERTEST_FILES})
target_link_libraries(optimizerTest optimizer)