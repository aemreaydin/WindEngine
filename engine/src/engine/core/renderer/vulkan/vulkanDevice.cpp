#include "vulkanDevice.hpp"
#include "assert.hpp"
#include "logger.hpp"
#include "vulkanContext.hpp"

namespace WindEngine::Core::Render
{

// Check for required device extensions
#if defined( __APPLE__ )
std::vector<const char*> requiredExtensions { "VK_KHR_portability_subset", "VK_KHR_swapchain" };
#else
std::vector<const char*> requiredExtensions { "VK_KHR_swapchain" };
#endif

void VulkanDevice::Initialize( VulkanContext& context )
{
    InitializePhysicalDevice( context.instance, context.surface );
    InitializeDevice( *context.allocator );

    context.device = *this;
}

void VulkanDevice::Shutdown()
{
    device.destroy();
}

void VulkanDevice::InitializePhysicalDevice( const vk::Instance& instance, const vk::SurfaceKHR& surface )
{
    for ( const auto& pd : instance.enumeratePhysicalDevices() )
    {
        if ( IsPhysicalDeviceSuitable( pd, surface ) )
        {
            physicalDevice = pd;
            physicalDeviceInfo = { .features = physicalDevice.getFeatures(),
                                   .memory = physicalDevice.getMemoryProperties(),
                                   .properties = physicalDevice.getProperties() };
            swapchainSupportInfo = {
                .presentModes = physicalDevice.getSurfacePresentModesKHR( surface ),
                .surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR( surface ),
                .surfaceFormats = physicalDevice.getSurfaceFormatsKHR( surface ),
            };

            indices = FindSuitableQueueFamilyIndices( physicalDevice, surface );

            return;
        }
    }
    WIND_FATAL( "No suitable physical device found." )
}

void VulkanDevice::InitializeDevice( const vk::AllocationCallbacks& allocator )
{
    const std::array<F32, 1> queuePriorities { 1.0F };
    const auto makeQueueInfo = [&]( U32 queueIndex ) {
        const auto queueFamilyProps = physicalDevice.getQueueFamilyProperties()[queueIndex];
        return vk::DeviceQueueCreateInfo { .queueFamilyIndex = queueIndex,
                                           .queueCount = queueFamilyProps.queueCount,
                                           .pQueuePriorities = queuePriorities.data() };
    };
    std::vector<vk::DeviceQueueCreateInfo> queueInfos { makeQueueInfo( indices.graphics ),
                                                        makeQueueInfo( indices.compute ),
                                                        makeQueueInfo( indices.transfer ),
                                                        makeQueueInfo( indices.present ) };

    const auto deviceInfo = vk::DeviceCreateInfo {
        .queueCreateInfoCount = ToU32( queueInfos.size() ),
        .pQueueCreateInfos = queueInfos.data(),
        .enabledExtensionCount = ToU32( requiredExtensions.size() ),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };
    device = physicalDevice.createDevice( deviceInfo, allocator );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( device );
}

bool VulkanDevice::IsPhysicalDeviceSuitable( const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface )
{
    // Check Device Types
    const auto pdProps = physicalDevice.getProperties();
    if ( pdProps.deviceType != vk::PhysicalDeviceType::eDiscreteGpu &&
         pdProps.deviceType != vk::PhysicalDeviceType::eIntegratedGpu )
    {
        WIND_ERROR( "{} is not discrete or integrated.", *pdProps.deviceName )
        return false;
    }

    // Check format and present mode support
    const auto formats = physicalDevice.getSurfaceFormatsKHR( surface );
    const auto presentModes = physicalDevice.getSurfacePresentModesKHR( surface );
    if ( formats.size() == 0 || presentModes.size() == 0 )
    {
        return false;
    }

    const auto deviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
    for ( const auto& ext : requiredExtensions )
    {
        if ( std::find_if( deviceExtensions.begin(), deviceExtensions.end(), [&]( const auto extension ) {
                 return strcmp( ext, extension.extensionName ) == 0;
             } ) == deviceExtensions.end() )
        {
            WIND_ERROR( "{} does not support {}.", *pdProps.deviceName, ext )
            return false;
        }
    }

    // Check Queue Family Support
    bool supportsPresent { false }, supportsGraphics { false }, supportsCompute { false }, supportsTransfer { false };
    const auto queueFamilyProps = physicalDevice.getQueueFamilyProperties();
    for ( size_t ind = 0; ind < queueFamilyProps.size(); ++ind )
    {
        if ( physicalDevice.getSurfaceSupportKHR( ind, surface ) )
        {
            supportsPresent = true;
        }
        if ( queueFamilyProps[ind].queueFlags & vk::QueueFlagBits::eGraphics )
        {
            supportsGraphics = true;
        }
        if ( queueFamilyProps[ind].queueFlags & vk::QueueFlagBits::eCompute )
        {
            supportsCompute = true;
        }
        if ( queueFamilyProps[ind].queueFlags & vk::QueueFlagBits::eTransfer )
        {
            supportsTransfer = true;
        }
    }
    if ( !( supportsTransfer && supportsCompute && supportsGraphics && supportsPresent ) )
    {
        WIND_ERROR( "{} does not have support for required queues.", *pdProps.deviceName )
        return false;
    }

    return true;
}

QueueFamilyIndices VulkanDevice::FindSuitableQueueFamilyIndices( const vk::PhysicalDevice& physicalDevice,
                                                                 const vk::SurfaceKHR& surfaceKhr )
{
    const auto queueFamilyProps = physicalDevice.getQueueFamilyProperties();
    QueueFamilyIndices indices {};

    for ( size_t ind = 0; ind < queueFamilyProps.size(); ++ind )
    {
        const auto& queueFamily = queueFamilyProps[ind];
        // Graphics
        if ( indices.graphics == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics )
        {
            if ( indices.compute != ind && indices.present != ind && indices.transfer != ind )
            {
                indices.graphics = ind;
                continue;
            }
        }
        // Compute
        if ( indices.compute == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eCompute )
        {
            if ( indices.graphics != ind && indices.present != ind && indices.transfer != ind )
            {
                indices.compute = ind;
                continue;
            }
        }
        // Transfer
        if ( indices.transfer == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eTransfer )
        {
            if ( indices.graphics != ind && indices.present != ind && indices.compute != ind )
            {
                indices.transfer = ind;
                continue;
            }
        }
        // TODO: I dont Like this Present
        if ( indices.graphics != ind && indices.transfer != ind && indices.compute != ind &&
             physicalDevice.getSurfaceSupportKHR( ind, surfaceKhr ) )
        {
            indices.present = ind;
            continue;
        }
        else if ( physicalDevice.getSurfaceSupportKHR( ind, surfaceKhr ) )
        {
            indices.present = ind;
            continue;
        }
    }

    G_ASSERT_MSG( indices.graphics != UINT32_MAX && indices.compute != UINT32_MAX && indices.transfer != UINT32_MAX &&
                    indices.present != UINT32_MAX,
                  "Failed to assign an index to a queue." );

    return indices;
}

}  // namespace WindEngine::Core::Render
