target_sources(WindVulkan PRIVATE engine.cpp)
target_include_directories(WindVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
