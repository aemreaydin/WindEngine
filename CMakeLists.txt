cmake_minimum_required(VERSION 3.26)

SET(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_CXX_STANDARD 20)

project(WindEngine VERSION 0.0.1 DESCRIPTION "A Game Engine" LANGUAGES CXX)

find_package(Vulkan REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(engine)
add_subdirectory(test)