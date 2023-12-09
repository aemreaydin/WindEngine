#ifndef WINDENGINE_VULKANCONTEXT_HPP
#define WINDENGINE_VULKANCONTEXT_HPP

#include "vulkanCommandBuffer.hpp"
#include "vulkanDevice.hpp"
#include "vulkanInstance.hpp"
#include "vulkanRenderPass.hpp"
#include "vulkanSwapchain.hpp"
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct Frame
{
    vk::Semaphore renderSemaphore;
    vk::Semaphore presentSemaphore;
    vk::Fence fence;
};

constexpr U32 kFramesInFlight = 2;  // TODO Configurable?

struct VulkanContext
{
    SDL_Window* window { nullptr };
    vk::AllocationCallbacks* allocator { nullptr };
    vk::SurfaceKHR surface { nullptr };

    VulkanInstance instance {};
    VulkanDevice device {};
    VulkanSwapchain swapchain;
    VulkanRenderPass renderPass;
    std::vector<VulkanCommandBuffer> graphicsCommandBuffers {};
    vk::CommandPool graphicsCommandPool;

    std::vector<vk::Framebuffer> framebuffers {};
    U32 framebufferWidth {};
    U32 framebufferHeight {};

    USize imageIndex {};
    USize currentFrame {};
    std::array<Frame, kFramesInFlight> frames {};

    VulkanContext();

    auto Initialize( const char* applicationName, U32 width, U32 height ) -> bool;
    void Shutdown();

    [[nodiscard]] auto GetInstance() const -> const vk::Instance&;
    [[nodiscard]] auto GetDevice() const -> const vk::Device&;
    [[nodiscard]] auto GetSwapchain() const -> const vk::SwapchainKHR&;
    [[nodiscard]] auto GetCurrentFrame() -> Frame&;

    void RecreateFramebuffers( U32 width, U32 height );
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANCONTEXT_HPP
