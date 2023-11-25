#ifndef WINDENGINE_CALLOCATOR_HPP
#define WINDENGINE_CALLOCATOR_HPP

#include "allocator.hpp"

namespace WindEngine::Core::Memory
{

class CAllocator : public Allocator
{
public:
    CAllocator();
    auto Allocate( size_t size, bool shouldAlign ) -> void* override;
    void Free( void* ptr ) override;
};

}  // namespace WindEngine::Core::Memory

#endif  // WINDENGINE_CALLOCATOR_HPP
