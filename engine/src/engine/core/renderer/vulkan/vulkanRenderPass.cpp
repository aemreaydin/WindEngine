#include "vulkanRenderPass.hpp"
#include "vulkanDevice.hpp"

namespace WindEngine::Core::Render
{

VulkanRenderPass::VulkanRenderPass( VulkanDevice& device, vk::AllocationCallbacks* allocator )
  : VulkanHandle( &device, allocator )
{
}

void VulkanRenderPass::Initialize( const vk::Format& imageFormat, const vk::Format& depthFormat )
{
    const auto attachments =
      std::vector<vk::AttachmentDescription> { vk::AttachmentDescription {
                                                 .format = imageFormat,
                                                 .samples = vk::SampleCountFlagBits::e1,
                                                 .loadOp = vk::AttachmentLoadOp::eClear,
                                                 .storeOp = vk::AttachmentStoreOp::eStore,
                                                 .initialLayout = vk::ImageLayout::eUndefined,
                                                 .finalLayout = vk::ImageLayout::ePresentSrcKHR,
                                               },
                                               vk::AttachmentDescription {
                                                 .format = depthFormat,
                                                 .samples = vk::SampleCountFlagBits::e1,
                                                 .loadOp = vk::AttachmentLoadOp::eClear,
                                                 .storeOp = vk::AttachmentStoreOp::eDontCare,
                                                 .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                                                 .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                                                 .initialLayout = vk::ImageLayout::eUndefined,
                                                 .finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal,
                                               } };

    const auto colorReference =
      vk::AttachmentReference { .attachment = 0, .layout = vk::ImageLayout::eColorAttachmentOptimal };

    const auto depthStencilReference =
      vk::AttachmentReference { .attachment = 1, .layout = vk::ImageLayout::eDepthStencilAttachmentOptimal };

    const auto mainSubpassDescription = vk::SubpassDescription { .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
                                                                 .colorAttachmentCount = 1,
                                                                 .pColorAttachments = &colorReference,
                                                                 .pDepthStencilAttachment = &depthStencilReference };

    const auto mainSubpassDependencies = vk::SubpassDependency {
        .srcSubpass = vk::SubpassExternal,
        .srcStageMask =
          vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
        .dstStageMask =
          vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
        .srcAccessMask = {},
        .dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite |
                         vk::AccessFlagBits::eDepthStencilAttachmentWrite,
    };

    const auto renderPassInfo = vk::RenderPassCreateInfo {
        .attachmentCount = ToU32( attachments.size() ),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &mainSubpassDescription,
        .dependencyCount = 1,
        .pDependencies = &mainSubpassDependencies,
    };

    _mainRenderPass = _device->device.createRenderPass( renderPassInfo, _allocator );
}

void VulkanRenderPass::Destroy()
{
    _device->device.destroy( _mainRenderPass, _allocator );
}

void VulkanRenderPass::BeginRenderPass( const vk::CommandBuffer& commandBuffer, const vk::Framebuffer& framebuffer,
                                        const vk::Rect2D& renderArea, const vk::ClearColorValue& colorValue,
                                        const vk::ClearDepthStencilValue& depthStencilValue )
{
    auto clearValues = std::array<vk::ClearValue, 2> { vk::ClearValue { .color = colorValue },
                                                       vk::ClearValue { .depthStencil = depthStencilValue } };
    const auto beginInfo = vk::RenderPassBeginInfo {
        .renderPass = _mainRenderPass,
        .framebuffer = framebuffer,
        .renderArea = renderArea,
        .clearValueCount = ToU32( clearValues.size() ),
        .pClearValues = clearValues.data(),
    };

    _commandBuffer = commandBuffer;
    _commandBuffer.beginRenderPass( beginInfo, vk::SubpassContents::eInline );
}

void VulkanRenderPass::EndRenderPass()
{
    _commandBuffer.endRenderPass();
}

auto VulkanRenderPass::GetRenderPass() const -> vk::RenderPass
{
    return _mainRenderPass;
};

}  // namespace WindEngine::Core::Render
