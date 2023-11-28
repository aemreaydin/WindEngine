#ifndef WINDENGINE_VULKANCONTEXT_HPP
#define WINDENGINE_VULKANCONTEXT_HPP

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
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANCONTEXT_HPP
