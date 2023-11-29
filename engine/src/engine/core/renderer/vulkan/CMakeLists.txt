target_sources(WindVulkan PRIVATE vulkanRenderer.cpp instance.cpp vulkanDevice.cpp)
target_include_directories(WindVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})