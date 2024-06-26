cmake_minimum_required(VERSION 3.28)
project(WindEngine VERSION 0.0.1 DESCRIPTION "A Graphics Engine" LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

option(USE_CPPCHECK "Use CppCheck?" OFF)
option(USE_CLANG_TIDY "Use ClangTidy?" OFF)
option(USE_IWYU "Use Include What You Use?" OFF)
if (${USE_CPPCHECK})
  set(CMAKE_CXX_CPPCHECK "cppcheck" --check-level=exhaustive --quiet --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=missingInclude)
endif()
if (${USE_CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif()
if (${USE_IWYU})
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use")
endif()

find_package(Vulkan REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sdl2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/Wall /WX)
endif ()

add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(test)
