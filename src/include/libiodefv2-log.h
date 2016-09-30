/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_LIBIODEFV2_LOG_H
#define _LIBIODEFV2_LIBIODEFV2_LOG_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libiodefv2-macros.h"
#include <stdarg.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        LIBIODEFV2_LOG_CRIT  = -1,
        LIBIODEFV2_LOG_ERR   =  0,
        LIBIODEFV2_LOG_WARN  =  1,
        LIBIODEFV2_LOG_INFO  =  2,
        LIBIODEFV2_LOG_DEBUG  = 3
} libiodefv2_log_t;


typedef enum {
        LIBIODEFV2_LOG_FLAGS_QUIET  = 0x01, /* Drop LIBIODEFV2_LOG_PRIORITY_INFO */
        LIBIODEFV2_LOG_FLAGS_SYSLOG = 0x02
} libiodefv2_log_flags_t;



void _libiodefv2_log_v(libiodefv2_log_t level, const char *file,
                    const char *function, int line, const char *fmt, va_list ap)
                    LIBIODEFV2_FMT_PRINTF(5, 0);

void _libiodefv2_log(libiodefv2_log_t level, const char *file,
                  const char *function, int line, const char *fmt, ...)
                  LIBIODEFV2_FMT_PRINTF(5, 6);


#ifdef HAVE_VARIADIC_MACROS

#define libiodefv2_log(level, ...) \
        _libiodefv2_log(level, __FILE__, __LIBIODEFV2_FUNC__, __LINE__, __VA_ARGS__)

#define libiodefv2_log_debug(level, ...) \
        _libiodefv2_log(LIBIODEFV2_LOG_DEBUG + level, __FILE__, __LIBIODEFV2_FUNC__, __LINE__, __VA_ARGS__)
#else

void libiodefv2_log(libiodefv2_log_t level, const char *fmt, ...)
                 LIBIODEFV2_FMT_PRINTF(2, 3);

void libiodefv2_log_debug(libiodefv2_log_t level, const char *fmt, ...)
                       LIBIODEFV2_FMT_PRINTF(2, 3);

#endif


#define libiodefv2_log_v(level, fmt, ap) \
        _libiodefv2_log_v(level, __FILE__, __LIBIODEFV2_FUNC__, __LINE__, fmt, ap)

#define libiodefv2_log_debug_v(level, fmt, ap) \
        _libiodefv2_log_v(LIBIODEFV2_LOG_DEBUG + level, __FILE__, __LIBIODEFV2_FUNC__, __LINE__, fmt, ap)


void libiodefv2_log_set_level(libiodefv2_log_t level);

void libiodefv2_log_set_debug_level(int level);

libiodefv2_log_flags_t libiodefv2_log_get_flags(void);

void libiodefv2_log_set_flags(libiodefv2_log_flags_t flags);

char *libiodefv2_log_get_prefix(void);

void libiodefv2_log_set_prefix(char *prefix);

void libiodefv2_log_set_callback(void log_cb(libiodefv2_log_t level, const char *str));

int libiodefv2_log_set_logfile(const char *filename);

void _libiodefv2_log_set_abort_level(libiodefv2_log_t level);

int _libiodefv2_log_set_abort_level_from_string(const char *level);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_LIBIODEFV2_LOG_H */
