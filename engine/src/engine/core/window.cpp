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
        }

        break;
    }
}

void Window::OnKeyPress( AppState& appState ) const
{
    WIND_TRACE( "KeyDown: {}", SDL_GetKeyName( _currentEvent.key.keysym.sym ) )
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
    WIND_TRACE( "KeyUp: {}", SDL_GetKeyName( _currentEvent.key.keysym.sym ) )
}

void Window::OnButtonPress( [[maybe_unused]] AppState& appState ) const
{
    switch ( _currentEvent.button.button )
    {
    case SDL_BUTTON_LEFT:
        WIND_TRACE( "Left Mouse Button Pressed." )
        break;
    case SDL_BUTTON_RIGHT:
        WIND_TRACE( "Right Mouse Button Pressed." )
        break;
    }
}

void Window::OnButtonRelease( [[maybe_unused]] AppState& appState ) const
{
    switch ( _currentEvent.button.button )
    {
    case SDL_BUTTON_LEFT:
        WIND_TRACE( "Left Mouse Button Released." )
        break;
    case SDL_BUTTON_RIGHT:
        WIND_TRACE( "Right Mouse Button Released." )
        break;
    }
}

void Window::OnMouseMove( [[maybe_unused]] AppState& appState ) const
{
    WIND_TRACE( "Mouse Wheel: {}-{}", _currentEvent.wheel.x, _currentEvent.wheel.y )
}

void Window::OnMouseWheel( [[maybe_unused]] AppState& appState ) const
{
    const auto mouseX = _currentEvent.motion.x;
    const auto mouseY = _currentEvent.motion.y;
    WIND_TRACE( "Mouse Position: {}-{}", mouseX, mouseY )
}

}  // namespace WindEngine::Core
