#ifndef WINDENGINE_VULKANIMAGE_HPP
#define WINDENGINE_VULKANIMAGE_HPP

#include "defines.hpp"
#include "vulkanHandle.hpp"
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanDevice;

struct VulkanImageCreateInfo
{
    vk::ImageAspectFlags aspectFlags {};
    vk::Extent3D extent {};
    vk::Format format {};
    vk::ImageType imageType {};
    vk::MemoryPropertyFlags memoryFlags {};
    vk::ImageTiling tiling {};
    vk::ImageUsageFlags usage;
};

struct VulkanImage : public VulkanHandle
{
    VulkanImage( const VulkanDevice* pDevice, vk::AllocationCallbacks* allocator );
    vk::Image image {};
    vk::ImageView imageView {};
    vk::DeviceMemory deviceMemory {};

    void Initialize( const VulkanImageCreateInfo& createInfo );
    void Destroy();
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANIMAGE_HPP
