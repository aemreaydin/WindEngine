#include "assert.hpp"
#include "logger.hpp"

void G_LOG_ASSERT_FAILURE( const char* expr, const char* message, const char* file, int line )
{
    WIND_FATAL( "ASSERT FAILURE in {} on line {}: Expr(\"{}\") - Msg(\"{}\")", file, line, expr, message )
}
