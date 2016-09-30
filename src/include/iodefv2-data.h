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

#ifndef _LIBIODEFV2_IODEFV2_DATA_H
#define _LIBIODEFV2_IODEFV2_DATA_H

#include "libiodefv2-list.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IODEFV2_DATA_TYPE_UNKNOWN      = 0,
        IODEFV2_DATA_TYPE_CHAR         = 1,
        IODEFV2_DATA_TYPE_BYTE         = 2,
        IODEFV2_DATA_TYPE_UINT32       = 3,
        IODEFV2_DATA_TYPE_UINT64       = 4,
        IODEFV2_DATA_TYPE_INT          = 4,
        IODEFV2_DATA_TYPE_FLOAT        = 5,
        IODEFV2_DATA_TYPE_CHAR_STRING  = 6,
        IODEFV2_DATA_TYPE_BYTE_STRING  = 7,
        IODEFV2_DATA_TYPE_TIME         = 8
} iodefv2_data_type_t;


typedef struct {
        int refcount;

        int flags;
        iodefv2_data_type_t type;
        size_t len;

        union {
                char char_data;
                uint8_t byte_data;
                int64_t int_data;
                float float_data;
                void *rw_data;
                const void *ro_data;
        } data;

        libiodefv2_list_t list;
} iodefv2_data_t;



int iodefv2_data_new(iodefv2_data_t **data);

iodefv2_data_t *iodefv2_data_ref(iodefv2_data_t *data);


int iodefv2_data_new_char(iodefv2_data_t **data, char c);
int iodefv2_data_new_byte(iodefv2_data_t **data, uint8_t i);
int iodefv2_data_new_uint32(iodefv2_data_t **data, uint32_t i) LIBIODEFV2_DEPRECATED;
int iodefv2_data_new_uint64(iodefv2_data_t **data, uint64_t i) LIBIODEFV2_DEPRECATED;
int iodefv2_data_new_int(iodefv2_data_t **data, int64_t i);
int iodefv2_data_new_float(iodefv2_data_t **data, float f);
int iodefv2_data_new_time(iodefv2_data_t **data, iodefv2_time_t *time);

void iodefv2_data_set_char(iodefv2_data_t *data, char c);
void iodefv2_data_set_byte(iodefv2_data_t *data, uint8_t i);
void iodefv2_data_set_uint32(iodefv2_data_t *data, uint32_t i) LIBIODEFV2_DEPRECATED;
void iodefv2_data_set_uint64(iodefv2_data_t *data, uint64_t i) LIBIODEFV2_DEPRECATED;
void iodefv2_data_set_int(iodefv2_data_t *data, int64_t i);
void iodefv2_data_set_float(iodefv2_data_t *data, float f);
void iodefv2_data_set_time(iodefv2_data_t *data, iodefv2_time_t *time);

int iodefv2_data_set_ptr_dup_fast(iodefv2_data_t *data, iodefv2_data_type_t type, const void *ptr, size_t len);
int iodefv2_data_set_ptr_ref_fast(iodefv2_data_t *data, iodefv2_data_type_t type, const void *ptr, size_t len);
int iodefv2_data_set_ptr_nodup_fast(iodefv2_data_t *data, iodefv2_data_type_t type, void *ptr, size_t len);

int iodefv2_data_new_ptr_dup_fast(iodefv2_data_t **data, iodefv2_data_type_t type, const void *ptr, size_t len);
int iodefv2_data_new_ptr_ref_fast(iodefv2_data_t **data, iodefv2_data_type_t type, const void *ptr, size_t len);
int iodefv2_data_new_ptr_nodup_fast(iodefv2_data_t **data, iodefv2_data_type_t type, void *ptr, size_t len);


/*
 * String functions
 */
int iodefv2_data_set_char_string_dup_fast(iodefv2_data_t *data, const char *str, size_t len);
int iodefv2_data_new_char_string_dup_fast(iodefv2_data_t **data, const char *str, size_t len);
int iodefv2_data_new_char_string_ref_fast(iodefv2_data_t **data, const char *ptr, size_t len);
int iodefv2_data_new_char_string_nodup_fast(iodefv2_data_t **data, char *ptr, size_t len);
int iodefv2_data_set_char_string_ref_fast(iodefv2_data_t *data, const char *ptr, size_t len);
int iodefv2_data_set_char_string_nodup_fast(iodefv2_data_t *data, char *ptr, size_t len);
int iodefv2_data_new_char_string_ref(iodefv2_data_t **data, const char *ptr);
int iodefv2_data_new_char_string_dup(iodefv2_data_t **data, const char *ptr);
int iodefv2_data_new_char_string_nodup(iodefv2_data_t **data, char *ptr);
int iodefv2_data_set_char_string_ref(iodefv2_data_t *data, const char *ptr);
int iodefv2_data_set_char_string_dup(iodefv2_data_t *data, const char *ptr);
int iodefv2_data_set_char_string_nodup(iodefv2_data_t *data, char *ptr);

#define iodefv2_data_set_char_string_constant(string, str)                \
        iodefv2_data_set_char_string_ref_fast((string), (str), sizeof((str)) - 1)


/*
 * Byte functions
 */
int iodefv2_data_new_byte_string_ref(iodefv2_data_t **data, const unsigned char *ptr, size_t len);
int iodefv2_data_new_byte_string_dup(iodefv2_data_t **data, const unsigned char *ptr, size_t len);
int iodefv2_data_new_byte_string_nodup(iodefv2_data_t **data, unsigned char *ptr, size_t len);
int iodefv2_data_set_byte_string_ref(iodefv2_data_t *data, const unsigned char *ptr, size_t len);
int iodefv2_data_set_byte_string_dup(iodefv2_data_t *data, const unsigned char *ptr, size_t len);
int iodefv2_data_set_byte_string_nodup(iodefv2_data_t *data, unsigned char *ptr, size_t len);



/*
 *
 */

void iodefv2_data_destroy(iodefv2_data_t *data);

int iodefv2_data_copy_ref(const iodefv2_data_t *src, iodefv2_data_t *dst);

int iodefv2_data_copy_dup(const iodefv2_data_t *src, iodefv2_data_t *dst);

int iodefv2_data_clone(const iodefv2_data_t *src, iodefv2_data_t **dst);

iodefv2_data_type_t iodefv2_data_get_type(const iodefv2_data_t *data);

size_t iodefv2_data_get_len(const iodefv2_data_t *data);

const void *iodefv2_data_get_data(const iodefv2_data_t *data);

char iodefv2_data_get_char(const iodefv2_data_t *data);

uint8_t iodefv2_data_get_byte(const iodefv2_data_t *data);

uint32_t iodefv2_data_get_uint32(const iodefv2_data_t *data) LIBIODEFV2_DEPRECATED;

uint64_t iodefv2_data_get_uint64(const iodefv2_data_t *data) LIBIODEFV2_DEPRECATED;

int64_t iodefv2_data_get_int(const iodefv2_data_t *data);

float iodefv2_data_get_float(const iodefv2_data_t *data);

const char *iodefv2_data_get_char_string(const iodefv2_data_t *data);

const unsigned char *iodefv2_data_get_byte_string(const iodefv2_data_t *data);

libiodefv2_bool_t iodefv2_data_is_empty(const iodefv2_data_t *data);

int iodefv2_data_to_string(const iodefv2_data_t *data, libiodefv2_string_t *out);

void iodefv2_data_destroy_internal(iodefv2_data_t *data);

int iodefv2_data_compare(const iodefv2_data_t *data1, const iodefv2_data_t *data2);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_IODEFV2_DATA_H */
