#include "appConfig.h"
#include <utility>

namespace WindEngine
{

AppConfig::AppConfig( std::string appName, U32 width, U32 height )
  : appName( std::move( appName ) ), width( width ), height( height )
{
}

}  // namespace WindEngine
