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

static auto CreateRenderer( RendererTypes type ) -> std::unique_ptr<Renderer>
{
    switch ( type )
    {
    case RendererTypes::VULKAN:
        return std::make_unique<VulkanRenderer>();
    case RendererTypes::DIRECTX:
        WindError( "DIRECTX renderer not implemented." );
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
        WindError( "SDL_Init failed. {}", SDL_GetError() );
        return false;
    }

    // TODO(emreaydn): Hard-coded
    const AppConfig config( "WindEngine", 1600, 900 );
    if ( !_upRenderer->Initialize( config ) )
    {
        return false;
    }

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

        //        _upApp->Update();
        //        _upApp->Render();

        if ( _upRenderer->BeginFrame( *_spAppState ) )
        {
            _upRenderer->EndFrame( *_spAppState );
        }

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

    SDL_Quit();
}

}  // namespace WindEngine
