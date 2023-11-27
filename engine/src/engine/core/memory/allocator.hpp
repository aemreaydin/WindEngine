#ifndef WINDENGINE_ALLOCATOR_HPP
#define WINDENGINE_ALLOCATOR_HPP

#include "defines.hpp"
#include <memory>

namespace WindEngine::Core::Memory
{

class Allocator
{
public:
    explicit Allocator( size_t size ) : _size( size )
    {
    }

    virtual ~Allocator()
    {
        _size = 0;
    }

    Allocator( const Allocator& ) = delete;
    Allocator( const Allocator&& ) = delete;
    auto operator=( const Allocator& ) -> Allocator& = delete;
    auto operator=( const Allocator&& ) -> Allocator& = delete;

    virtual void Init()
    {
    }

    virtual auto Allocate( size_t size, bool shouldAlign ) -> void* = 0;
    virtual void Free( void* ptr ) = 0;

    [[nodiscard]] auto GetSize() const -> size_t
    {
        return _size;
    }

private:
    size_t _size;
};

}  // namespace WindEngine::Core::Memory

#endif  // WINDENGINE_ALLOCATOR_HPP
