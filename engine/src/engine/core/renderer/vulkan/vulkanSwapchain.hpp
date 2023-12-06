#ifndef WINDENGINE_VULKANSWAPCHAIN_HPP
#define WINDENGINE_VULKANSWAPCHAIN_HPP

#include "defines.hpp"
#include "vulkanHandle.hpp"
#include "vulkanImage.hpp"
#include <vector>
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{
struct VulkanDevice;

struct VulkanSwapchain : public VulkanHandle
{
    vk::SwapchainKHR swapchain {};
    std::vector<vk::Image> images {};
    std::vector<vk::ImageView> imageViews {};
    U32 imageCount {};
    VulkanImage depthImage;

    vk::SurfaceFormatKHR imageFormat {};

    VulkanSwapchain( VulkanDevice& device, vk::AllocationCallbacks* allocator );

    void Initialize( const vk::SurfaceKHR& surface, U32 width, U32 height );
    void Destroy() override;

    void Recreate( const vk::SurfaceKHR& surface, U32 width, U32 height );

    // Render Functionality
    auto AcquireNextImage( U64 timeout, vk::Semaphore semaphore, vk::Fence fence ) -> std::optional<U32>;
    auto Present( vk::Semaphore semaphore, U32 imageIndex ) -> bool;

private:
    void Create( const vk::SurfaceKHR& surface, U32 width, U32 height );
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANSWAPCHAIN_HPP
