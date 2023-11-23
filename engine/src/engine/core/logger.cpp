#include "logger.hpp"
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>

#if NDEBUG
constexpr auto kLogLevel = spdlog::level::info;
#else
constexpr auto kLogLevel = spdlog::level::trace;
#endif

void WindEngine::Core::Logger::Initialize()
{
    std::vector<spdlog::sink_ptr> sinks{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
    auto logger = std::make_shared<spdlog::logger>( kLoggerName, sinks.begin(), sinks.end() );
    logger->set_pattern( "%^[%H:%M:%S:%e] (Thread #%t) [%n-%L] > %v%$" );
    logger->set_level( kLogLevel );
    logger->flush_on( kLogLevel );

    spdlog::register_logger( logger );
}

void WindEngine::Core::Logger::Shutdown()
{
    spdlog::shutdown();
}
