#ifndef WINDENGINE_VULKANCONTEXT_HPP
#define WINDENGINE_VULKANCONTEXT_HPP

#include "vulkanDevice.hpp"
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanContext
{
    vk::Instance instance { nullptr };
    vk::AllocationCallbacks* allocator { nullptr };
#if defined( _DBG )
    vk::DebugUtilsMessengerEXT debugMessenger { nullptr };
#endif
    vk::SurfaceKHR surface { nullptr };
    VulkanDevice device;
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANCONTEXT_HPP
