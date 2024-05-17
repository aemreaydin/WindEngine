#include "vulkanInstance.hpp"
#include <logger.hpp>
#include <vulkan/vulkan_extension_inspection.hpp>
#include <vulkan/vulkan_static_assertions.hpp>

// #if defined( _DBG )
// constexpr auto kIncludeValidation = false;
// #else
// constexpr auto kIncludeValidation = true;
// #endif

static auto VKAPI_PTR DebugUtilsMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                   VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                   [[maybe_unused]] void* pUserData ) -> VkBool32
{
    const auto* type = std::invoke( [&]() {
        switch ( messageTypes )
        {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            return "GENERAL";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            return "VALIDATION";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            return "PERFORMANCE";
        default:
            return "UNKNOWN";
        }
    } );

    //    const auto* msgIdName = pCallbackData->pMessageIdName != nullptr ? pCallbackData->pMessageIdName : "NO_ID";

    switch ( messageSeverity )
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        WindTrace( "[{}]: {}", type, pCallbackData->pMessage );
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        WindInfo( "[{}]: {}", type, pCallbackData->pMessage );
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        WindWarn( "[{}]: {}", type, pCallbackData->pMessage );
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        WindError( "[{}]: {}", type, pCallbackData->pMessage );
        break;
    default:
        WindError( "**UNKNOWN_SEVERITY** - [{}]: {}", type, pCallbackData->pMessage );
        break;
    }
    return VK_FALSE;
}

namespace WindEngine::Core::Render
{

static void AddRequestedExtension( const char* requestedExtensionName, std::vector<const char*>& enabledExtensionNames )
{
    if ( vk::isInstanceExtension( requestedExtensionName ) )
    {
        enabledExtensionNames.push_back( requestedExtensionName );
        return;
    }
    WindWarn( "Failed to find extension {}.", requestedExtensionName );
}

void VulkanInstance::Initialize( const char* applicationName, SDL_Window* window, vk::AllocationCallbacks* allocator )
{
    const vk::ApplicationInfo appInfo {
        .pApplicationName = applicationName,
        .applicationVersion = VK_MAKE_VERSION( 0, 0, 1 ),
        .pEngineName = "Wind Engine",
        .engineVersion = VK_MAKE_VERSION( 0, 0, 1 ),
        .apiVersion = VK_API_VERSION_1_3,
    };

    vk::InstanceCreateFlags flags {};
    U32 numSDLExtensions {};
    SDL_Vulkan_GetInstanceExtensions( window, &numSDLExtensions, nullptr );
    std::vector<const char*> enabledExtensions( numSDLExtensions );
    SDL_Vulkan_GetInstanceExtensions( window, &numSDLExtensions, enabledExtensions.data() );

#if defined( __APPLE__ )
    flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    AddRequestedExtension( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, enabledExtensions );
#endif

    std::vector<const char*> enabledLayers;
#if defined( _DBG )
    vk::DebugUtilsMessengerCreateInfoEXT debugInfo;
    AddRequestedExtension( VK_EXT_DEBUG_UTILS_EXTENSION_NAME, enabledExtensions );
    enabledLayers.push_back( "VK_LAYER_KHRONOS_validation" );

    debugInfo = { .flags = vk::DebugUtilsMessengerCreateFlagsEXT {},
                  .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
                  .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
                  .pfnUserCallback = &DebugUtilsMessengerCallback };

    void* pNext = static_cast<vk::DebugUtilsMessengerCreateInfoEXT*>( &debugInfo );
#else
    void* pNext { nullptr };
#endif
    const vk::InstanceCreateInfo instanceInfo {
        .pNext = pNext,
        .flags = flags,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = ToU32( enabledLayers.size() ),
        .ppEnabledLayerNames = enabledLayers.data(),
        .enabledExtensionCount = ToU32( enabledExtensions.size() ),
        .ppEnabledExtensionNames = enabledExtensions.data(),
    };

    instance = vk::createInstance( instanceInfo, allocator );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance );

#if defined( _DBG )
    debugMessenger = instance.createDebugUtilsMessengerEXT( debugInfo, allocator );
#endif
}

void VulkanInstance::Destroy( vk::AllocationCallbacks* allocator )
{
#if defined( _DBG )
    instance.destroyDebugUtilsMessengerEXT( debugMessenger );
#endif
    instance.destroy( allocator );
}

}  // namespace WindEngine::Core::Render
