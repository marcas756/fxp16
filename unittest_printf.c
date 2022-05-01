#include <stdarg.h>
#include <stdio.h>




void unittest_printf_function ( const char * format, ... )
{
    va_list arglist;
    va_start( arglist, format );
    vprintf( format, arglist );
    va_end( arglist );
}
