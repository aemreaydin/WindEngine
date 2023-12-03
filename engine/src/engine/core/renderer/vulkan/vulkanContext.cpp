#include "vulkanContext.hpp"
#include "logger.hpp"
#include "vulkanDevice.hpp"
#include "vulkanInstance.hpp"

namespace WindEngine::Core::Render
{

VulkanContext::VulkanContext() : swapchain( device, allocator )
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
    return true;
}

void VulkanContext::Shutdown()
{
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
