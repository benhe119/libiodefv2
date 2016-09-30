/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_COMMON_H
#define _LIBIODEFV2_COMMON_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "iodefv2.h"
#include "libiodefv2-inttypes.h"
#include "libiodefv2-log.h"
#include <sys/types.h>

#ifdef WIN32
# include <winsock2.h>
#else
# include <sys/socket.h>
# include <netinet/in.h>
#endif

#include <time.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define libiodefv2_return_val_if_fail(cond, val) do {                               \
        if ( ! (cond) ) {                                                        \
                libiodefv2_log(LIBIODEFV2_LOG_CRIT, "assertion '%s' failed\n", #cond); \
                return val;                                                      \
        }                                                                        \
} while(0)


#define libiodefv2_return_if_fail(cond) do {                                        \
        if ( ! (cond) ) {                                                        \
                libiodefv2_log(LIBIODEFV2_LOG_CRIT, "assertion '%s' failed\n", #cond); \
                return;                                                          \
        }                                                                        \
} while(0)


int libiodefv2_parse_address(const char *str, char **addr, unsigned int *port);

uint64_t libiodefv2_hton64(uint64_t val);

uint32_t libiodefv2_htonf(float fval);

time_t libiodefv2_timegm(struct tm *tm);

int libiodefv2_get_gmt_offset(long *gmt_offset);

int libiodefv2_get_gmt_offset_from_tm(struct tm *tm, long *gmtoff);

int libiodefv2_get_gmt_offset_from_time(const time_t *utc, long *gmtoff);

int libiodefv2_read_multiline(FILE *fd, unsigned int *line, char *buf, size_t size);

int libiodefv2_read_multiline2(FILE *fd, unsigned int *line, libiodefv2_string_t *out);

void *libiodefv2_sockaddr_get_inaddr(struct sockaddr *sa);

void *_libiodefv2_realloc(void *ptr, size_t size);

int _libiodefv2_load_file(const char *filename, unsigned char **fdata, size_t *outsize);

void _libiodefv2_unload_file(unsigned char *fdata, size_t size);

double libiodefv2_simple_strtod(const char *s, char **endptr);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_COMMON_H */
