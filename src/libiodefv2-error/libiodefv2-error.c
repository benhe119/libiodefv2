#include "config.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gettext.h>
#include <errno.h>

#include "libiodefv2-log.h"
#include "libiodefv2-error.h"

#include "code-to-errno.h"
#include "code-from-errno.h"


#define LIBIODEFV2_ERROR_CODE_DIM     65536
#define LIBIODEFV2_ERROR_SOURCE_DIM     256

#define LIBIODEFV2_ERROR_SOURCE_SHIFT    23
#define LIBIODEFV2_ERROR_VERBOSE_SHIFT   22

#define LIBIODEFV2_ERROR_CODE_MASK       (LIBIODEFV2_ERROR_CODE_DIM - 1)
#define LIBIODEFV2_ERROR_SOURCE_MASK     (LIBIODEFV2_ERROR_SOURCE_DIM - 1)
#define LIBIODEFV2_ERROR_VERBOSE_MASK    (1)


/**
 * libiodefv2_error_make:
 * @source: Error source.
 * @code: Error code.
 *
 * Create a new #libiodefv2_error_t error using @source and @code.
 *
 * Returns: The created #libiodefv2_error_t integer.
 */
libiodefv2_error_t libiodefv2_error_make(libiodefv2_error_source_t source, libiodefv2_error_code_t code)
{
        return (code == LIBIODEFV2_ERROR_NO_ERROR) ? code : -((source << LIBIODEFV2_ERROR_SOURCE_SHIFT) | code);
}


/**
 * libiodefv2_error_make_from_errno:
 * @source: Error source.
 * @err: errno value.
 *
 * Create a new #libiodefv2_error_t error using @source and @errno.
 *
 * Returns: The created #libiodefv2_error_t integer.
 */
libiodefv2_error_t libiodefv2_error_make_from_errno(libiodefv2_error_source_t source, int err)
{
        libiodefv2_error_code_t code = libiodefv2_error_code_from_errno(err);
        return libiodefv2_error_make(source, code);
}



/**
 * libiodefv2_error_verbose_make_v:
 * @source: Error source.
 * @code: Error code.
 * @fmt: Format string.
 * @ap: Argument list.
 *
 * Create a new error using @source and @code, using the detailed error message
 * specified within @fmt.
 *
 * Returns: The created #libiodefv2_error_t integer.
 */
libiodefv2_error_t libiodefv2_error_verbose_make_v(libiodefv2_error_source_t source,
                                             libiodefv2_error_code_t code, const char *fmt, va_list ap)
{
        int ret;

        ret = libiodefv2_error_make(source, code);
        ret = -ret;
        ret |= (1 << LIBIODEFV2_ERROR_VERBOSE_SHIFT);

        return -ret;
}



/**
 * libiodefv2_error_verbose_make:
 * @source: Error source.
 * @code: Error code.
 * @fmt: Format string.
 * @...: Argument list.
 *
 * Create a new error using @source and @code, using the detailed error message
 * specified within @fmt.
 *
 * Returns: The created #libiodefv2_error_t integer.
 */
libiodefv2_error_t libiodefv2_error_verbose_make(libiodefv2_error_source_t source,
                                           libiodefv2_error_code_t code, const char *fmt, ...)
{
        int ret;
        va_list ap;

        va_start(ap, fmt);
        ret = libiodefv2_error_verbose_make_v(source, code, fmt, ap);
        va_end(ap);

        return ret;
}


/**
 * libiodefv2_error_get_code:
 * @error: A #libiodefv2_error_t return value.
 *
 * Returns: the #libiodefv2_code_t code contained within the @libiodefv2_error_t integer.
 */
libiodefv2_error_code_t libiodefv2_error_get_code(libiodefv2_error_t error)
{
        error = -error;
        return (libiodefv2_error_code_t) (error & LIBIODEFV2_ERROR_CODE_MASK);
}


/**
 * libiodefv2_error_get_source:
 * @error: A #libiodefv2_error_t return value.
 *
 * Returns: the #libiodefv2_source_t source contained within the @libiodefv2_error_t integer.
 */
libiodefv2_error_source_t libiodefv2_error_get_source(libiodefv2_error_t error)
{
        error = -error;
        return (libiodefv2_error_source_t) ((error >> LIBIODEFV2_ERROR_SOURCE_SHIFT) & LIBIODEFV2_ERROR_SOURCE_MASK);
}


/**
 * libiodefv2_error_is_verbose:
 * @error: A #libiodefv2_error_t return value.
 *
 * Returns: #LIBIODEFV2_BOOL_TRUE if there is a detailed message for this error, #LIBIODEFV2_BOOL_FALSE otherwise.
 */
libiodefv2_bool_t libiodefv2_error_is_verbose(libiodefv2_error_t error)
{
        error = -error;
        return ((error >> LIBIODEFV2_ERROR_VERBOSE_SHIFT) & LIBIODEFV2_ERROR_VERBOSE_MASK) ? LIBIODEFV2_BOOL_TRUE : LIBIODEFV2_BOOL_FALSE;
}


/**
 * libiodefv2_error_code_from_errno:
 * @err: errno value.
 *
 * Returns: the #libiodefv2_error_code_t value corresponding to @err.
 */
libiodefv2_error_code_t libiodefv2_error_code_from_errno(int err)
{
        int idx;

        if ( ! err )
                return LIBIODEFV2_ERROR_NO_ERROR;

        idx = errno_to_idx(err);
        if ( idx < 0 )
                return LIBIODEFV2_ERROR_UNKNOWN_ERRNO;

        return LIBIODEFV2_ERROR_SYSTEM_ERROR | err_code_from_index[idx];
}


/**
 * libiodefv2_error_code_to_errno:
 * @code: Error code.
 *
 * Returns: the errno value corresponding to @code.
 */
int libiodefv2_error_code_to_errno(libiodefv2_error_code_t code)
{
        if ( ! (code & LIBIODEFV2_ERROR_SYSTEM_ERROR) )
                return 0;

        code &= ~LIBIODEFV2_ERROR_SYSTEM_ERROR;

        if ( code < sizeof(err_code_to_errno) / sizeof(err_code_to_errno[0]) )
                return err_code_to_errno[code];
        else
                return 0;
}



/**
 * libiodefv2_perror:
 * @error: A #libiodefv2_error_t return value.
 * @fmt: Format string.
 * @...: Argument list.
 *
 * Print the error to stderr, or to syslog() in case stderr is unavailable.
 */
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
