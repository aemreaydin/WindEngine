#ifndef WINDENGINE_INSTANCE_HPP
#define WINDENGINE_INSTANCE_HPP

namespace WindEngine::Core::Render
{

struct VulkanContext;

void InstanceInitialize( const char* applicationName, VulkanContext& context );
void InstanceShutdown( VulkanContext& context );

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_INSTANCE_HPP
