#include "vulkanCommandBuffer.hpp"

namespace WindEngine::Core::Render
{

void VulkanCommandBuffer::Allocate( const vk::Device& device, const vk::CommandPool& pool, bool bIsPrimary )
{
    const auto allocateInfo = vk::CommandBufferAllocateInfo {
        .commandPool = pool,
        .level = bIsPrimary ? vk::CommandBufferLevel::ePrimary : vk::CommandBufferLevel::eSecondary,
        .commandBufferCount = 1,
    };
    // TODO allocator
    commandBuffer = device.allocateCommandBuffers( allocateInfo ).front();
}

void VulkanCommandBuffer::Free( const vk::Device& device, const vk::CommandPool& pool )
{
    device.freeCommandBuffers( pool, commandBuffer );
}

void VulkanCommandBuffer::Begin()
{
    const auto beginInfo = vk::CommandBufferBeginInfo {
        .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,  // TODO Add rest
    };
    commandBuffer.begin( beginInfo );
}

void VulkanCommandBuffer::End()
{
    commandBuffer.end();
}

void VulkanCommandBuffer::Reset()
{
    commandBuffer.reset();
}

}  // namespace WindEngine::Core::Render
