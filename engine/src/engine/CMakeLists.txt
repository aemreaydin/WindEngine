SET(THIRD_PARTY_LIBS Vulkan::Vulkan spdlog::spdlog)

add_library(WindVulkan SHARED core/logger.cpp core/assert.cpp)
target_link_libraries(WindVulkan PUBLIC ${THIRD_PARTY_LIBS})
target_compile_definitions(WindVulkan PUBLIC DLLEXPORT)