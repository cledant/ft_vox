cmake_minimum_required(VERSION 3.10)
project(lib_win_manager)

#LIB GLFW
add_library(window_manager STATIC
		private/WindowManager.cpp)
target_include_directories(window_manager
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(window_manager glad glfw)
target_link_libraries(window_manager PUBLIC glad glfw)
target_compile_options(window_manager PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native)