cmake_minimum_required(VERSION 3.10)
project(lib_engine)

add_library(engine STATIC
		private/Engine.cpp
		private/Camera.cpp
		private/EventHandler.cpp
		private/TestBoxDrawer.cpp)
target_include_directories(engine
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(engine iomanager globjects chunk)
target_link_libraries(engine PUBLIC iomanager glm globjects chunk)
target_compile_options(engine PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)