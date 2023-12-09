#include "vulkanBuffer.hpp"
#include "utils.hpp"

namespace WindEngine::Core::Render
{

VulkanBuffer::VulkanBuffer( VulkanDevice& device, vk::AllocationCallbacks* allocator )
  : VulkanHandle( &device, allocator )
{
}

void VulkanBuffer::Initialize( const VulkanBufferCreateInfo& vulkanBufferInfo )
{
    auto sharingMode = vk::SharingMode::eExclusive;
    if ( !vulkanBufferInfo.queueIndices.empty() && G_IS_VECTOR_UNIQUE( vulkanBufferInfo.queueIndices ) )
    {
        sharingMode = vk::SharingMode::eConcurrent;
    }

    const auto bufferInfo =
      vk::BufferCreateInfo { .size = vulkanBufferInfo.size,
                             .usage = vulkanBufferInfo.usageFlags,
                             .sharingMode = sharingMode,
                             .queueFamilyIndexCount = ToU32( vulkanBufferInfo.queueIndices.size() ),
                             .pQueueFamilyIndices = vulkanBufferInfo.queueIndices.data() };
    buffer = _device->device.createBuffer( bufferInfo, _allocator );

    _memoryRequirements = _device->device.getBufferMemoryRequirements( buffer );
    const auto memoryIndex =
      _device->physicalDeviceInfo.FindMemoryIndex( _memoryRequirements.memoryTypeBits, vulkanBufferInfo.memoryFlags );
    const auto memoryInfo =
      vk::MemoryAllocateInfo { .allocationSize = _memoryRequirements.size, .memoryTypeIndex = memoryIndex };
    deviceMemory = _device->device.allocateMemory( memoryInfo, _allocator );

    _device->device.bindBufferMemory( buffer, deviceMemory, 0 );
}

void VulkanBuffer::Destroy()
{
    _device->device.destroy( buffer, _allocator );
    _device->device.free( deviceMemory, _allocator );
}

}  // namespace WindEngine::Core::Render
