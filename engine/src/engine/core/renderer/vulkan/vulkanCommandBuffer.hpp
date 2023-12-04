#include "vulkanHandle.hpp"

#ifndef WINDENGINE_VULKANCOMMANDBUFFER_HPP
#define WINDENGINE_VULKANCOMMANDBUFFER_HPP

namespace WindEngine::Core::Render
{

struct VulkanCommandBuffer
{
    vk::CommandBuffer commandBuffer;

    void Allocate( const vk::Device& device, const vk::CommandPool& pool, bool bIsPrimary );
    void Free( const vk::Device& device, const vk::CommandPool& pool );

    void Begin();
    void End();
    void Reset();
};

}  // namespace WindEngine::Core::Render
#endif  // WINDENGINE_VULKANCOMMANDBUFFER_HPP
