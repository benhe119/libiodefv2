#include <stdio.h>
#include <stdarg.h>

#include "libiodefv2-log.h"
#include "libiodefv2-error.h"


void libiodefv2_perror(libiodefv2_error_t error, const char *fmt, ...)
{
	va_list ap;
	char buf[1024];
        
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

        if ( libiodefv2_error_get_source(error) )
                libiodefv2_log(LIBIODEFV2_LOG_WARN, "%s: %s: %s.\n", libiodefv2_strsource(error), buf, libiodefv2_strerror(error));
        else
                libiodefv2_log(LIBIODEFV2_LOG_WARN, "%s: %s.\n", buf, libiodefv2_strerror(error));
}
