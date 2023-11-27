#ifndef WINDENGINE_MAIN_HPP
#define WINDENGINE_MAIN_HPP

#include "app.hpp"
#include "core/logger.hpp"
#include "core/memory/linearAllocator.hpp"
#include "engine.hpp"

auto CreateApp() -> std::unique_ptr<WindEngine::App>;

auto main() -> int
{
    WindEngine::Core::Logger::Initialize();

    WindEngine::Core::Memory::LinearAllocator allocator { 256 };
    allocator.Init();
    
    {
        WindEngine::Engine engine { CreateApp() };
        engine.Run();
    }

    WindEngine::Core::Logger::Shutdown();
    return 0;
}

#endif  // WINDENGINE_MAIN_HPP
