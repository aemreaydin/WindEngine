#ifndef WINDENGINE_VULKANCONTEXT_HPP
#define WINDENGINE_VULKANCONTEXT_HPP

#include "vulkanDevice.hpp"
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanContext
{
    SDL_Window* window { nullptr };
    vk::Instance instance { nullptr };
    vk::AllocationCallbacks* allocator { nullptr };
#if defined( _DBG )
    vk::DebugUtilsMessengerEXT debugMessenger { nullptr };
#endif
    vk::SurfaceKHR surface { nullptr };
    VulkanDevice device;

    auto Initialize( const char* applicationName ) -> bool;
    void Shutdown();
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANCONTEXT_HPP
