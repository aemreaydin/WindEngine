#ifndef WINDENGINE_APP_HPP
#define WINDENGINE_APP_HPP

namespace WindEngine
{

class App
{
public:
    App() = default;
    virtual ~App() = default;
    App( const App& ) = delete;
    App( const App&& ) = delete;
    auto operator=( const App& ) -> App& = delete;
    auto operator=( const App&& ) -> App& = delete;

    virtual auto Initialize() -> bool = 0;
    virtual void Shutdown() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
};

}  // namespace WindEngine

#endif  // WINDENGINE_APP_HPP
