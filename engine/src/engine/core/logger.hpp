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

#define WIND_TRACE( ... )                                                                                              \
    if ( WIND_CHECK_LOGGER )                                                                                           \
        WIND_GET_LOGGER->trace( __VA_ARGS__ );

#define WIND_DEBUG( ... )                                                                                              \
    if ( WIND_CHECK_LOGGER )                                                                                           \
        WIND_GET_LOGGER->debug( __VA_ARGS__ );

#define WIND_INFO( ... )                                                                                               \
    if ( WIND_CHECK_LOGGER )                                                                                           \
        WIND_GET_LOGGER->info( __VA_ARGS__ );

#define WIND_WARN( ... )                                                                                               \
    if ( WIND_CHECK_LOGGER )                                                                                           \
        WIND_GET_LOGGER->warn( __VA_ARGS__ );

#define WIND_ERROR( ... )                                                                                              \
    if ( WIND_CHECK_LOGGER )                                                                                           \
        WIND_GET_LOGGER->error( __VA_ARGS__ );

#define WIND_FATAL( ... )                                                                                              \
    if ( WIND_CHECK_LOGGER )                                                                                           \
        WIND_GET_LOGGER->critical( __VA_ARGS__ );                                                                      \
    throw std::runtime_error( "Fatal error occurred. Exiting..." );

#endif  // WINDENGINE_LOGGER_HPP
