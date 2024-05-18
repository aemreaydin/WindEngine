#ifndef WINDENGINE_VULKANRENDERPASS_HPP
#define WINDENGINE_VULKANRENDERPASS_HPP

#include "vulkanHandle.hpp"
#include <glm/glm.hpp>

namespace WindEngine::Core::Render
{

struct VulkanDevice;

struct VulkanRenderPass : public VulkanHandle
{
    VulkanRenderPass( VulkanDevice& device, vk::AllocationCallbacks* allocator );

    void Initialize( const vk::Format& imageFormat, const vk::Format& depthFormat );
    void Destroy() override;

    void BeginRenderPass( const vk::CommandBuffer& commandBuffer, const vk::Framebuffer& framebuffer,
                          const vk::Rect2D& renderArea, const vk::ClearColorValue& colorValue,
                          const vk::ClearDepthStencilValue& depthStencilValue );
    void EndRenderPass();

    [[nodiscard]] auto GetRenderPass() const -> vk::RenderPass;

private:
    vk::CommandBuffer _commandBuffer;
    vk::RenderPass _mainRenderPass;
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANRENDERPASS_HPP
