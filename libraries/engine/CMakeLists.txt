cmake_minimum_required(VERSION 3.10)
project(lib_engine)

add_library(engine STATIC
		private/Engine.cpp)
target_include_directories(engine
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(engine iomanager)
target_link_libraries(engine PUBLIC iomanager)
target_compile_options(engine PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)