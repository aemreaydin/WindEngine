#include "vulkanRenderer.hpp"
#include "instance.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace WindEngine::Core::Render
{

void VulkanRenderer::Initialize( const char* applicationName )
{
    // Initialize the vulkan-hpp dispatcher
    vk::DynamicLoader dl;
    auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );

    InstanceInitialize( applicationName, _context );
}

void VulkanRenderer::Shutdown()
{
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
