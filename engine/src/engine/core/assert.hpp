#ifndef WINDENGINE_ASSERT_HPP
#define WINDENGINE_ASSERT_HPP

#if __APPLE__
#define WIND_BREAK __builtin_debugtrap();
#elif _WIN32
#define WIND_BREAK __debugbreak();
#elif __linux
#define WIND_BREAK __builtin_trap();
#endif

#define STRINGIZE( expr ) #expr

void G_LOG_ASSERT_FAILURE( const char* expr, const char* message, const char* file, int line );

template <typename T, typename M> void G_ASSERT_MSG( T expr, M msg )
{
    if ( !expr )
    {
        G_LOG_ASSERT_FAILURE( STRINGIZE(expr), msg, __FILE_NAME__, __LINE__ );
        WIND_BREAK
    }
}

template <typename T> void G_ASSERT( T expr )
{
    if ( !expr )
    {
        G_LOG_ASSERT_FAILURE( STRINGIZE(expr), "", __FILE_NAME__, __LINE__ );
        WIND_BREAK
    }
}

// #if defined( NDEBUG )
// #define WIND_ASSERT_DEBUG( expr, msg )
// #else
// #define WIND_ASSERT_DEBUG( expr, msg ) WIND_ASSERT_MSG( expr, msg )
// #endif

#endif  // WINDENGINE_ASSERT_HPP
