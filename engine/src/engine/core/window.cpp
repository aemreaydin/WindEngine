#include "window.hpp"
#include "logger.hpp"
#include <SDL.h>
#include <SDL_vulkan.h>

auto WindEngine::Core::Window::Initialize() -> bool
{
    _window = SDL_CreateWindow( "WindEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );
    if ( _window == nullptr )
    {
        WIND_ERROR( "Failed to create SDL_Window. {}", SDL_GetError() );
    }
    return _window != nullptr;
}

void WindEngine::Core::Window::PollEvents( bool& isRunning )
{
    SDL_Event event;
    while ( SDL_PollEvent( &event ) != 0 )
    {
        if ( event.type == SDL_QUIT || ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) )
        {
            isRunning = false;
        }
        break;
    }
}

void WindEngine::Core::Window::Shutdown()
{
    if ( _window == nullptr )
    {
        return;
    }
    SDL_DestroyWindow( _window );
}
