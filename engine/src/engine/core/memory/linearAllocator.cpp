#include "linearAllocator.hpp"
#include "core/assert.hpp"
#include "core/logger.hpp"
#include <spdlog/fmt/fmt.h>

namespace WindEngine::Core::Memory
{

static constexpr size_t kAlignment = sizeof( void* );

LinearAllocator::LinearAllocator( size_t size ) : Allocator( size )
{
}

LinearAllocator::~LinearAllocator()
{
    std::free( _pStart );
    _pStart = nullptr;
}

void LinearAllocator::Init()
{
    if ( _pStart != nullptr )
    {
        std::free( _pStart );
    }
    _pStart = static_cast<std::byte*>( std::malloc( GetSize() ) );
    _offset = 0;
    WindDebug( "LinearAllocator::Init" );
}

auto LinearAllocator::Allocate( size_t size, bool shouldAlign ) -> void*
{
    size_t padding = 0;
    if ( shouldAlign && _offset % kAlignment != 0 )
    {
        padding = kAlignment - ( _offset % kAlignment );
    }

    if ( _offset + size + padding > GetSize() )
    {
        return nullptr;
    }

    std::byte* pAddress = _pStart + _offset + padding;
    _offset += size + padding;

    WindDebug( "LinearAllocator::Allocate" );
    WindDebug( "Current Address: {} - Padding: {}", fmt::ptr( pAddress ), padding );

    return pAddress;
}

void LinearAllocator::Free( [[maybe_unused]] void* ptr )
{
    WindAssert( false, "LinearAllocator::Reset should be used instead." );
}

void LinearAllocator::Reset()
{
    WindDebug( "LinearAllocator::Reset" );
    _offset = 0;
}

}  // namespace WindEngine::Core::Memory
