#ifndef WINDENGINE_APPSTATE_HPP
#define WINDENGINE_APPSTATE_HPP

namespace WindEngine
{

struct AppState
{
    bool isInitialized{ false };
    bool isRunning{ false };
    bool isSuspended{ false };
};

}  // namespace WindEngine

#endif  // WINDENGINE_APPSTATE_HPP
