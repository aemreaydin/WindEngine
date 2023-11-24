#ifndef WINDENGINE_MAIN_HPP
#define WINDENGINE_MAIN_HPP

#include "app.hpp"
#include "core/logger.hpp"
#include "core/memory/linearAllocator.hpp"
#include "engine.hpp"

auto CreateApp() -> std::unique_ptr<WindEngine::App>;

#pragma pack( push, 1 )

struct TestStruct
{
    U32 val;
    U16 val2;
    U8 val3;
};

#pragma pack( pop )

auto main() -> int
{
    WindEngine::Core::Logger::Initialize();

    WindEngine::Core::Memory::LinearAllocator allocator{ 256 };
    allocator.Init();

    *( static_cast<U8*>( allocator.Allocate( sizeof( U8 ), true ) ) ) = 1;
    *( static_cast<TestStruct*>( allocator.Allocate( sizeof( TestStruct ), true ) ) ) = { 4, 5, 3 };
    *( static_cast<TestStruct*>( allocator.Allocate( sizeof( TestStruct ), true ) ) ) = { 6, 7, 2 };

    {
        WindEngine::Engine engine{ CreateApp() };
        engine.Run();
    }

    WindEngine::Core::Logger::Shutdown();
    return 0;
}

#endif  // WINDENGINE_MAIN_HPP
