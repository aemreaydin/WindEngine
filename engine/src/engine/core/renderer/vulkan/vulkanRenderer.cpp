#include "vulkanRenderer.hpp"
#include "instance.hpp"
#include <SDL_vulkan.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace WindEngine::Core::Render
{

void VulkanRenderer::Initialize( const char* applicationName, SDL_Window* window )
{
    // Initialize the vulkan-hpp dispatcher
    vk::DynamicLoader dl;
    auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

    InstanceInitialize( window, applicationName, _context );

    // TODO: Might fail
    SDL_Vulkan_CreateSurface( window, _context.instance, reinterpret_cast<VkSurfaceKHR*>( &_context.surface ) );

    _context.device.Initialize( _context );
}

void VulkanRenderer::Shutdown()
{
    _context.device.Shutdown();

    _context.instance.destroy( _context.surface );
#if defined( _DBG )
    _context.instance.destroyDebugUtilsMessengerEXT( _context.debugMessenger );
#endif
    _context.instance.destroy( _context.allocator );
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
