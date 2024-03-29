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
add_dependencies(engine ui iomanager globjects chunk skybox)
target_link_libraries(engine PUBLIC ui iomanager glm globjects chunk skybox arg_parsing)
set_target_properties(engine PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(engine PRIVATE -Wall -Wextra -Werror)