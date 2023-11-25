#include "engine.hpp"
#include "app.hpp"
#include "appState.hpp"
#include "core/logger.hpp"
#include <SDL.h>
#include <SDL_vulkan.h>

namespace WindEngine
{
using namespace WindEngine::Core;
using namespace WindEngine::Core::Memory;

Engine::Engine( std::unique_ptr<App> app ) : _upApp( std::move( app ) ), _spAppState( std::make_shared<AppState>() )
{
    _spAppState->isInitialized = Initialize();
    _spAppState->isRunning = true;
}

Engine::~Engine()
{
    Shutdown();
}

auto Engine::Initialize() -> bool
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        WIND_ERROR( "SDL_Init failed. {}", SDL_GetError() );
        return false;
    }

    if ( !_window.Initialize() )
    {
        return false;
    }

    _upApp->Initialize();

    auto* ptrApp = _allocationManager.Allocate( 64, AllocationType::APPLICATION );
    auto* ptrTex = _allocationManager.Allocate( 32, AllocationType::TEXTURE );
    _allocationManager.PrintStats();
    _allocationManager.Free( ptrApp, 64, AllocationType::APPLICATION );
    _allocationManager.Free( ptrTex, 32, AllocationType::TEXTURE );
    _allocationManager.PrintStats();

    return true;
}

void Engine::Run()
{
    if ( !_spAppState->isInitialized )
    {
        Shutdown();
        return;
    }

    while ( _spAppState->isRunning )
    {
        _window.PollEvents( *_spAppState );

        if ( !_spAppState->isSuspended )
        {
            continue;
        }
        
        _upApp->Update();
        _upApp->Render();
    }

    Shutdown();
}

void Engine::Shutdown()
{
    _upApp->Shutdown();

    // Destroy SDL related objects
    _window.Shutdown();
    SDL_Quit();
}

}  // namespace WindEngine
