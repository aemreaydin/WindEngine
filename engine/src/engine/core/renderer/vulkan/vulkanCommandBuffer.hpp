#include "vulkanHandle.hpp"

#ifndef WINDENGINE_VULKANCOMMANDBUFFER_HPP
#define WINDENGINE_VULKANCOMMANDBUFFER_HPP

namespace WindEngine::Core::Render
{

struct VulkanCommandBuffer
{
    vk::CommandBuffer commandBuffer;

    void Allocate( const vk::Device& device, const vk::CommandPool& pool, bool bIsPrimary );
    void Free( const vk::Device& device, const vk::CommandPool& pool ) const;

    void Begin() const;
    void End() const;
    void Reset() const;
};

}  // namespace WindEngine::Core::Render
#endif  // WINDENGINE_VULKANCOMMANDBUFFER_HPP
