#ifndef WINDENGINE_ASSERT_HPP
#define WINDENGINE_ASSERT_HPP

#include "logger.hpp"

inline void WindBreak()
{
#if defined( __APPLE__ )
    __builtin_debugtrap();
#elif _WIN32
    __debugbreak();
#elif __linux
    __builtin_trap();
#endif
}

template <typename T, typename M> void WindAssert( T expr, M msg )
{
    if ( !expr )
    {
        WindFatal( R"(ASSERT FAILURE in {} on line {}: Msg("{}"))", __FILE_NAME__, __LINE__, msg );
        WindBreak();
    }
}

#endif  // WINDENGINE_ASSERT_HPP
