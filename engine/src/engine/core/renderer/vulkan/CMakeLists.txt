target_sources(WindVulkan PRIVATE
        vulkanRenderer.cpp
        vulkanContext.cpp
        vulkanInstance.cpp
        vulkanDevice.cpp
        vulkanSwapchain.cpp
        vulkanImage.cpp
        vulkanRenderPass.cpp
        vulkanCommandBuffer.cpp
        vulkanPipeline.cpp
        vulkanBuffer.cpp
)
target_include_directories(WindVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})