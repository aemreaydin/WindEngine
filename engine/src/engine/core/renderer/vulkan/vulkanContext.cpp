#include "vulkanContext.hpp"
#include "logger.hpp"
#include "vulkanDevice.hpp"
#include "vulkanInstance.hpp"
#include "vulkanPipeline.hpp"

namespace WindEngine::Core::Render
{

VulkanContext::VulkanContext()
  : swapchain( device, allocator ), renderPass( device, allocator ), graphicsPipeline( device, allocator ),
    triangleBuffer( device, allocator )
{
}

auto VulkanContext::Initialize( const char* applicationName, U32 width, U32 height ) -> bool
{
    window = SDL_CreateWindow( "WindEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>( width ),
                               static_cast<int>( height ), SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );
    if ( window == nullptr )
    {
        WIND_FATAL( "Failed to create SDL window. {}", SDL_GetError() )
    }

    instance.Initialize( applicationName, window, allocator );

    if ( SDL_Vulkan_CreateSurface( window, GetInstance(), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) != SDL_TRUE )
    {
        WIND_FATAL( "Failed to create surface." )
    }

    if ( !device.Initialize( GetInstance(), surface, allocator ) )
    {
        return false;
    }

    swapchain.Initialize( surface, framebufferWidth, framebufferHeight );
    renderPass.Initialize( swapchain.imageFormat.format, device.depthFormat );
    graphicsPipeline.Initialize( renderPass.mainRenderPass );

    // TODO? Abstract away
    // Command pool and command buffers
    const auto poolInfo = vk::CommandPoolCreateInfo {
        .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        .queueFamilyIndex = device.indices.graphics,
    };
    graphicsCommandPool = GetDevice().createCommandPool( poolInfo, allocator );
    graphicsCommandBuffers.resize( swapchain.imageCount );
    for ( auto& buffer : graphicsCommandBuffers )
    {
        buffer.Allocate( GetDevice(), graphicsCommandPool, true );
    }
    WIND_INFO( "Allocated {} graphics command buffers.", graphicsCommandBuffers.size() )

    RecreateFramebuffers( width, height );

    // TODO Move from here
    for ( auto& frame : frames )
    {
        const auto fenceInfo = vk::FenceCreateInfo {
            .flags = vk::FenceCreateFlagBits::eSignaled,
        };
        frame.renderSemaphore = GetDevice().createSemaphore( {}, allocator );
        frame.presentSemaphore = GetDevice().createSemaphore( {}, allocator );
        frame.fence = GetDevice().createFence( fenceInfo, allocator );
    }

    triangleBuffer.Initialize(
      { .size = sizeof( Vertex ) * 3,
        .usageFlags = vk::BufferUsageFlagBits::eVertexBuffer,
        .memoryFlags = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible } );
    triangleBuffer.MapMemory( triangle );

    return true;
}

void VulkanContext::Shutdown()
{
    GetDevice().waitIdle();

    triangleBuffer.Destroy();

    for ( auto& frame : frames )
    {
        GetDevice().destroy( frame.renderSemaphore, allocator );
        GetDevice().destroy( frame.presentSemaphore, allocator );
        GetDevice().destroy( frame.fence, allocator );
    }

    for ( auto& buffer : framebuffers )
    {
        GetDevice().destroy( buffer, allocator );
    }

    for ( auto& buffer : graphicsCommandBuffers )
    {
        buffer.Free( GetDevice(), graphicsCommandPool );
    }
    GetDevice().destroy( graphicsCommandPool, allocator );

    graphicsPipeline.Destroy();
    renderPass.Destroy();
    swapchain.Destroy();

    device.Destroy();

    GetInstance().destroy( surface );

    instance.Destroy( allocator );

    SDL_DestroyWindow( window );
}

auto VulkanContext::GetInstance() const -> const vk::Instance&
{
    return instance.instance;
}

auto VulkanContext::GetDevice() const -> const vk::Device&
{
    return device.device;
}

auto VulkanContext::GetSwapchain() const -> const vk::SwapchainKHR&
{
    return swapchain.swapchain;
}

auto VulkanContext::GetCurrentFrame() -> Frame&
{
    return frames.at( currentFrame % kFramesInFlight );
}

void VulkanContext::RecreateFramebuffers( U32 width, U32 height )
{
    for ( auto& buffer : framebuffers )
    {
        GetDevice().destroy( buffer, allocator );
    }

    framebufferWidth = width;
    framebufferHeight = height;
    framebuffers.resize( swapchain.imageCount );
    for ( size_t ind = 0; ind < framebuffers.size(); ++ind )
    {
        auto attachments = std::vector<vk::ImageView> { swapchain.imageViews[ind], swapchain.depthImage.imageView };
        const auto framebufferInfo = vk::FramebufferCreateInfo { .renderPass = renderPass.mainRenderPass,
                                                                 .attachmentCount = ToU32( attachments.size() ),
                                                                 .pAttachments = attachments.data(),
                                                                 .width = framebufferWidth,
                                                                 .height = framebufferHeight,
                                                                 .layers = 1 };
        framebuffers[ind] = GetDevice().createFramebuffer( framebufferInfo, allocator );
    }
}

}  // namespace WindEngine::Core::Render
