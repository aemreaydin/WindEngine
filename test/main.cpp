#include <engine/core/assert.hpp>
#include <engine/core/logger.hpp>

auto main() -> int
{
    WindEngine::Core::Logger::Initialize();
    WIND_TRACE( "Testing logger v{}", 0.0 );
    WIND_DEBUG( "Testing logger v{}", 0.0 );
    WIND_INFO( "Testing logger v{}", 0.0 );
    WIND_WARN( "Testing logger v{}", 0.0 );
    WIND_ERROR( "Testing logger v{}", 0.0 );
    WIND_FATAL( "Testing logger v{}", 0.0 );

    WIND_ASSERT_DEBUG( 1 == 0, "Failed assertion." )

    WindEngine::Core::Logger::Shutdown();
    return 0;
}