#include "window.hpp"
#include "appState.hpp"
#include "logger.hpp"
#include <SDL_vulkan.h>

namespace WindEngine::Core
{

auto Window::Initialize() -> bool
{
    _window = SDL_CreateWindow( "WindEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE );
    if ( _window == nullptr )
    {
        WIND_ERROR( "Failed to create SDL_Window. {}", SDL_GetError() )
        return false;
    }
    WIND_DEBUG( "Window::Initialize" )
    return true;
}

void Window::PollEvents( AppState& appState )
{
    while ( SDL_PollEvent( &_currentEvent ) != 0 )
    {
        switch ( _currentEvent.type )
        {
        case SDL_KEYDOWN: {
            OnKeyPress( appState );
            break;
        }
        case SDL_KEYUP: {
            OnKeyRelease( appState );
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            OnButtonPress( appState );
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            OnButtonRelease( appState );
            break;
        }
        case SDL_MOUSEWHEEL:
            OnMouseMove( appState );
            break;
        case SDL_MOUSEMOTION: {
            OnMouseWheel( appState );
            break;
        }
        }

        break;
    }
}

void Window::Shutdown()
{
    if ( _window == nullptr )
    {
        return;
    }
    SDL_DestroyWindow( _window );
    WIND_DEBUG( "Window::Shutdown" )
}

void Window::OnKeyPress( AppState& appState ) const
{
    WIND_DEBUG( "KeyDown: {}", SDL_GetKeyName( _currentEvent.key.keysym.sym ) )
    switch ( _currentEvent.key.keysym.sym )
    {
    case SDLK_ESCAPE:
        appState.isRunning = false;
        break;
    case SDLK_F1:
        appState.isSuspended = !appState.isSuspended;
        break;
    case SDLK_F2:
        appState.isFrameRateFixed = !appState.isFrameRateFixed;
        break;
    }
}

void Window::OnKeyRelease( [[maybe_unused]] AppState& appState ) const
{
    WIND_DEBUG( "KeyUp: {}", SDL_GetKeyName( _currentEvent.key.keysym.sym ) )
}

void Window::OnButtonPress( [[maybe_unused]] AppState& appState ) const
{
    switch ( _currentEvent.button.button )
    {
    case SDL_BUTTON_LEFT:
        WIND_DEBUG( "Left Mouse Button Pressed." )
        break;
    case SDL_BUTTON_RIGHT:
        WIND_DEBUG( "Right Mouse Button Pressed." )
        break;
    }
}

void Window::OnButtonRelease( [[maybe_unused]] AppState& appState ) const
{
    switch ( _currentEvent.button.button )
    {
    case SDL_BUTTON_LEFT:
        WIND_DEBUG( "Left Mouse Button Released." )
        break;
    case SDL_BUTTON_RIGHT:
        WIND_DEBUG( "Right Mouse Button Released." )
        break;
    }
}

void Window::OnMouseMove( [[maybe_unused]] AppState& appState ) const
{
    WIND_DEBUG( "Mouse Wheel: {}-{}", _currentEvent.wheel.x, _currentEvent.wheel.y )
}

void Window::OnMouseWheel( [[maybe_unused]] AppState& appState ) const
{
    const auto mouseX = _currentEvent.motion.x;
    const auto mouseY = _currentEvent.motion.y;
    WIND_DEBUG( "Mouse Position: {}-{}", mouseX, mouseY )
}

}  // namespace WindEngine::Core
