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
