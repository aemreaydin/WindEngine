#ifndef WINDENGINE_ENGINE_HPP
#define WINDENGINE_ENGINE_HPP

#include "allocationManager.hpp"
#include "defines.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include <memory>

namespace WindEngine
{
class App;
struct AppState;

class WINDAPI Engine
{
public:
    explicit Engine( std::unique_ptr<App> app );
    ~Engine();
    Engine( const Engine& ) = delete;
    Engine( const Engine&& ) = delete;
    auto operator=( const Engine& ) -> Engine& = delete;
    auto operator=( const Engine&& ) -> Engine& = delete;

    void Run();

private:
    auto Initialize() -> bool;
    void Shutdown();

    std::unique_ptr<App> _upApp { nullptr };
    std::shared_ptr<AppState> _spAppState { nullptr };
    Core::Window _window {};
    Core::Memory::AllocationManager _allocationManager {};
    std::unique_ptr<Core::Render::Renderer> _upRenderer { nullptr };
};
}  // namespace WindEngine

#endif  // WINDENGINE_ENGINE_HPP
