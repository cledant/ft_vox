cmake_minimum_required(VERSION 3.10)
project(lib_chunk)

add_library(chunk STATIC
        private/ChunkManager.cpp private/Chunk.cpp)
target_include_directories(chunk
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(chunk glad glm globjects noise)
target_link_libraries(chunk PUBLIC glad glm globjects noise pthread)
set_target_properties(chunk PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(chunk PRIVATE -Wall -Wextra -Werror)