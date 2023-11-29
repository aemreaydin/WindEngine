#ifndef WINDENGINE_VULKANINSTANCE_HPP
#define WINDENGINE_VULKANINSTANCE_HPP

#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanInstance
{
    vk::Instance instance;
#if defined( _DBG )
    vk::DebugUtilsMessengerEXT debugMessenger { nullptr };
#endif

    void Initialize( const char* applicationName, SDL_Window* window, vk::AllocationCallbacks* allocator );
    void Shutdown( vk::AllocationCallbacks* allocator );
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANINSTANCE_HPP
