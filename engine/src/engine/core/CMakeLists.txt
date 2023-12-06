target_sources(WindVulkan PRIVATE assert.cpp logger.cpp window.cpp appConfig.cpp)
target_include_directories(WindVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(memory)
add_subdirectory(renderer)