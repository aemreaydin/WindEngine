#ifndef WINDENGINE_APPCONFIG_H
#define WINDENGINE_APPCONFIG_H

#include "defines.hpp"
#include <string>

namespace WindEngine
{

struct AppConfig
{
    std::string appName {};
    U32 width {};
    U32 height {};

    AppConfig( std::string appName, U32 width, U32 height );
};

}  // namespace WindEngine

#endif  // WINDENGINE_APPCONFIG_H
