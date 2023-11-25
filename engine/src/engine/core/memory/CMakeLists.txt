target_sources(WindVulkan PRIVATE linearAllocator.cpp cAllocator.cpp allocationManager.cpp)
target_include_directories(WindVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
