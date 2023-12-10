cmake_minimum_required(VERSION 3.26)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
SET(CMAKE_CXX_STANDARD 20)

project(WindEngine VERSION 0.0.1 DESCRIPTION "A Game Engine" LANGUAGES CXX)

find_package(Vulkan REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sdl2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #    add_compile_options(-Weverything -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-weak-vtables -Wno-padded
    #            -Wno-switch-enum -Wno-global-constructors -Wno-exit-time-destructors)
    add_compile_options(-Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/Wall /WX)
endif ()


add_subdirectory(engine)
add_subdirectory(test)