#ifndef WINDENGINE_VULKANHANDLE_HPP
#define WINDENGINE_VULKANHANDLE_HPP

#include "defines.hpp"
#include "vulkanDevice.hpp"
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanHandle
{
    VulkanHandle( const VulkanDevice* device, vk::AllocationCallbacks* allocator )
      : _device( device ), _allocator( allocator )
    {
    }

    virtual ~VulkanHandle() = default;

    virtual void Destroy() = 0;

protected:
    const VulkanDevice* _device { nullptr };
    vk::AllocationCallbacks* _allocator { nullptr };
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANHANDLE_HPP
