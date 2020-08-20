cmake_minimum_required(VERSION 3.10)
project(lib_skybox)

add_library(skybox STATIC
        private/Skybox.cpp)
target_include_directories(skybox
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(skybox glad glm globjects)
target_link_libraries(skybox PUBLIC glad glm globjects)
set_target_properties(skybox PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(skybox PRIVATE -Wall -Wextra -Werror)