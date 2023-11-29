#ifndef WINDENGINE_RENDERER_HPP
#define WINDENGINE_RENDERER_HPP

#include "defines.hpp"
#include <SDL.h>

namespace WindEngine::Core::Render
{

enum class RendererTypes
{
    VULKAN,
    DIRECTX
};
constexpr auto kDefaultRenderer = RendererTypes::VULKAN;

class Renderer
{
public:
    virtual void Initialize( const char* applicationName, SDL_Window* window ) = 0;
    virtual void Shutdown() = 0;
    virtual auto BeginFrame( F64 deltaTime ) -> bool = 0;
    virtual auto EndFrame( F64 deltaTime ) -> bool = 0;
    virtual void Resize( U16 width, U16 height ) = 0;

    Renderer() = default;
    virtual ~Renderer() = default;
    Renderer( const Renderer& ) = delete;
    Renderer( const Renderer&& ) = delete;
    auto operator=( const Renderer& ) -> Renderer& = delete;
    auto operator=( const Renderer&& ) -> Renderer& = delete;
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_RENDERER_HPP
