#include "assert.hpp"
#include "logger.hpp"

void G_LOG_ASSERT_FAILURE( const char* expr, const char* message, const char* file, int line )
{
    WindFatal( "ASSERT FAILURE in {} on line {}: Expr(\"{}\") - Msg(\"{}\")", file, line, expr, message );
}
