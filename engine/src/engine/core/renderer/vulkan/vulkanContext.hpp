#ifndef WINDENGINE_VULKANCONTEXT_HPP
#define WINDENGINE_VULKANCONTEXT_HPP

#include "vulkanDevice.hpp"
#include "vulkanInstance.hpp"
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanContext
{
    SDL_Window* window { nullptr };
    vk::AllocationCallbacks* allocator { nullptr };
    vk::SurfaceKHR surface { nullptr };

    VulkanInstance instance;
    VulkanDevice device;

    auto Initialize( const char* applicationName ) -> bool;
    void Shutdown();

    auto GetInstance() const -> vk::Instance;
    auto GetDevice() const -> vk::Device;
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANCONTEXT_HPP
