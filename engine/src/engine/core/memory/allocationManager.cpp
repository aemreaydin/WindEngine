#include "allocationManager.hpp"
#include "../logger.hpp"
#include <array>
#include <memory>

namespace WindEngine::Core::Memory
{

constexpr std::array<const char*, kAllocationTypeSize> kTagNames { "ARRAY",     "VECTOR", "APPLICATION",
                                                                   "TEXTURE",   "GAME",   "RENDERER",
                                                                   "TRANSFORM", "SCENE",  "DEBUG" };

AllocationManager::AllocationManager() : _allocator( std::make_unique<CAllocator>() )
{
}

auto AllocationManager::Allocate( size_t size, AllocationType type ) -> void*
{
    auto* address = _allocator->Allocate( size, false );
    if ( address != nullptr )
    {
        _stats.tagAllocations[static_cast<size_t>( type )] += size;
        _stats.totalAllocated += size;
    }
    return address;
}

void AllocationManager::Free( void* ptr, size_t size, AllocationType type )
{
    if ( ptr != nullptr )
    {
        _stats.tagAllocations[static_cast<size_t>( type )] -= size;
        _stats.totalAllocated -= size;
    }

    _allocator->Free( ptr );
}

void AllocationManager::PrintStats()
{
    WindDebug( "[[Memory Statistics]]" );
    WindDebug( "\tTotal Allocations: {}", _stats.totalAllocated );
    for ( size_t ind = 0; ind != _stats.tagAllocations.size(); ++ind )
    {
        WindDebug( "\t{} Allocations: {}", kTagNames[ind], _stats.tagAllocations[ind] );
    }
}

}  // namespace WindEngine::Core::Memory
