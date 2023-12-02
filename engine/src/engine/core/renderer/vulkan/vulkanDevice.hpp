#ifndef WINDENGINE_VULKANDEVICE_HPP
#define WINDENGINE_VULKANDEVICE_HPP

#include "defines.hpp"
#include <map>
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanContext;

struct QueueFamilyIndices
{
    U32 graphics { UINT32_MAX };
    U32 compute { UINT32_MAX };
    U32 transfer { UINT32_MAX };
    U32 present { UINT32_MAX };
};

struct PhysicalDeviceInfo
{
    vk::PhysicalDeviceFeatures features {};
    vk::PhysicalDeviceMemoryProperties memory {};
    vk::PhysicalDeviceProperties properties {};
};

struct SwapchainSupportInfo
{
    std::vector<vk::PresentModeKHR> presentModes {};
    vk::SurfaceCapabilitiesKHR surfaceCapabilities {};
    std::vector<vk::SurfaceFormatKHR> surfaceFormats {};
};

struct VulkanDevice
{
    vk::PhysicalDevice physicalDevice { nullptr };
    vk::Device device { nullptr };

    vk::Queue graphicsQueue { nullptr };
    vk::Queue computeQueue { nullptr };
    vk::Queue transferQueue { nullptr };
    vk::Queue presentQueue { nullptr };

    QueueFamilyIndices indices {};
    PhysicalDeviceInfo physicalDeviceInfo {};
    SwapchainSupportInfo swapchainSupportInfo {};

    auto Initialize( const vk::Instance& instance, const vk::SurfaceKHR& surface,
                     const vk::AllocationCallbacks* allocator ) -> bool;
    void Shutdown();

private:
    auto InitializePhysicalDevice( const vk::Instance& instance, const vk::SurfaceKHR& surface ) -> bool;
    void InitializeDevice( const vk::AllocationCallbacks* allocator );

    static bool IsPhysicalDeviceSuitable( const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface );
    static QueueFamilyIndices FindSuitableQueueFamilyIndices( const vk::PhysicalDevice& physicalDevice,
                                                              const vk::SurfaceKHR& surfaceKhr );
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANDEVICE_HPP
