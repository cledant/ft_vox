cmake_minimum_required(VERSION 3.10)
project(lib_ui)

add_library(ui STATIC
        private/UiFont.cpp
        private/UiTexture.cpp
        private/Ui.cpp)
target_include_directories(ui
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
add_dependencies(ui glad glm globjects noise)
target_link_libraries(ui PUBLIC glad glm globjects noise)
set_target_properties(ui PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(ui PRIVATE -Wall -Wextra -Werror)