cmake_minimum_required(VERSION 3.10)
project(lib_gl_object)

add_library(globjects STATIC
        private/GLShader.cpp
        private/GLTexture2D.cpp)
target_include_directories(globjects
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_SOURCE_DIR}/external/stb)
add_dependencies(globjects glad glm freetype)
target_link_libraries(globjects PUBLIC glad glm freetype)
set_target_properties(globjects PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(globjects PRIVATE -Wall -Wextra -Werror)