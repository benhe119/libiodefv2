/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodefv2-ids.com>
*
* This file is part of the LibIodef library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>

#if !((defined _WIN32 || defined __WIN32__) && !defined __CYGWIN__)
# include <syslog.h>
#else
# include <windows.h>
#endif

#include "common.h"
#include "libiodefv2-log.h"
#include "libiodefv2-error.h"
#include "libiodefv2-inttypes.h"


#if ! defined (LIBIODEFV2_VA_COPY)

# if defined (__GNUC__) && defined (__PPC__) && (defined (_CALL_SYSV) || defined (_WIN32))
#  define LIBIODEFV2_VA_COPY(ap1, ap2)     (*(ap1) = *(ap2))

# elif defined (LIBIODEFV2_VA_COPY_AS_ARRAY)
#  define LIBIODEFV2_VA_COPY(ap1, ap2)     memmove ((ap1), (ap2), sizeof(va_list))

# else /* va_list is a pointer */
#  define LIBIODEFV2_VA_COPY(ap1, ap2)     ((ap1) = (ap2))

# endif
#endif


static int do_log_print(libiodefv2_log_t level, const char *str);


static int debug_level = 0;
static int raise_abort_level = 0;
libiodefv2_bool_t raise_abort_set = FALSE;
static int log_level = LIBIODEFV2_LOG_INFO;

static char *global_prefix = NULL;

static FILE *debug_logfile = NULL;
static libiodefv2_log_flags_t log_flags = 0;
static int (*global_log_cb)(libiodefv2_log_t level, const char *str) = do_log_print;
static void (*external_log_cb)(libiodefv2_log_t level, const char *str) = NULL;


#if (defined _WIN32 || defined __WIN32__) && !defined __CYGWIN__
#define LOG_CRIT     EVENTLOG_WARNING_TYPE
#define LOG_ERR      EVENTLOG_ERROR_TYPE
#define LOG_WARNING  EVENTLOG_WARNING_TYPE
#define LOG_INFO     EVENTLOG_INFORMATION_TYPE
#define LOG_DEBUG    EVENTLOG_INFORMATION_TYPE

static void syslog_win32(int priority, const char *log)
{
        static HANDLE event_source = NULL;

        if ( ! event_source )
                event_source = RegisterEventSource(NULL, TEXT("LibIodef"));

        ReportEvent(event_source,    /* handle of event source */
                    (WORD)priority,  /* event type */
                    0,               /* event category */
                    0,               /* event ID */
                    NULL,            /* current user's SID */
                    1,               /* strings in szMsg */
                    0,               /* no bytes of raw data */
                    &log,            /* array of error strings */
                    NULL);           /* no raw data */
}
#endif


static int do_log_syslog(libiodefv2_log_t level, const char *str)
{
        int slevel = LOG_INFO;

        if ( level == LIBIODEFV2_LOG_CRIT )
                slevel = LOG_CRIT;

        else if ( level == LIBIODEFV2_LOG_ERR )
                slevel = LOG_ERR;

        else if ( level == LIBIODEFV2_LOG_WARN )
                slevel = LOG_WARNING;

        else if ( level == LIBIODEFV2_LOG_INFO )
                slevel = LOG_INFO;

        else if ( level >= LIBIODEFV2_LOG_DEBUG )
                slevel = LOG_DEBUG;

        while (*str == '\n' ) str++;

#if !((defined _WIN32 || defined __WIN32__) && !defined __CYGWIN__)
        syslog(slevel, "%s", str);
#else
        syslog_win32(slevel, str);
#endif

        return 0;
}


static inline FILE *get_out_fd(libiodefv2_log_t level)
{
        return (level < LIBIODEFV2_LOG_INFO) ? stderr : stdout;
}



static int do_log_print(libiodefv2_log_t level, const char *str)
{
        int ret;
        FILE *out = get_out_fd(level);

        ret = fprintf(out, "%s", str);
        if ( ret < 0 && errno == EBADF )
                return -1;

        return 0;
}


static int do_log_external(libiodefv2_log_t level, const char *str)
{
        external_log_cb(level, str);
        return 0;
}


