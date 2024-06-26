#include "vulkanSwapchain.hpp"
#include "logger.hpp"

namespace WindEngine::Core::Render
{

VulkanSwapchain::VulkanSwapchain( VulkanDevice& device, vk::AllocationCallbacks* allocator )
  : VulkanHandle( &device, allocator ), depthImage( device, allocator )
{
}

void VulkanSwapchain::Initialize( const vk::SurfaceKHR& surface, U32 width, U32 height )
{
    Create( surface, width, height );
}

void VulkanSwapchain::Destroy()
{
    depthImage.Destroy();

    for ( auto& imageView : imageViews )
    {
        _device->device.destroy( imageView, _allocator );
    }
    _device->device.destroy( swapchain, _allocator );
}

void VulkanSwapchain::Recreate( const vk::SurfaceKHR& surface, U32 width, U32 height )
{
    Destroy();
    Create( surface, width, height );
    WindDebug( "Swapchain recreated with size ({},{})", width, height );
}

auto VulkanSwapchain::AcquireNextImage( U64 timeout, vk::Semaphore semaphore, vk::Fence fence ) -> std::optional<U32>
{
    const auto [result, imageIndex] = _device->device.acquireNextImageKHR( swapchain, timeout, semaphore, fence );
    if ( result == vk::Result::eErrorOutOfDateKHR )
    {
        return {};
    }
    if ( result == vk::Result::eSuccess || result == vk::Result::eSuboptimalKHR )
    {
        return { imageIndex };
    }
    WindFatal( "Failed to acquire image." );
}

auto VulkanSwapchain::Present( vk::Semaphore semaphore, U32 imageIndex ) -> bool
{
    const auto presentInfo = vk::PresentInfoKHR {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &semaphore,
        .swapchainCount = 1,
        .pSwapchains = &swapchain,
        .pImageIndices = &imageIndex,
        .pResults = nullptr,
    };
    const auto result = _device->presentQueue.presentKHR( presentInfo );
    if ( result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR )
    {
        return false;
    }
    if ( result == vk::Result::eSuccess )
    {
        return true;
    }
    WindFatal( "Failed to present image." );
}

void VulkanSwapchain::Create( const vk::SurfaceKHR& surface, U32 width, U32 height )
{
    _device->QueryForSwapchainSupportInfo( surface );
    const auto& surfaceCapabilities = _device->swapchainSupportInfo.surfaceCapabilities;
    const auto& formats = _device->swapchainSupportInfo.surfaceFormats;
    const auto& presentModes = _device->swapchainSupportInfo.presentModes;

    auto minImageCount = surfaceCapabilities.minImageCount + 1;
    // maxImageCount = 0 means that there is not limit on the number of images
    if ( surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount )
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    auto imageFormatIter = std::ranges::find_if( formats, []( const auto& format ) {
        return format.format == vk::Format::eR8G8B8A8Unorm &&
               format.colorSpace == vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear;
    } );
    if ( imageFormatIter == formats.end() )
    {
        imageFormat = formats[0];
    }
    WindDebug( "Selected Format: {}", vk::to_string( imageFormat.format ) );

    vk::Extent2D imageExtent;
    if ( surfaceCapabilities.currentExtent.width != UINT32_MAX )
    {
        imageExtent = surfaceCapabilities.currentExtent;
    }
    else
    {
        imageExtent = {
            std::clamp( width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width ),
            std::clamp( height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height )
        };
    }

    auto imageSharingMode = vk::SharingMode::eExclusive;
    std::vector<U32> queueFamilyIndices {};
    if ( !_device->AreGraphicsAndPresentSharing() )
    {
        imageSharingMode = vk::SharingMode::eConcurrent;
        queueFamilyIndices.assign( { _device->indices.graphics, _device->indices.present } );
    }

    auto presentMode = vk::PresentModeKHR::eFifo;
    auto presentModeIter = std::ranges::find_if(
      presentModes, []( const auto& presentMode ) { return presentMode == vk::PresentModeKHR::eMailbox; } );
    if ( presentModeIter != presentModes.end() )
    {
        presentMode = *presentModeIter;
    }

    const auto swapchainInfo = vk::SwapchainCreateInfoKHR {
        .surface = surface,
        .minImageCount = minImageCount,
        .imageFormat = imageFormat.format,
        .imageColorSpace = imageFormat.colorSpace,
        .imageExtent = imageExtent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = imageSharingMode,
        .queueFamilyIndexCount = ToU32( queueFamilyIndices.size() ),
        .pQueueFamilyIndices = queueFamilyIndices.empty() ? nullptr : queueFamilyIndices.data(),
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = nullptr,
    };

    swapchain = _device->device.createSwapchainKHR( swapchainInfo, _allocator );

    images = _device->device.getSwapchainImagesKHR( swapchain );
    imageViews.resize( images.size() );

    for ( size_t ind = 0; ind < images.size(); ++ind )
    {
        const auto imageViewInfo = vk::ImageViewCreateInfo { .image = images[ind],
                                                             .viewType = vk::ImageViewType::e2D,
                                                             .format = imageFormat.format,
                                                             .components = {},
                                                             .subresourceRange = vk::ImageSubresourceRange {
                                                               .aspectMask = vk::ImageAspectFlagBits::eColor,
                                                               .baseMipLevel = 0,
                                                               .levelCount = 1,
                                                               .baseArrayLayer = 0,
                                                               .layerCount = 1 } };
        imageViews[ind] = _device->device.createImageView( imageViewInfo, _allocator );
    }
    imageCount = ToU32( images.size() );

    const auto depthImageInfo = VulkanImageCreateInfo { .aspectFlags = vk::ImageAspectFlagBits::eDepth,
                                                        .extent = { imageExtent.width, imageExtent.height, 1 },
                                                        .format = _device->depthFormat,
                                                        .imageType = vk::ImageType::e2D,
                                                        .memoryFlags = vk::MemoryPropertyFlagBits::eDeviceLocal,
                                                        .tiling = vk::ImageTiling::eOptimal,
                                                        .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment };
    depthImage.Initialize( depthImageInfo );
}

}  // namespace WindEngine::Core::Render
