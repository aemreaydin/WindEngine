#ifndef WINDENGINE_INSTANCE_HPP
#define WINDENGINE_INSTANCE_HPP

#include "defines.hpp"
#include <vulkan/vulkan.hpp>

namespace WindEngine::Core::Render
{

struct VulkanContext;

void AddRequestedExtension( const char* requestedExtensionName, std::vector<const char*>& enabledExtensionNames );
void InstanceInitialize( const char* applicationName, VulkanContext& context );

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_INSTANCE_HPP
