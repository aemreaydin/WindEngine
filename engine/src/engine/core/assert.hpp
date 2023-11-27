#ifndef WINDENGINE_ASSERT_HPP
#define WINDENGINE_ASSERT_HPP

#if __APPLE__
#define WIND_BREAK __builtin_debugtrap();
#elif _WIN32
#define WIND_BREAK __debugbreak();
#elif __linux
#define WIND_BREAK __builtin_trap();
#endif

void LogAssertFailure( const char* expr, const char* message, const char* file, int line );

#define WIND_ASSERT_MSG( expr, msg )                                                                                   \
    if ( ( expr ) )                                                                                                    \
    {                                                                                                                  \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        LogAssertFailure( #expr, msg, __FILE_NAME__, __LINE__ );                                                       \
        WIND_BREAK                                                                                                     \
    }

#define WIND_ASSERT( expr ) WIND_ASSERT_MSG( expr, "" )

#if NDEBUG
#define WIND_ASSERT_DEBUG( expr, msg )
#else
#define WIND_ASSERT_DEBUG( expr, msg ) WIND_ASSERT_MSG( expr, msg )
#endif

#endif  // WINDENGINE_ASSERT_HPP
