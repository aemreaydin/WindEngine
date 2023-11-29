#include "vulkanContext.hpp"
#include "logger.hpp"
#include "vulkanDevice.hpp"
#include "vulkanInstance.hpp"

namespace WindEngine::Core::Render
{

auto VulkanContext::Initialize( const char* applicationName ) -> bool
{
    window = SDL_CreateWindow( "WindEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900,
                               SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );
    if ( window == nullptr )
    {
        WIND_FATAL( "Failed to create SDL window." )
        return false;
    }

    instance.Initialize( applicationName, window, allocator );

    if ( !SDL_Vulkan_CreateSurface( window, GetInstance(), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) )
    {
        WIND_FATAL( "Failed to create surface." )
        return false;
    }

    if ( !device.Initialize( GetInstance(), surface, allocator ) )
    {
        return false;
    }

    return true;
}

void VulkanContext::Shutdown()
{
    device.Shutdown();

    GetInstance().destroy( surface );

    instance.Shutdown( allocator );

    SDL_DestroyWindow( window );
}

auto VulkanContext::GetInstance() const -> vk::Instance
{
    return instance.instance;
}

auto VulkanContext::GetDevice() const -> vk::Device
{
    return device.device;
}

}  // namespace WindEngine::Core::Render
