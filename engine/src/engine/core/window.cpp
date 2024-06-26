#include "window.hpp"
#include "appState.hpp"
#include "logger.hpp"
#include <SDL_vulkan.h>

namespace WindEngine::Core
{

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
        case SDL_WINDOWEVENT: {
            if ( _currentEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
            {
                appState.shouldResize = true;
            }
            break;
        }
        default:
            break;
        }
    }
}

void Window::OnKeyPress( AppState& appState ) const
{
    WindTrace( "KeyDown: {}", SDL_GetKeyName( _currentEvent.key.keysym.sym ) );
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
    default:
        break;
    }
}

void Window::OnKeyRelease( [[maybe_unused]] AppState& appState ) const
{
    WindTrace( "KeyUp: {}", SDL_GetKeyName( _currentEvent.key.keysym.sym ) );
}

void Window::OnButtonPress( [[maybe_unused]] AppState& appState ) const
{
    switch ( _currentEvent.button.button )
    {
    case SDL_BUTTON_LEFT:
        WindTrace( "Left Mouse Button Pressed." );
        break;
    case SDL_BUTTON_RIGHT:
        WindTrace( "Right Mouse Button Pressed." );
        break;
    default:
        break;
    }
}

void Window::OnButtonRelease( [[maybe_unused]] AppState& appState ) const
{
    switch ( _currentEvent.button.button )
    {
    case SDL_BUTTON_LEFT:
        WindTrace( "Left Mouse Button Released." );
        break;
    case SDL_BUTTON_RIGHT:
        WindTrace( "Right Mouse Button Released." );
        break;
    default:
        break;
    }
}

void Window::OnMouseMove( [[maybe_unused]] AppState& appState ) const
{
    WindTrace( "Mouse Wheel: {}-{}", _currentEvent.wheel.x, _currentEvent.wheel.y );
}

void Window::OnMouseWheel( [[maybe_unused]] AppState& appState ) const
{
    const auto mouseX = _currentEvent.motion.x;
    const auto mouseY = _currentEvent.motion.y;
    WindTrace( "Mouse Position: {}-{}", mouseX, mouseY );
}

}  // namespace WindEngine::Core
