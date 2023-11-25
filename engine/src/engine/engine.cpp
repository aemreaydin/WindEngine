#include "engine.hpp"
#include "app.hpp"
#include "core/logger.hpp"
#include <SDL.h>
#include <SDL_vulkan.h>

namespace WindEngine
{
using namespace WindEngine::Core;
using namespace WindEngine::Core::Memory;

Engine::Engine( std::unique_ptr<App> app ) : _upApp( std::move( app ) ), _isInitialized( Initialize() )
{
}

Engine::~Engine()
{
    Shutdown();
}

void Engine::Run()
{
    if ( !_isInitialized )
    {
        Shutdown();
        return;
    }

    _isRunning = true;
    while ( _isRunning )
    {
        Core::Window::PollEvents( _isRunning );

        _upApp->Update();
        _upApp->Render();
    }

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

void Engine::Shutdown()
{
    _upApp->Shutdown();

    // Destroy SDL related objects
    _window.Shutdown();
    SDL_Quit();
}

}  // namespace WindEngine
