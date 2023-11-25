#ifndef WINDENGINE_ALLOCATIONMANAGER_HPP
#define WINDENGINE_ALLOCATIONMANAGER_HPP

#include "cAllocator.hpp"
#include "defines.hpp"
#include <array>
#include <memory>

namespace WindEngine::Core::Memory
{

enum class AllocationType
{
    ARRAY,
    VECTOR,
    APPLICATION,
    TEXTURE,
    GAME,
    RENDERER,
    TRANSFORM,
    SCENE,
    DEBUG,
};

constexpr size_t kAllocationTypeSize = static_cast<size_t>( AllocationType::DEBUG ) + 1;

struct MemoryStats
{
    U64 totalAllocated;
    std::array<U64, kAllocationTypeSize> tagAllocations;
};

class AllocationManager
{
public:
    AllocationManager();
    auto Allocate( size_t size, AllocationType type ) -> void*;
    void Free( void* ptr, size_t size, AllocationType type );

    void PrintStats();

private:
    std::unique_ptr<CAllocator> _allocator{ nullptr };
    MemoryStats _stats{};
};

}  // namespace WindEngine::Core::Memory

#endif  // WINDENGINE_ALLOCATIONMANAGER_HPP
