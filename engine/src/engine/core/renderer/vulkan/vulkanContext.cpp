#include "vulkanContext.hpp"
#include "logger.hpp"
#include "vulkanDevice.hpp"
#include "vulkanInstance.hpp"

namespace WindEngine::Core::Render
{

VulkanContext::VulkanContext() : swapchain( device, allocator ), renderPass( device, allocator )
{
}

auto VulkanContext::Initialize( const char* applicationName ) -> bool
{
    window = SDL_CreateWindow( "WindEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900,
                               SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );
    if ( window == nullptr )
    {
        WIND_FATAL( "Failed to create SDL window." )
    }

    instance.Initialize( applicationName, window, allocator );

    if ( !SDL_Vulkan_CreateSurface( window, GetInstance(), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) )
    {
        WIND_FATAL( "Failed to create surface." )
    }

    if ( !device.Initialize( GetInstance(), surface, allocator ) )
    {
        return false;
    }

    swapchain.Initialize( surface, framebufferWidth, framebufferHeight );
    renderPass.Initialize( swapchain.imageFormat.format, device.depthFormat );

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
    WIND_INFO( "Allocated {} graphics command buffers.", graphicsCommandBuffers.size() );

    return true;
}

void VulkanContext::Shutdown()
{
    for ( auto& buffer : graphicsCommandBuffers )
    {
        buffer.Free( GetDevice(), graphicsCommandPool );
    }
    GetDevice().destroy( graphicsCommandPool, allocator );

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

}  // namespace WindEngine::Core::Render
