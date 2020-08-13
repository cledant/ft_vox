cmake_minimum_required(VERSION 3.10)
project(lib_noise)

add_library(noise STATIC
        private/PerlinNoise.cpp
        private/NoiseUtils.cpp)
target_include_directories(noise
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(noise glm)
target_link_libraries(noise PUBLIC glm pthread)
target_compile_options(noise PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)