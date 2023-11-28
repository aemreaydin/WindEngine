#include "vulkanRenderer.hpp"
#include "vulkan/vulkan_extension_inspection.hpp"
#include "vulkan/vulkan_static_assertions.hpp"

#if __APPLE__
constexpr vk::InstanceCreateFlags kInstanceCreateFlags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
#else
constexpr vk::InstanceCreateFlags kInstanceCreateFlags = 0;
#endif

// #if defined( NDEBUG )
// constexpr auto kIncludeValidation = false;
// #else
// constexpr auto kIncludeValidation = true;
// #endif

namespace WindEngine::Core::Render
{

void AddRequestedExtension( const char* requestedExtension, std::vector<const char*>& enabledExtensions )
{
    if ( vk::isInstanceExtension( requestedExtension ) )
    {
        enabledExtensions.push_back( requestedExtension );
    }
}

auto CreateInstance( const char* applicationName, const vk::AllocationCallbacks& allocator ) -> vk::Instance
{
    const vk::ApplicationInfo appInfo {
        .pApplicationName = applicationName,
        .applicationVersion = VK_MAKE_VERSION( 0, 0, 1 ),
        .pEngineName = "Wind Engine",
        .engineVersion = VK_MAKE_VERSION( 0, 0, 1 ),
        .apiVersion = VK_API_VERSION_1_3,
    };

    const auto layerProps = vk::enumerateInstanceLayerProperties();
    const auto extProps = vk::enumerateInstanceExtensionProperties();

    std::vector<const char*> enabledExtensions;
#if __APPLE__
    AddRequestedExtension( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, enabledExtensions );
#endif

    const vk::InstanceCreateInfo instanceInfo {
        .flags = kInstanceCreateFlags,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = ToU32( enabledExtensions.size() ),
        .ppEnabledExtensionNames = enabledExtensions.data(),
    };
    return vk::createInstance( instanceInfo, allocator );
}

void VulkanRenderer::Initialize( const char* applicationName )
{
    _context.instance = CreateInstance( applicationName, *_context.allocator );
}

void VulkanRenderer::Shutdown()
{
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
