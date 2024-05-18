#ifndef WINDENGINE_VULKANDEVICE_HPP
#define WINDENGINE_VULKANDEVICE_HPP

#include "defines.hpp"
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanContext;
struct VulkanSwapchain;

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

    [[nodiscard]] auto FindMemoryIndex( U32 memoryTypeBits, vk::MemoryPropertyFlags requiredFlags ) const -> U32;
};

struct SwapchainSupportInfo
{
    std::vector<vk::PresentModeKHR> presentModes;
    vk::SurfaceCapabilitiesKHR surfaceCapabilities {};
    std::vector<vk::SurfaceFormatKHR> surfaceFormats;
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
    vk::Format depthFormat {};

    [[nodiscard]] auto Initialize( const vk::Instance& instance, const vk::SurfaceKHR& surface,
                                   const vk::AllocationCallbacks* allocator ) -> bool;
    void Destroy() const;

    [[nodiscard]] auto AreGraphicsAndPresentSharing() const -> bool;
    void QueryForSwapchainSupportInfo( const vk::SurfaceKHR& surface );

private:
    [[nodiscard]] auto InitializePhysicalDevice( const vk::Instance& instance, const vk::SurfaceKHR& surface ) -> bool;
    void InitializeDevice( const vk::AllocationCallbacks* allocator );

    [[nodiscard]] static auto IsPhysicalDeviceSuitable( const vk::PhysicalDevice& physicalDevice,
                                                        const vk::SurfaceKHR& surface ) -> bool;
    [[nodiscard]] static auto FindSuitableQueueFamilyIndices( const vk::PhysicalDevice& physicalDevice,
                                                              const vk::SurfaceKHR& surfaceKhr ) -> QueueFamilyIndices;

    [[nodiscard]] auto FindDepthFormat() const -> vk::Format;
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANDEVICE_HPP
