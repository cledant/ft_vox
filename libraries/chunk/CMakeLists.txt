cmake_minimum_required(VERSION 3.10)
project(lib_chunk)

add_library(chunk STATIC
		private/ChunkManager.cpp private/Chunk.cpp)
target_include_directories(chunk
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(chunk glad glm globjects)
target_link_libraries(chunk PUBLIC glad glm globjects)
target_compile_options(chunk PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)