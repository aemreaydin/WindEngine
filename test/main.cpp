#include <engine/main.hpp>

using namespace WindEngine;

class WindEditorApp : public App
{
public:
    auto Initialize() -> bool override
    {
        WIND_DEBUG( "WindEditorApp::Initialize." )
        return false;
    }

    void Shutdown() override
    {
        WIND_DEBUG( "WindEditorApp::Shutdown." )
    }

    void Update() override
    {
        WIND_TRACE( "WindEditorApp::Update." )
    }

    void Render() override
    {
        WIND_TRACE( "WindEditorApp::Render." )
    }
};

auto CreateApp() -> std::unique_ptr<WindEngine::App>
{
    return std::make_unique<WindEditorApp>();
}