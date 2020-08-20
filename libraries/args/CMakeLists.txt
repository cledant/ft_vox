cmake_minimum_required(VERSION 3.10)
project(lib_arg_parsing)

add_library(arg_parsing STATIC
        private/ArgsParsing.cpp)
target_include_directories(arg_parsing
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private)
set_target_properties(arg_parsing PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(arg_parsing PRIVATE -Wall -Wextra -Werror)