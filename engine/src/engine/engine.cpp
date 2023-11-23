#include "engine.hpp"
#include "core/logger.hpp"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <memory>

void WindEngine::Engine::Run()
{
    if ( !Initialize() )
    {
        Shutdown();
        return;
    }

    _isRunning = true;
    while ( _isRunning )
    {
        WindEngine::Core::Window::PollEvents( _isRunning );
    }

    Shutdown();
}

auto WindEngine::Engine::Initialize() -> bool
{
    Core::Logger::Initialize();

    WIND_TRACE( "Testing logger v{}", 0.0 );
    WIND_DEBUG( "Testing logger v{}", 0.0 );
    WIND_INFO( "Testing logger v{}", 0.0 );
    WIND_WARN( "Testing logger v{}", 0.0 );
    WIND_ERROR( "Testing logger v{}", 0.0 );
    WIND_FATAL( "Testing logger v{}", 0.0 );

    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        WIND_ERROR( "SDL_Init failed. {}", SDL_GetError() );
        return false;
    }

    if ( !_window.Initialize() )
    {
        return false;
    }

    return true;
}

void WindEngine::Engine::Shutdown()
{
    _window.Shutdown();
    SDL_Quit();

    Core::Logger::Shutdown();
}