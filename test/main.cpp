#include <engine/app.hpp>
#include <engine/core/logger.hpp>
#include <engine/main.hpp>
#include <memory>

using namespace WindEngine;

class WindEditorApp : public App
{
public:
    auto Initialize() -> bool override
    {
        WindDebug( "WindEditorApp::Initialize." );
        return false;
    }

    void Shutdown() override
    {
        WindDebug( "WindEditorApp::Shutdown." );
    }

    void Update() override
    {
        SDL_Delay( 3 );
        //        WindTrace( "WindEditorApp::Update." );
    }

    void Render() override
    {
        SDL_Delay( 3 );
        //        WindTrace( "WindEditorApp::Render." );
    }
};

auto G_CREATE_APP() -> std::unique_ptr<WindEngine::App>
{
    return std::make_unique<WindEditorApp>();
}
