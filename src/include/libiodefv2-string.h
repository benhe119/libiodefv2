/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_LIBIODEFV2_STRING_H
#define _LIBIODEFV2_LIBIODEFV2_STRING_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdarg.h>

#include "libiodefv2-list.h"
#include "libiodefv2-macros.h"
#include "libiodefv2-inttypes.h"
#include "libiodefv2-linked-object.h"

#ifdef __cplusplus
 extern "C" {
#endif


typedef struct libiodefv2_string libiodefv2_string_t;


int libiodefv2_string_new(libiodefv2_string_t **string);

int libiodefv2_string_new_nodup(libiodefv2_string_t **string, char *str);

int libiodefv2_string_new_ref(libiodefv2_string_t **string, const char *str);

int libiodefv2_string_new_dup(libiodefv2_string_t **string, const char *str);

int libiodefv2_string_new_dup_fast(libiodefv2_string_t **string, const char *str, size_t len);

void libiodefv2_string_destroy(libiodefv2_string_t *string);

void libiodefv2_string_destroy_internal(libiodefv2_string_t *string);

int libiodefv2_string_new_nodup_fast(libiodefv2_string_t **string, char *str, size_t len);

int libiodefv2_string_new_ref_fast(libiodefv2_string_t **string, const char *str, size_t len);

int libiodefv2_string_set_dup_fast(libiodefv2_string_t *string, const char *buf, size_t len);

int libiodefv2_string_set_dup(libiodefv2_string_t *string, const char *buf);

int libiodefv2_string_set_nodup_fast(libiodefv2_string_t *string, char *buf, size_t len);

int libiodefv2_string_set_nodup(libiodefv2_string_t *string, char *buf);

int libiodefv2_string_set_ref_fast(libiodefv2_string_t *string, const char *buf, size_t len);

int libiodefv2_string_set_ref(libiodefv2_string_t *string, const char *buf);

int libiodefv2_string_copy_ref(const libiodefv2_string_t *src, libiodefv2_string_t *dst);

int libiodefv2_string_copy_dup(const libiodefv2_string_t *src, libiodefv2_string_t *dst);

libiodefv2_string_t *libiodefv2_string_ref(libiodefv2_string_t *string);

int libiodefv2_string_clone(const libiodefv2_string_t *src, libiodefv2_string_t **dst);

size_t libiodefv2_string_get_len(const libiodefv2_string_t *string);

const char *libiodefv2_string_get_string_or_default(const libiodefv2_string_t *string, const char *def);

const char *libiodefv2_string_get_string(const libiodefv2_string_t *string);

int libiodefv2_string_get_string_released(libiodefv2_string_t *string, char **outptr);

libiodefv2_bool_t libiodefv2_string_is_empty(const libiodefv2_string_t *string);

int libiodefv2_string_truncate(libiodefv2_string_t *string, size_t len);

void libiodefv2_string_clear(libiodefv2_string_t *string);

/*
 * string operation
 */
int libiodefv2_string_cat(libiodefv2_string_t *dst, const char *str);
int libiodefv2_string_ncat(libiodefv2_string_t *dst, const char *str, size_t len);

int libiodefv2_string_sprintf(libiodefv2_string_t *string, const char *fmt, ...)
                           LIBIODEFV2_FMT_PRINTF(2, 3);

int libiodefv2_string_vprintf(libiodefv2_string_t *string, const char *fmt, va_list ap)
                           LIBIODEFV2_FMT_PRINTF(2, 0);

int libiodefv2_string_compare(const libiodefv2_string_t *str1, const libiodefv2_string_t *str2);

#define libiodefv2_string_set_constant(string, str)                         \
        libiodefv2_string_set_ref_fast((string), (str), strlen((str)))

#define libiodefv2_string_new_constant(string, str)                         \
        libiodefv2_string_new_ref_fast((string), (str), strlen((str)))

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_LIBIODEFV2_STRING_H */