static inline libiodefv2_bool_t need_to_log(libiodefv2_log_t level, libiodefv2_log_t cur)
{
        return (level > cur) ? FALSE : TRUE;
}


static const char *level_to_string(libiodefv2_log_t level)
{
        if ( level >= LIBIODEFV2_LOG_DEBUG )
                return "DEBUG";

        else if ( level == LIBIODEFV2_LOG_ERR )
                return "ERROR";

        else if ( level == LIBIODEFV2_LOG_INFO )
                return "INFO";

        else if ( level == LIBIODEFV2_LOG_WARN )
                return "WARNING";

        else if ( level == LIBIODEFV2_LOG_CRIT )
                return "CRITICAL";

        return "";
}


static ssize_t get_header(libiodefv2_log_t level, char *buf, size_t size)
{
        int ret;
        struct tm *t;
        size_t len = 0;
        time_t now = time(NULL);

        buf[0] = 0;

        if ( ! (log_flags & LIBIODEFV2_LOG_FLAGS_SYSLOG) ) {
                t = localtime(&now);
                if ( t )
                        len = strftime(buf, size, "%d %b %H:%M:%S ", t);

                ret = snprintf(buf + len, size - len, "(process:%d) %s: ", (int) getpid(), level_to_string(level));
                if ( ret < 0 || (size_t) ret >= (size - len) )
                        return -1;
        } else {
                ret = snprintf(buf + len, size - len, "%s: ", level_to_string(level));
                if ( ret < 0 || (size_t) ret >= (size - len) )
                        return -1;
        }

        return (size_t) len + ret;
}


static void do_log_v(libiodefv2_log_t level, const char *file,
                     const char *function, int line, const char *fmt, va_list ap)
{
        int ret;
        va_list bkp;
        char buf[1024];
        ssize_t len = 0;

        if ( global_log_cb == do_log_print || global_log_cb == do_log_syslog ) {
                len = get_header(level, buf, sizeof(buf));
                if ( len < 0 )
                        return;
        }

        LIBIODEFV2_VA_COPY(bkp, ap);

        ret = vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
        if ( ret < 0 || (size_t) ret >= (sizeof(buf) - len) )
                goto out;

        if ( level <= LIBIODEFV2_LOG_ERR || level >= LIBIODEFV2_LOG_DEBUG ) {
                for ( len += ret; buf[len - 1] == '\n' ; len-- );
                snprintf(buf + len, sizeof(buf) - len, " (%s:%d %s)\n", file, line, function);
        }

        if ( need_to_log(level, log_level) || need_to_log(level, debug_level) ) {
                ret = global_log_cb(level, buf);
                if ( ret < 0 && global_log_cb == do_log_print ) {
                        libiodefv2_log_set_flags(LIBIODEFV2_LOG_FLAGS_SYSLOG);
                        do_log_v(level, file, function, line, fmt, bkp);
                        goto out;
                }

                if ( debug_logfile )
                        fprintf(debug_logfile, "%s", buf);
        }

out:
        va_end(bkp);
}



void _libiodefv2_log_v(libiodefv2_log_t level, const char *file,
                    const char *function, int line, const char *fmt, va_list ap)
{
        if ( ! need_to_log(level, log_level) && ! need_to_log(level, debug_level) )
                return;

        do_log_v(level, file, function, line, fmt, ap);
}



/**
 * _libiodefv2_log:
 * @level: LIBIODEFV2_LOG_PRIORITY_INFO or LIBIODEFV2_LOG_PRIORITY_ERROR.
 * @file: The caller filename.
 * @function: The caller function name.
 * @line: The caller line number.
 * @fmt: Format string.
 * @...: Variable argument list.
 *
 * This function should not be called directly.
 * Use the #log macro defined in libiodefv2-log.h
 */
void _libiodefv2_log(libiodefv2_log_t level, const char *file,
                  const char *function, int line, const char *fmt, ...)
{
        va_list ap;

        if ( ! need_to_log(level, log_level) && ! need_to_log(level, debug_level) )
                return;

        va_start(ap, fmt);
        do_log_v(level, file, function, line, fmt, ap);
        va_end(ap);

        if ( raise_abort_set && level == raise_abort_level )
                abort();
}



