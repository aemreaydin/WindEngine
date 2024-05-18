#include "assert.hpp"
#include "logger.hpp"

void LogAssertFailure( const char* expr, const char* message, const char* file, int line )
{
    WindFatal( R"(ASSERT FAILURE in {} on line {}: Expr("{}") - Msg("{}"))", file, line, expr, message );
}
