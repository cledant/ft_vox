cmake_minimum_required(VERSION 3.10)
project(lib_binome)

add_library(binome STATIC
		private/Perlin.cpp)
target_include_directories(binome
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(binome glm)
target_link_libraries(binome PUBLIC glm)
target_compile_options(binome PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)