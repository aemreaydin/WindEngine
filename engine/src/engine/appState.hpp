#ifndef WINDENGINE_APPSTATE_HPP
#define WINDENGINE_APPSTATE_HPP

#include <SDL.h>
#include <logger.hpp>

namespace WindEngine
{

constexpr F64 kFrameRate = 1.0 / 60.0 * 1000.0;

struct FrameStats
{
    U64 totalFrames;
    F64 totalTicks;
};

struct AppState
{
    bool isInitialized { false };
    bool isRunning { false };
    bool isSuspended { false };
    bool isFrameRateFixed { false };
    bool shouldResize { false };

    F64 deltaTime {};
    U64 frameStartTime {};
    U64 lastFrameStartTime {};
    FrameStats frameStats {};

    void FrameStart()
    {
        frameStartTime = SDL_GetTicks64();
        // Time passed between the previous frame start time and now
        deltaTime = static_cast<F64>( frameStartTime - lastFrameStartTime );
    }

    void FrameEnd()
    {
        const auto endTime = SDL_GetTicks64();
        // Time elapsed during update and render
        const auto timeElapsed = static_cast<F64>( endTime - frameStartTime );

        if ( isFrameRateFixed )
        {
            const F64 delay = kFrameRate - timeElapsed;
            WindTrace( "Delta Time: {} ms - Time Elapsed: {} ms - Delaying by {} ms.", deltaTime, timeElapsed, delay );
            SDL_Delay( static_cast<U32>( delay ) );

            frameStats.totalFrames += 1;
            frameStats.totalTicks += timeElapsed + delay;
            WindTrace( "Current FPS: {}", 1000.0 / ( timeElapsed + delay ) );
        }
        else
        {
            frameStats.totalFrames += 1;
            frameStats.totalTicks += timeElapsed;
            WindTrace( "Current FPS: {}", 1000.0 / ( timeElapsed ) );
        }

        lastFrameStartTime = frameStartTime;
    }
};

}  // namespace WindEngine

#endif  // WINDENGINE_APPSTATE_HPP
