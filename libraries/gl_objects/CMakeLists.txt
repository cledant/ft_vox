cmake_minimum_required(VERSION 3.10)
project(lib_gl_object)

add_library(globjects STATIC
		private/GLShader.cpp)
target_include_directories(globjects
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(globjects glad glm)
target_link_libraries(globjects PUBLIC glad glm)
target_compile_options(globjects PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)