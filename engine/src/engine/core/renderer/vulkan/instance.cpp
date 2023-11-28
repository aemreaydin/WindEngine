#include "instance.hpp"
#include <logger.hpp>
#include <vulkan/vulkan_extension_inspection.hpp>
#include <vulkan/vulkan_static_assertions.hpp>
#include <vulkanContext.hpp>

// #if defined( _DBG )
// constexpr auto kIncludeValidation = false;
// #else
// constexpr auto kIncludeValidation = true;
// #endif

VkBool32 VKAPI_PTR DebugUtilsMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                [[maybe_unused]] void* pUserData )
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
        WIND_TRACE( "[{}]: {}", type, pCallbackData->pMessage )
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        WIND_INFO( "[{}]: {}", type, pCallbackData->pMessage )
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        WIND_WARN( "[{}]: {}", type, pCallbackData->pMessage )
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        WIND_ERROR( "[{}]: {}", type, pCallbackData->pMessage )
        break;
    default:
        WIND_ERROR( "**UNKNOWN_SEVERITY** - [{}]: {}", type, pCallbackData->pMessage )
        break;
    }
    return VK_FALSE;
}

namespace WindEngine::Core::Render
{

void AddRequestedExtension( const char* requestedExtensionName, std::vector<const char*>& enabledExtensionNames )
{
    if ( vk::isInstanceExtension( requestedExtensionName ) )
    {
        enabledExtensionNames.push_back( requestedExtensionName );
        return;
    }
    WIND_WARN( "Failed to find extension {}." )
}

void InstanceInitialize( const char* applicationName, VulkanContext& context )
{
    const vk::ApplicationInfo appInfo {
        .pApplicationName = applicationName,
        .applicationVersion = VK_MAKE_VERSION( 0, 0, 1 ),
        .pEngineName = "Wind Engine",
        .engineVersion = VK_MAKE_VERSION( 0, 0, 1 ),
        .apiVersion = VK_API_VERSION_1_3,
    };

    vk::InstanceCreateFlags flags {};
    std::vector<const char*> enabledExtensions;
#if defined( __APPLE__ )
    flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    AddRequestedExtension( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, enabledExtensions );
#endif

    std::vector<const char*> enabledLayers;
    vk::DebugUtilsMessengerCreateInfoEXT debugInfo;
    void* pNext;
#if defined( _DBG )
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

    pNext = static_cast<vk::DebugUtilsMessengerCreateInfoEXT*>( &debugInfo );
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

    context.instance = vk::createInstance( instanceInfo, context.allocator );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( context.instance );

#if defined( _DBG )
    context.debugMessenger = context.instance.createDebugUtilsMessengerEXT( debugInfo, context.allocator );
#endif
}

}  // namespace WindEngine::Core::Render
