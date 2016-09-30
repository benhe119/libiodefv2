/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@libiodefv2-ids.com>
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

#ifndef _LIBIODEFV2_IODEFV2_TIME_H
#define _LIBIODEFV2_IODEFV2_TIME_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef __cplusplus
 extern "C" {
#endif

struct iodefv2_time {
        /* <private> */
        int refcount;
        uint32_t sec;
        uint32_t usec;
        int32_t gmt_offset;
};

typedef struct iodefv2_time iodefv2_time_t;

iodefv2_time_t *iodefv2_time_ref(iodefv2_time_t *time);
int iodefv2_time_new(iodefv2_time_t **time);

int iodefv2_time_new_from_time(iodefv2_time_t **time, const time_t *t);
int iodefv2_time_new_from_gettimeofday(iodefv2_time_t **time);
int iodefv2_time_new_from_string(iodefv2_time_t **time, const char *buf);
int iodefv2_time_new_from_ntpstamp(iodefv2_time_t **time, const char *buf);
int iodefv2_time_new_from_timeval(iodefv2_time_t **time, const struct timeval *tv);

void iodefv2_time_set_from_time(iodefv2_time_t *time, const time_t *t);
int iodefv2_time_set_from_gettimeofday(iodefv2_time_t *time);
int iodefv2_time_set_from_string(iodefv2_time_t *time, const char *buf);
int iodefv2_time_set_from_ntpstamp(iodefv2_time_t *time, const char *buf);
int iodefv2_time_set_from_timeval(iodefv2_time_t *time, const struct timeval *tv);

void iodefv2_time_destroy_internal(iodefv2_time_t *time);
void iodefv2_time_destroy(iodefv2_time_t *time);

int iodefv2_time_clone(const iodefv2_time_t *src, iodefv2_time_t **dst);
int iodefv2_time_copy(const iodefv2_time_t *src, iodefv2_time_t *dst);

void iodefv2_time_set_sec(iodefv2_time_t *time, uint32_t sec);
void iodefv2_time_set_usec(iodefv2_time_t *time, uint32_t usec);
void iodefv2_time_set_gmt_offset(iodefv2_time_t *time, int32_t gmtoff);

uint32_t iodefv2_time_get_sec(const iodefv2_time_t *time);
uint32_t iodefv2_time_get_usec(const iodefv2_time_t *time);
int32_t iodefv2_time_get_gmt_offset(const iodefv2_time_t *time);

int iodefv2_time_to_string(const iodefv2_time_t *time, libiodefv2_string_t *out);
int iodefv2_time_to_ntpstamp(const iodefv2_time_t *time, libiodefv2_string_t *out);

int iodefv2_time_compare(const iodefv2_time_t *time1, const iodefv2_time_t *time2);
         
#ifdef __cplusplus
 }
#endif
         
#endif /* _LIBIODEFV2_IODEFV2_TIME_H */
