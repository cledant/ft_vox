cmake_minimum_required(VERSION 3.10)
project(lib_iomanager)

add_library(iomanager STATIC
		private/IOManager.cpp)
target_include_directories(iomanager
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(iomanager glad glfw)
target_link_libraries(iomanager PUBLIC glad glfw)
target_compile_options(iomanager PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)