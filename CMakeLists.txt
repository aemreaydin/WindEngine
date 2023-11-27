cmake_minimum_required(VERSION 3.26)

SET(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_COMPILE_WARNING_AS_ERROR ON)

project(WindEngine VERSION 0.0.1 DESCRIPTION "A Game Engine" LANGUAGES CXX)

find_package(Vulkan REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sdl2 CONFIG REQUIRED)

MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/Wall /WX)
endif ()

add_subdirectory(engine)
add_subdirectory(test)