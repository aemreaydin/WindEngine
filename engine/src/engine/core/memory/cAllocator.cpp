#include "cAllocator.hpp"

namespace WindEngine::Core::Memory
{

CAllocator::CAllocator() : Allocator( 0 )
{
}

auto CAllocator::Allocate( size_t size, [[maybe_unused]] bool shouldAlign ) -> void*
{
    return std::malloc( size );
}

void WindEngine::Core::Memory::CAllocator::Free( void* ptr )
{
    std::free( ptr );
}

}  // namespace WindEngine::Core::Memory
