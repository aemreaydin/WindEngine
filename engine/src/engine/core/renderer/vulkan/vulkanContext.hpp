#ifndef WINDENGINE_VULKANCONTEXT_HPP
#define WINDENGINE_VULKANCONTEXT_HPP

#include "vulkanDevice.hpp"
#include "vulkanInstance.hpp"
#include "vulkanRenderPass.hpp"
#include "vulkanSwapchain.hpp"
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct FrameData
{
    U32 imageIndex;
};

struct VulkanContext
{
    SDL_Window* window { nullptr };
    vk::AllocationCallbacks* allocator { nullptr };
    vk::SurfaceKHR surface { nullptr };

    VulkanInstance instance {};
    VulkanDevice device {};
    VulkanSwapchain swapchain;
    VulkanRenderPass renderPass;

    U32 framebufferWidth {};
    U32 framebufferHeight {};

    FrameData frameData {};

    VulkanContext();

    auto Initialize( const char* applicationName ) -> bool;
    void Shutdown();

    auto GetInstance() const -> const vk::Instance&;
    auto GetDevice() const -> const vk::Device&;
    auto GetSwapchain() const -> const vk::SwapchainKHR&;
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANCONTEXT_HPP
