cmake_minimum_required(VERSION 3.10)
project(ft_vox)

#Detecting + setting global LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_output)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND lto_supported)
    message("Compiling with LTO enabled")
    cmake_policy(SET CMP0069 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else ()
    message("LTO not supported")
endif ()

#Setting global C/CXX compile options
set(FT_VOX_OPTI_TYPE NATIVE CACHE STRING "")

if (${FT_VOX_OPTI_TYPE} STREQUAL NATIVE)
    message("Build binary for current computer arch")
    set(CMAKE_CXX_FLAGS "-march=native -pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
elseif (${FT_VOX_OPTI_TYPE} STREQUAL AVX2)
    message("Build binary with AVX2 profile")
    set(CMAKE_CXX_FLAGS "-march=core2 -mtune=haswell \
    -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 \
    -mavx -mavx2 -mbmi -mbmi2 -mf16c -mfma -mfsgsbase \
    -mlzcnt -mmovbe -mpclmul -mpopcnt -maes \
    -mrdrnd  -pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
elseif (${FT_VOX_OPTI_TYPE} STREQUAL SSE4)
    message("Build binary with SSE4 profile")
    set(CMAKE_CXX_FLAGS "-march=core2 -mtune=nehalem \
    -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 \
    -mpopcnt -pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
else ()
    message("No arch optimization selected")
    set(CMAKE_CXX_FLAGS "-pipe")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
endif ()

#Setup glm
add_library(glm INTERFACE IMPORTED)
set_target_properties(glm PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

#Setup Glad
set(GLAD_API "gl=4.1" CACHE STRING "" FORCE)
set(GLAD_EXTENSIONS "GL_EXT_texture_filter_anisotropic" CACHE STRING "" FORCE)
option(GLAD_INSTALL "" OFF)
option(BUILD_SHARED_LIBS "" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad ${CMAKE_BINARY_DIR}/external/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
option(BUILD_SHARED_LIBS "" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw ${CMAKE_BINARY_DIR}/external/glfw/build)

#Setup freetype
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/freetype ${CMAKE_BINARY_DIR}/external/freetype2/build)

#Projet libs
add_subdirectory(libraries/io_manager)
add_subdirectory(libraries/engine)
add_subdirectory(libraries/gl_objects)
add_subdirectory(libraries/chunk)
add_subdirectory(libraries/skybox)
add_subdirectory(libraries/ui)
add_subdirectory(libraries/args)
add_subdirectory(libraries/noise)

#Main binary
add_executable(ft_vox binary/private/ft_vox.cpp)
target_link_libraries(ft_vox engine iomanager arg_parsing)
add_dependencies(ft_vox engine iomanager arg_parsing)
set_target_properties(ft_vox PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(ft_vox PRIVATE -Wall -Wextra -Werror)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ressources DESTINATION ${CMAKE_BINARY_DIR})