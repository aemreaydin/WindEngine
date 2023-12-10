#ifndef WINDENGINE_VULKANBUFFER_HPP
#define WINDENGINE_VULKANBUFFER_HPP

#include "vulkanHandle.hpp"

namespace WindEngine::Core::Render
{

struct VulkanBufferCreateInfo
{
    vk::DeviceSize size {};
    vk::BufferUsageFlags usageFlags {};
    std::vector<U32> queueIndices {};
    vk::MemoryPropertyFlags memoryFlags {};
};

struct VulkanBuffer : public VulkanHandle
{
    vk::Buffer buffer {};
    vk::DeviceMemory deviceMemory {};

    VulkanBuffer( VulkanDevice& device, vk::AllocationCallbacks* allocator );

    void Initialize( const VulkanBufferCreateInfo& vulkanBufferInfo );
    void Destroy() override;

    template <typename T> void MapMemory( const std::vector<T>& vec )
    {
        const auto size = sizeof( T ) * vec.size();
        auto* pData = static_cast<uint8_t*>( _device->device.mapMemory( deviceMemory, 0, _memoryRequirements.size ) );
        memcpy( pData, vec.data(), size );
        _device->device.unmapMemory( deviceMemory );
    }

private:
    vk::MemoryRequirements _memoryRequirements {};
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANBUFFER_HPP
