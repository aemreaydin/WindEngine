#include "vulkanRenderer.hpp"
#include "logger.hpp"
#include "vulkanInstance.hpp"
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

    return _context.Initialize( applicationName );
}

void VulkanRenderer::Shutdown()
{
    _context.Shutdown();

    SDL_DestroyWindow( _context.window );
}

auto VulkanRenderer::BeginFrame( [[maybe_unused]] F64 deltaTime ) -> bool
{
    //    auto optionalIndex = _context.swapchain.AcquireNextImage( 0, nullptr, nullptr );
    //    if ( !optionalIndex.has_value() )
    //    {
    //        _context.swapchain.Recreate( _context.surface, _context.framebufferWidth, _context.framebufferHeight );
    //        return false;
    //    }
    //    _context.frameData.imageIndex = *optionalIndex;
    return true;
}

auto VulkanRenderer::EndFrame( [[maybe_unused]] F64 deltaTime ) -> bool
{
    //    if ( !_context.swapchain.Present( nullptr, _context.frameData.imageIndex ) )
    //    {
    //        _context.swapchain.Recreate( _context.surface, _context.framebufferWidth, _context.framebufferHeight );
    //        return false;
    //    }
    return true;
}

void VulkanRenderer::Resize( [[maybe_unused]] U16 width, [[maybe_unused]] U16 height )
{
}

}  // namespace WindEngine::Core::Render
