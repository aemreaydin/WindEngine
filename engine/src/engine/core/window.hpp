#ifndef WINDENGINE_WINDOW_HPP
#define WINDENGINE_WINDOW_HPP

#include <SDL.h>

namespace WindEngine
{
struct AppState;
}

namespace WindEngine::Core
{

class Window
{
    void OnKeyPress( AppState& appState ) const;
    void OnKeyRelease( AppState& appState ) const;
    void OnButtonPress( AppState& appState ) const;
    void OnButtonRelease( AppState& appState ) const;
    void OnMouseMove( AppState& appState ) const;
    void OnMouseWheel( AppState& appState ) const;

public:
    void PollEvents( AppState& appState );

private:
    SDL_Event _currentEvent {};
};

}  // namespace WindEngine::Core

#endif  // WINDENGINE_WINDOW_HPP
