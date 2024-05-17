#ifndef WINDENGINE_LOGGER_HPP
#define WINDENGINE_LOGGER_HPP

#include "defines.hpp"
#include <spdlog/spdlog.h>

constexpr const char* kLoggerName = "WindLogger";

namespace WindEngine::Core
{

class WINDAPI Logger
{
public:
    static void Initialize();
    static void Shutdown();
};

}  // namespace WindEngine::Core

#define WIND_GET_LOGGER spdlog::get( kLoggerName )
#define WIND_CHECK_LOGGER WIND_GET_LOGGER != nullptr

template <typename... Args>
void WindLog( spdlog::level::level_enum level, spdlog::format_string_t<Args...> fmt, Args&&... args )
{
    const auto& logger = spdlog::get( kLoggerName );
    if ( logger != nullptr )
    {
        logger->log( level, fmt, std::forward<Args>( args )... );
    }
}

template <typename... Args> void WindTrace( spdlog::format_string_t<Args...> fmt, Args&&... args )
{
    WindLog( spdlog::level::trace, fmt, std::forward<Args>( args )... );
}

template <typename... Args> void WindDebug( spdlog::format_string_t<Args...> fmt, Args&&... args )
{
    WindLog( spdlog::level::debug, fmt, std::forward<Args>( args )... );
}

template <typename... Args> void WindInfo( spdlog::format_string_t<Args...> fmt, Args&&... args )
{
    WindLog( spdlog::level::info, fmt, std::forward<Args>( args )... );
}

template <typename... Args> void WindWarn( spdlog::format_string_t<Args...> fmt, Args&&... args )
{
    WindLog( spdlog::level::warn, fmt, std::forward<Args>( args )... );
}

template <typename... Args> void WindError( spdlog::format_string_t<Args...> fmt, Args&&... args )
{
    WindLog( spdlog::level::err, fmt, std::forward<Args>( args )... );
}

template <typename... Args> [[noreturn]] void WindFatal( spdlog::format_string_t<Args...> fmt, Args&&... args )
{
    WindLog( spdlog::level::critical, fmt, std::forward<Args>( args )... );
    throw std::runtime_error( "Fatal error occured. Exiting..." );
}

#endif  // WINDENGINE_LOGGER_HPP
