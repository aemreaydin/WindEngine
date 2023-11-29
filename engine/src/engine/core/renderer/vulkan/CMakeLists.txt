target_sources(WindVulkan PRIVATE vulkanRenderer.cpp vulkanContext.cpp vulkanInstance.cpp vulkanDevice.cpp)
target_include_directories(WindVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})