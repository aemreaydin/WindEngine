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

    InstanceInitialize( applicationName, *this );

    if ( !SDL_Vulkan_CreateSurface( window, instance, reinterpret_cast<VkSurfaceKHR*>( &surface ) ) )
    {
        WIND_FATAL( "Failed to create surface." )
        return false;
    }

    if ( !device.Initialize( *this ) )
    {
        return false;
    }

    return true;
}

void VulkanContext::Shutdown()
{
    device.Shutdown();

    instance.destroy( surface );

    InstanceShutdown( *this );

    SDL_DestroyWindow( window );
}

}  // namespace WindEngine::Core::Render
