cmake_minimum_required(VERSION 3.10)

set(CMAKE_CUDA_HOST_COMPILER /usr/bin/g++-15)

project(LCP1 LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(a
    main.cpp
    ${CAMERA_SRCS}
    ${CORE_SRCS}
    ${LENS_SRCS}
    ${SAMPLE_SRCS}
    ${SCENE_SRCS}
    ${FILM_SRCS}
    ${UTIL_SRCS}
)

add_executable(cuda_smoke
    a.cu
)

set_target_properties(cuda_smoke PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)
