#ifndef WINDENGINE_VULKANRENDERER_HPP
#define WINDENGINE_VULKANRENDERER_HPP

#include "defines.hpp"
#include "renderer.hpp"
#include "vulkanContext.hpp"
#include <SDL_video.h>

namespace WindEngine::Core::Render
{

class VulkanRenderer final : public Renderer
{
public:
    auto Initialize( const char* applicationName ) -> bool override;
    void Shutdown() override;
    auto BeginFrame( F64 deltaTime ) -> bool override;
    auto EndFrame( F64 deltaTime ) -> bool override;
    void Resize( U16 width, U16 height ) override;

    VulkanRenderer() = default;
    ~VulkanRenderer() override = default;
    VulkanRenderer( const VulkanRenderer& ) = delete;
    VulkanRenderer( const VulkanRenderer&& ) = delete;
    auto operator=( const VulkanRenderer& ) -> VulkanRenderer& = delete;
    auto operator=( const VulkanRenderer&& ) -> VulkanRenderer& = delete;

private:
    VulkanContext _context;
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANRENDERER_HPP
