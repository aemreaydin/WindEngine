#ifndef WINDENGINE_WINDOW_HPP
#define WINDENGINE_WINDOW_HPP

struct SDL_Window;

namespace WindEngine::Core
{

class Window
{
    SDL_Window* _window{ nullptr };

  public:
    [[nodiscard]] auto Initialize() -> bool;
    static void PollEvents( bool& isRunning );
    void Shutdown();
};

}  // namespace WindEngine::Core

#endif  // WINDENGINE_WINDOW_HPP
