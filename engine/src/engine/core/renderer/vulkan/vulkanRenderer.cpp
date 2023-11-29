#include "vulkanRenderer.hpp"
#include "instance.hpp"
#include "logger.hpp"
#include <SDL_vulkan.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace WindEngine::Core::Render
{

auto VulkanRenderer::Initialize( const char* applicationName ) -> bool
{
    // Initialize the vulkan-hpp dispatcher
    vk::DynamicLoader dl;
    auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

    _context.window = SDL_CreateWindow( "WindEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900,
                                        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );
    if ( _context.window == nullptr )
    {
        WIND_FATAL( "Failed to create SDL window." )
        return false;
    }

    InstanceInitialize( applicationName, _context );

    if ( !SDL_Vulkan_CreateSurface( _context.window, _context.instance,
                                    reinterpret_cast<VkSurfaceKHR*>( &_context.surface ) ) )
    {
        WIND_FATAL( "Failed to create surface." )
        return false;
    }

    if ( !_context.device.Initialize( _context ) )
    {
        return false;
    }

    return true;
}

void VulkanRenderer::Shutdown()
{
    _context.device.Shutdown();

    _context.instance.destroy( _context.surface );

    InstanceShutdown( _context );

    SDL_DestroyWindow( _context.window );
}

auto VulkanRenderer::BeginFrame( [[maybe_unused]] F64 deltaTime ) -> bool
{
    return true;
}

auto VulkanRenderer::EndFrame( [[maybe_unused]] F64 deltaTime ) -> bool
{
    return true;
}

void VulkanRenderer::Resize( [[maybe_unused]] U16 width, [[maybe_unused]] U16 height )
{
}

}  // namespace WindEngine::Core::Render
