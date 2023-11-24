#ifndef WINDENGINE_LINEARALLOCATOR_HPP
#define WINDENGINE_LINEARALLOCATOR_HPP

#include "allocator.hpp"

namespace WindEngine::Core::Memory
{

class LinearAllocator : public Allocator
{
public:
    explicit LinearAllocator( size_t size );

    void Init();
    auto Allocate( size_t size, bool shouldAlign ) -> void* override;
    void Free( void* ptr ) override;
    void Reset();

private:
    std::byte* _pStart{ nullptr };
    size_t _offset{ 0 };
};

}  // namespace WindEngine::Core::Memory

#endif  // WINDENGINE_LINEARALLOCATOR_HPP
