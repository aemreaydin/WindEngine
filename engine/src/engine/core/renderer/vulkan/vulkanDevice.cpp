#include "vulkanDevice.hpp"
#include "assert.hpp"
#include "logger.hpp"
#include "vulkanSwapchain.hpp"

namespace WindEngine::Core::Render
{

// Check for required device extensions
#if defined( __APPLE__ )
static std::vector<const char*> requiredExtensions { "VK_KHR_portability_subset", "VK_KHR_swapchain" };
#else
static std::vector<const char*> requiredExtensions { "VK_KHR_swapchain" };
#endif

auto PhysicalDeviceInfo::FindMemoryIndex( U32 memoryTypeBits, vk::MemoryPropertyFlags requiredFlags ) const -> U32
{
    for ( size_t ind = 0; ind < memory.memoryTypeCount; ++ind )
    {
        const auto isRequiredMemoryType = ( memoryTypeBits & ( 1 << ind ) ) != 0U;
        const auto propertyFlags = memory.memoryTypes[ind].propertyFlags;
        const auto hasRequiredProperties = ( propertyFlags & requiredFlags ) == requiredFlags;

        if ( isRequiredMemoryType && hasRequiredProperties )
        {
            return ind;
        }
    }
    WIND_FATAL( "Failed to find a suitable memory index." )
    throw std::runtime_error( "Failed to find a suitable memory index." );  // TODO
}

auto VulkanDevice::Initialize( const vk::Instance& instance, const vk::SurfaceKHR& surface,
                               const vk::AllocationCallbacks* allocator ) -> bool
{
    if ( !InitializePhysicalDevice( instance, surface ) )
    {
        WIND_FATAL( "Failed to find a suitable physical device." )
        return false;
    }

    InitializeDevice( allocator );

    return true;
}

void VulkanDevice::Destroy()
{
    device.destroy();
}

auto VulkanDevice::AreGraphicsAndPresentSharing() const -> bool
{
    return indices.graphics == indices.present;
}

bool VulkanDevice::InitializePhysicalDevice( const vk::Instance& instance, const vk::SurfaceKHR& surface )
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

            depthFormat = FindDepthFormat();

            WIND_INFO( "Physical Device Name: {}", std::string_view( physicalDeviceInfo.properties.deviceName ) );
            WIND_INFO( "Physical Device Type: {}", vk::to_string( physicalDeviceInfo.properties.deviceType ) )
            for ( size_t ind = 0; ind != physicalDeviceInfo.memory.memoryHeapCount; ++ind )
            {
                const auto& memoryHeap = physicalDeviceInfo.memory.memoryHeaps[ind];
                WIND_INFO( "Heap Size: {} GiB - Heap Types: {}", memoryHeap.size / 1024.0f / 1024.0f / 1024.0f,
                           vk::to_string( memoryHeap.flags ) )
            }
            return true;
        }
    }
    return false;
}

void VulkanDevice::InitializeDevice( const vk::AllocationCallbacks* allocator )
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

    graphicsQueue = device.getQueue( indices.graphics, 0 );
    computeQueue = device.getQueue( indices.compute, 0 );
    transferQueue = device.getQueue( indices.transfer, 0 );
    presentQueue = device.getQueue( indices.present, 0 );
}

bool VulkanDevice::IsPhysicalDeviceSuitable( const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface )
{
    // Check Device Types
    const auto pdProps = physicalDevice.getProperties();
    if ( pdProps.deviceType != vk::PhysicalDeviceType::eDiscreteGpu &&
         pdProps.deviceType != vk::PhysicalDeviceType::eIntegratedGpu )
    {
        WIND_ERROR( "{} is not discrete or integrated.", std::string_view( pdProps.deviceName ) )
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
            WIND_ERROR( "{} does not support {}.", std::string_view( pdProps.deviceName ), ext )
            return false;
        }
    }

    // Check Queue Family Support
    bool supportsPresent { false }, supportsGraphics { false }, supportsCompute { false }, supportsTransfer { false };
    const auto queueFamilyProps = physicalDevice.getQueueFamilyProperties();
    for ( size_t ind = 0; ind < queueFamilyProps.size(); ++ind )
    {
        if ( physicalDevice.getSurfaceSupportKHR( ToU32( ind ), surface ) )
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
        WIND_ERROR( "{} does not have support for required queues.", std::string_view( pdProps.deviceName ) )
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
                indices.graphics = ToU32( ind );
                continue;
            }
        }
        // Compute
        if ( indices.compute == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eCompute )
        {
            if ( indices.graphics != ind && indices.present != ind && indices.transfer != ind )
            {
                indices.compute = ToU32( ind );
                continue;
            }
        }
        // Transfer
        if ( indices.transfer == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eTransfer )
        {
            if ( indices.graphics != ind && indices.present != ind && indices.compute != ind )
            {
                indices.transfer = ToU32( ind );
                continue;
            }
        }
        if ( indices.graphics != ind && indices.transfer != ind && indices.compute != ind &&
             physicalDevice.getSurfaceSupportKHR( ToU32( ind ), surfaceKhr ) )
        {
            indices.present = ToU32( ind );
            continue;
        }
    }
    // Second loop in case we have similar queue family indices
    for ( size_t ind = 0; ind < queueFamilyProps.size(); ++ind )
    {
        const auto& queueFamily = queueFamilyProps[ind];
        // Graphics
        if ( indices.graphics == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics )
        {
            indices.graphics = ToU32( ind );
        }
        // Compute
        if ( indices.compute == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eCompute )
        {
            indices.compute = ToU32( ind );
        }
        // Transfer
        if ( indices.transfer == UINT32_MAX && queueFamily.queueFlags & vk::QueueFlagBits::eTransfer )
        {
            indices.transfer = ToU32( ind );
        }
        // Present
        if ( indices.present == UINT32_MAX && physicalDevice.getSurfaceSupportKHR( ToU32( ind ), surfaceKhr ) )
        {
            indices.present = ToU32( ind );
        }
    }

    G_ASSERT_MSG( indices.graphics != UINT32_MAX && indices.compute != UINT32_MAX && indices.transfer != UINT32_MAX &&
                    indices.present != UINT32_MAX,
                  "Failed to assign an index to a queue." );

    return indices;
}

auto VulkanDevice::FindDepthFormat() -> vk::Format
{
    auto formatProperties = physicalDevice.getFormatProperties( vk::Format::eD32SfloatS8Uint );
    if ( formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
    {
        return vk::Format::eD32SfloatS8Uint;
    }

    formatProperties = physicalDevice.getFormatProperties( vk::Format::eD24UnormS8Uint );
    if ( formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment )
    {
        return vk::Format::eD24UnormS8Uint;
    }

    WIND_FATAL( "Failed to find a suitable depth format." );
    throw std::runtime_error( "Failed to find a suitable format." );  // TODO
}

}  // namespace WindEngine::Core::Render
