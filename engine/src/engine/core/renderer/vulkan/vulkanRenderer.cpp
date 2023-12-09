#include "vulkanRenderer.hpp"
#include "vulkanInstance.hpp"
#include <SDL_vulkan.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace WindEngine::Core::Render
{

auto VulkanRenderer::Initialize( const AppConfig& config ) -> bool
{
    // Initialize the vulkan-hpp dispatcher
    const vk::DynamicLoader dynamicLoader;
    auto vkGetInstanceProcAddr = dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

    return _context.Initialize( config.appName.c_str(), config.width, config.height );
}

void VulkanRenderer::Shutdown()
{
    _context.Shutdown();

    SDL_DestroyWindow( _context.window );
}

auto VulkanRenderer::BeginFrame( AppState& state ) -> bool
{
    auto frame = _context.GetCurrentFrame();
    const auto result = _context.GetDevice().waitForFences( frame.fence, VK_TRUE, std::numeric_limits<U64>::max() );
    if ( result != vk::Result::eTimeout && result != vk::Result::eSuccess )
    {
        WIND_ERROR( "vkWaitForFences Error" )
        return false;
    }

    if ( state.shouldResize )
    {
        _context.GetDevice().waitIdle();
        int width {};
        int height {};
        SDL_GetWindowSize( _context.window, &width, &height );
        _context.swapchain.Recreate( _context.surface, ToU32( width ), ToU32( height ) );
        _context.RecreateFramebuffers( width, height );

        state.shouldResize = false;
        return false;
    }

    auto optionalImageIndex = _context.swapchain.AcquireNextImage( 0, frame.presentSemaphore, nullptr );
    if ( !optionalImageIndex.has_value() )
    {
        _context.swapchain.Recreate( _context.surface, _context.framebufferWidth, _context.framebufferHeight );
        return false;
    }

    _context.GetDevice().resetFences( frame.fence );

    _context.imageIndex = *optionalImageIndex;
    const auto& cmd = _context.graphicsCommandBuffers[_context.imageIndex];
    const auto& framebuffer = _context.framebuffers[_context.imageIndex];
    cmd.Begin();

    const auto viewportInfo = vk::Viewport { .x = 0.0F,
                                             .y = -static_cast<F32>( _context.framebufferHeight ),
                                             .width = static_cast<F32>( _context.framebufferWidth ),
                                             .height = static_cast<F32>( _context.framebufferHeight ),
                                             .minDepth = 0.0F,
                                             .maxDepth = 1.0F };
    cmd.commandBuffer.setViewport( 0, 1, &viewportInfo );

    const auto scissor =
      vk::Rect2D { { static_cast<I32>( _context.framebufferWidth ), static_cast<I32>( _context.framebufferHeight ) },
                   { 0, 0 } };
    cmd.commandBuffer.setScissor( 0, scissor );

    const vk::Rect2D rect2D { { 0, 0 }, { _context.framebufferWidth, _context.framebufferHeight } };
    const vk::ClearColorValue colorValue { .float32 = { { 0.0F, 0.5F, 0.0F, 1.0F } } };
    const vk::ClearDepthStencilValue depthStencilValue { .depth = 1.F, .stencil = 0 };
    _context.renderPass.BeginRenderPass( cmd.commandBuffer, framebuffer, rect2D, colorValue, depthStencilValue );
    cmd.commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _context.graphicsPipeline.pipeline );

    return true;
}

auto VulkanRenderer::EndFrame( [[maybe_unused]] AppState& state ) -> bool
{
    auto frame = _context.GetCurrentFrame();
    const auto& cmd = _context.graphicsCommandBuffers[_context.imageIndex];
    // const auto& framebuffer = _context.framebuffers[_context.imageIndex];

    cmd.commandBuffer.bindVertexBuffers( 0, _context.triangleBuffer.buffer, { 0 } );
    cmd.commandBuffer.draw( ToU32( 3 ), 1, 0, 0 );

    _context.renderPass.EndRenderPass( cmd.commandBuffer );
    cmd.End();

    const auto dstStageMask = std::vector<vk::PipelineStageFlags> { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    const auto submitInfo = vk::SubmitInfo { .waitSemaphoreCount = 1,
                                             .pWaitSemaphores = &frame.presentSemaphore,
                                             .pWaitDstStageMask = dstStageMask.data(),
                                             .commandBufferCount = 1,
                                             .pCommandBuffers = &cmd.commandBuffer,
                                             .signalSemaphoreCount = 1,
                                             .pSignalSemaphores = &frame.renderSemaphore };
    _context.device.graphicsQueue.submit( submitInfo, frame.fence );

    if ( !_context.swapchain.Present( frame.renderSemaphore, _context.imageIndex ) )
    {
        _context.GetDevice().waitIdle();
        int width {};
        int height {};
        SDL_GetWindowSize( _context.window, &width, &height );
        _context.swapchain.Recreate( _context.surface, ToU32( width ), ToU32( height ) );
        _context.RecreateFramebuffers( width, height );
        return false;
    }
    WIND_TRACE( "Presented Frame {}", _context.currentFrame )
    ++_context.currentFrame;
    return true;
}

void VulkanRenderer::Resize( [[maybe_unused]] U16 width, [[maybe_unused]] U16 height )
{
}

}  // namespace WindEngine::Core::Render
