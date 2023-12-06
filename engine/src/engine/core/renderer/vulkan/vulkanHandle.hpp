#ifndef WINDENGINE_VULKANHANDLE_HPP
#define WINDENGINE_VULKANHANDLE_HPP

#include "defines.hpp"
#include "vulkanDevice.hpp"
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanHandle
{
    VulkanHandle( VulkanDevice* device, vk::AllocationCallbacks* allocator )
      : _device( device ), _allocator( allocator )
    {
    }

    virtual ~VulkanHandle() = default;
    VulkanHandle( const VulkanHandle& ) = delete;
    VulkanHandle( const VulkanHandle&& ) = delete;
    auto operator=( const VulkanHandle& ) -> VulkanHandle& = delete;
    auto operator=( const VulkanHandle&& ) -> VulkanHandle& = delete;

    virtual void Destroy() = 0;

protected:
    VulkanDevice* _device { nullptr };
    vk::AllocationCallbacks* _allocator { nullptr };
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANHANDLE_HPP
