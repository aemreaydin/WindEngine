#ifndef WINDENGINE_ENGINE_HPP
#define WINDENGINE_ENGINE_HPP

#include "core/memory/allocationManager.hpp"
#include "core/window.hpp"
#include "defines.hpp"
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
    [[nodiscard]] auto Initialize() -> bool;
    void Shutdown();

    std::unique_ptr<App> _upApp;
    std::shared_ptr<AppState> _spAppState;
    Core::Window _window{};
    Core::Memory::AllocationManager _allocationManager;
};
}  // namespace WindEngine

#endif  // WINDENGINE_ENGINE_HPP
