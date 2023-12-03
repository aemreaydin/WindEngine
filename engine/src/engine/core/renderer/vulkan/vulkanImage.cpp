#include "vulkanImage.hpp"
#include "vulkanDevice.hpp"

namespace WindEngine::Core::Render
{

VulkanImage::VulkanImage( const VulkanDevice* pDevice, vk::AllocationCallbacks* allocator )
  : VulkanHandle( pDevice, allocator )
{
}

void VulkanImage::Initialize( const VulkanImageCreateInfo& createInfo )
{
    const auto imageInfo = vk::ImageCreateInfo {
        .imageType = createInfo.imageType,
        .format = createInfo.format,
        .extent = createInfo.extent,
        .mipLevels = 5,
        .arrayLayers = 1,
        .samples = vk::SampleCountFlagBits::e1,
        .tiling = createInfo.tiling,
        .usage = createInfo.usage,
        .sharingMode = vk::SharingMode::eExclusive,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = vk::ImageLayout::eUndefined,
    };
    image = _device->device.createImage( imageInfo, _allocator );

    const auto memoryRequirements = _device->device.getImageMemoryRequirements( image );
    const auto memoryIndex =
      _device->physicalDeviceInfo.FindMemoryIndex( memoryRequirements.memoryTypeBits, createInfo.memoryFlags );
    const auto allocateInfo =
      vk::MemoryAllocateInfo { .allocationSize = memoryRequirements.size, .memoryTypeIndex = memoryIndex };
    deviceMemory = _device->device.allocateMemory( allocateInfo, _allocator );

    _device->device.bindImageMemory( image, deviceMemory, 0 );

    const auto imageViewInfo =
      vk::ImageViewCreateInfo { .image = image,
                                .viewType = vk::ImageViewType::e2D,
                                .format = createInfo.format,
                                .components = {},
                                .subresourceRange = vk::ImageSubresourceRange { .aspectMask = createInfo.aspectFlags,
                                                                                .baseMipLevel = 0,
                                                                                .levelCount = 1,
                                                                                .baseArrayLayer = 0,
                                                                                .layerCount = 1 } };
    imageView = _device->device.createImageView( imageViewInfo, _allocator );
}

void VulkanImage::Destroy()
{
    _device->device.destroy( imageView, _allocator );
    _device->device.freeMemory( deviceMemory, _allocator );
    _device->device.destroy( image, _allocator );
}

}  // namespace WindEngine::Core::Render
