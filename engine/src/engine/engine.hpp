#ifndef WINDENGINE_ENGINE_HPP
#define WINDENGINE_ENGINE_HPP

#include "core/window.hpp"
#include "defines.hpp"

namespace WindEngine
{
class WINDAPI Engine
{
  public:
    Engine() = default;

    void Run();

  private:
    [[nodiscard]] auto Initialize() -> bool;
    void Shutdown();

    Core::Window _window{};
    bool _isRunning{ false };
};
}  // namespace WindEngine

#endif  // WINDENGINE_ENGINE_HPP