/**
 * libiodefv2_log_set_flags:
 * @flags:
 */
void libiodefv2_log_set_flags(libiodefv2_log_flags_t flags)
{
        if ( flags & LIBIODEFV2_LOG_FLAGS_QUIET )
                log_level = LIBIODEFV2_LOG_WARN;

        if ( flags & LIBIODEFV2_LOG_FLAGS_SYSLOG )
                global_log_cb = do_log_syslog;
        else
                global_log_cb = do_log_print;

        log_flags = flags;
}


libiodefv2_log_flags_t libiodefv2_log_get_flags(void)
{
        return log_flags;
}



void libiodefv2_log_set_level(libiodefv2_log_t level)
{
        log_level = level;
}


void libiodefv2_log_set_debug_level(int level)
{
        debug_level = LIBIODEFV2_LOG_DEBUG + level;
}



/**
 * libiodefv2_log_set_prefix:
 * @prefix: Pointer to the prefix to use.
 *
 * Tell the LibIodef standard logger to add @prefix before logging
 * a line.
 */
void libiodefv2_log_set_prefix(char *prefix)
{
        global_prefix = prefix;
}



char *libiodefv2_log_get_prefix(void)
{
        return global_prefix;
}



/**
 * libiodefv2_log_set_callback:
 * @log_cb: Callback function.
 *
 * @log_cb() will be called in place of the libiodefv2 function usally
 * used for loging.
 */
void libiodefv2_log_set_callback(void log_cb(libiodefv2_log_t level, const char *str))
{
        external_log_cb = log_cb;
        global_log_cb = do_log_external;
}



int libiodefv2_log_set_logfile(const char *filename)
{
        if ( ! filename && debug_logfile ) {
                fclose(debug_logfile);
                debug_logfile = NULL;
        }

        else {
                debug_logfile = fopen(filename, "a");
                if ( ! debug_logfile )
                        return libiodefv2_error_from_errno(errno);
        }

        return 0;
}


void _libiodefv2_log_set_abort_level(libiodefv2_log_t level)
{
        raise_abort_set = TRUE;
        raise_abort_level = level;
}


int _libiodefv2_log_set_abort_level_from_string(const char *level)
{
        size_t i;
        char *eptr;
        long lvalue;
        struct {
                const char *prefix;
                libiodefv2_log_t level;
        } tbl[] = {
                { "CRIT",  LIBIODEFV2_LOG_CRIT  },
                { "ERR",   LIBIODEFV2_LOG_ERR   },
                { "WARN",  LIBIODEFV2_LOG_WARN  },
                { "INFO",  LIBIODEFV2_LOG_INFO  },
                { "DEBUG", LIBIODEFV2_LOG_DEBUG }
        };

        libiodefv2_return_val_if_fail(level != NULL, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        for ( i = 0; i < sizeof(tbl) / sizeof(*tbl); i++ ) {
                if ( strncasecmp(tbl[i].prefix, level, strlen(tbl[i].prefix)) == 0 ) {
                        _libiodefv2_log_set_abort_level(tbl[i].level);
                        return 0;
                }
        }

        lvalue = strtol(level, &eptr, 10);
        if ( eptr != (level + strlen(level)) || lvalue == LONG_MIN || lvalue == LONG_MAX ) {
                libiodefv2_log(LIBIODEFV2_LOG_WARN, "Invalid abort level specified: '%s'.\n", level);
                return -1;
        }

        _libiodefv2_log_set_abort_level(lvalue);

        return 0;
}


#ifndef HAVE_VARIADIC_MACROS

void libiodefv2_log(libiodefv2_log_t level, const char *fmt, ...)
{
        va_list ap;

        va_start(ap, fmt);
        _libiodefv2_log_v(level, "", "", 0, fmt, ap);
        va_end(ap);
}


void libiodefv2_log_debug(libiodefv2_log_t level, const char *fmt, ...)
{
        va_list ap;

        va_start(ap, fmt);
        _libiodefv2_log_v(LIBIODEFV2_LOG_DEBUG + level, "", "", 0, fmt, ap);
        va_end(ap);
}

#endif
