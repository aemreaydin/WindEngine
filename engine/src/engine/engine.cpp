#include "engine.hpp"
#include "app.hpp"
#include "appState.hpp"
#include "core/logger.hpp"
#include "vulkanRenderer.hpp"
#include <SDL.h>
#include <SDL_vulkan.h>

namespace WindEngine
{
using namespace WindEngine::Core;
using namespace WindEngine::Core::Memory;
using namespace WindEngine::Core::Render;

auto CreateRenderer( RendererTypes type ) -> std::unique_ptr<Renderer>
{
    switch ( type )
    {
    case RendererTypes::VULKAN:
        return std::make_unique<VulkanRenderer>();
    case RendererTypes::DIRECTX:
        WIND_ERROR( "DIRECTX renderer not implemented." )
        return nullptr;
    }
}

Engine::Engine( std::unique_ptr<App> app )
  : _upApp( std::move( app ) ), _spAppState( std::make_shared<AppState>() ),
    _upRenderer( CreateRenderer( kDefaultRenderer ) )
{
    if ( Initialize() )
    {
        _spAppState->isInitialized = true;
        _spAppState->isRunning = true;
    }
}

Engine::~Engine()
{
    Shutdown();
}

auto Engine::Initialize() -> bool
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        WIND_ERROR( "SDL_Init failed. {}", SDL_GetError() )
        return false;
    }

    if ( !_window.Initialize() )
    {
        return false;
    }

    // TODO: Hard-coded name
    _upRenderer->Initialize( "Wind Engine", _window.GetSDLWindow() );

    _upApp->Initialize();

    return true;
}

void Engine::Run()
{
    if ( !_spAppState->isInitialized )
    {
        return;
    }
    while ( _spAppState->isRunning )
    {
        _window.PollEvents( *_spAppState );

        if ( _spAppState->isSuspended )
        {
            continue;
        }

        _spAppState->FrameStart();

        _upApp->Update();
        _upApp->Render();

        _spAppState->FrameEnd();
    }
}

void Engine::Shutdown()
{
    if ( !_spAppState->isInitialized )
    {
        return;
    }
    _upApp->Shutdown();

    _upRenderer->Shutdown();
    // Destroy SDL related objects
    _window.Shutdown();
    SDL_Quit();
}

}  // namespace WindEngine
