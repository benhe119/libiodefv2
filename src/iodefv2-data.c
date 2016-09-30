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

#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "libiodefv2-error.h"
#include "libiodefv2-inttypes.h"
#include "libiodefv2-string.h"
#include "common.h"
#include "iodefv2-data.h"


/*
 * Data structure may be free'd
 */
#define IODEFV2_DATA_OWN_STRUCTURE  0x1

/*
 * Data content may be free'd
 */
#define IODEFV2_DATA_OWN_DATA       0x2


#define IODEFV2_DATA_DECL(iodefv2_data_type, c_type, name, fname)         \
int iodefv2_data_new_ ## name(iodefv2_data_t **nd, c_type val)            \
{                                                                     \
        int ret;                                                      \
                                                                      \
        ret = iodefv2_data_new(nd);                                     \
        if ( ret < 0 )                                                \
                return ret;                                           \
                                                                      \
        iodefv2_data_set_ ## fname(*nd, val);                           \
                                                                      \
        return ret;                                                   \
}                                                                     \
                                                                      \
void iodefv2_data_set_ ## name(iodefv2_data_t *ptr, c_type val)           \
{                                                                     \
        libiodefv2_return_if_fail(ptr);                                  \
        iodefv2_data_destroy_internal(ptr);                             \
        ptr->type = iodefv2_data_type;                                  \
        ptr->len = sizeof(val);                                       \
        ptr->data.fname ## _data = val;                               \
}                                                                     \
                                                                      \
c_type iodefv2_data_get_ ## name(const iodefv2_data_t *ptr)               \
{                                                                     \
        return ptr->data.fname ## _data;                              \
}


IODEFV2_DATA_DECL(IODEFV2_DATA_TYPE_CHAR, char, char, char)
IODEFV2_DATA_DECL(IODEFV2_DATA_TYPE_BYTE, uint8_t, byte, byte)
IODEFV2_DATA_DECL(IODEFV2_DATA_TYPE_INT, uint32_t, uint32, int)
IODEFV2_DATA_DECL(IODEFV2_DATA_TYPE_INT, uint64_t, uint64, int)
IODEFV2_DATA_DECL(IODEFV2_DATA_TYPE_INT, int64_t, int, int)
IODEFV2_DATA_DECL(IODEFV2_DATA_TYPE_FLOAT, float, float, float)


int iodefv2_data_new(iodefv2_data_t **data)
{
        *data = calloc(1, sizeof(**data));
        if ( ! *data )
                return libiodefv2_error_from_errno(errno);

        (*data)->refcount = 1;
        (*data)->flags |= IODEFV2_DATA_OWN_STRUCTURE;

        return 0;
}


int iodefv2_data_new_time(iodefv2_data_t **data, iodefv2_time_t *time)
{
        int ret;

        ret = iodefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        (*data)->len = 1;
        (*data)->type = IODEFV2_DATA_TYPE_TIME;
        (*data)->flags |= IODEFV2_DATA_OWN_DATA;
        (*data)->data.ro_data = time;

        return 0;
}


void iodefv2_data_set_time(iodefv2_data_t *data, iodefv2_time_t *time)
{
        data->len = 1;
        data->type = IODEFV2_DATA_TYPE_TIME;
        data->flags |= IODEFV2_DATA_OWN_DATA;
        data->data.ro_data = time;
}

iodefv2_data_t *iodefv2_data_ref(iodefv2_data_t *data)
{
        libiodefv2_return_val_if_fail(data, NULL);

        data->refcount++;
        return data;
}



int iodefv2_data_set_ptr_ref_fast(iodefv2_data_t *data, iodefv2_data_type_t type, const void *ptr, size_t len)
{
        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        iodefv2_data_destroy_internal(data);

        data->type = type;
        data->data.ro_data = ptr;
        data->len = len;

        return 0;
}



int iodefv2_data_set_ptr_dup_fast(iodefv2_data_t *data, iodefv2_data_type_t type, const void *ptr, size_t len)
{
        void *new;

        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        iodefv2_data_destroy_internal(data);

        new = malloc(len);
        if ( ! new )
                return -1;

        memcpy(new, ptr, len);

        data->type = type;
        data->data.rw_data = new;
        data->len = len;
        data->flags |= IODEFV2_DATA_OWN_DATA;

        return 0;
}



int iodefv2_data_set_ptr_nodup_fast(iodefv2_data_t *data, iodefv2_data_type_t type, void *ptr, size_t len)
{
        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        iodefv2_data_destroy_internal(data);

        data->type = type;
        data->data.rw_data= ptr;
        data->len = len;
        data->flags |= IODEFV2_DATA_OWN_DATA;

        return 0;
}



int iodefv2_data_new_ptr_ref_fast(iodefv2_data_t **data, iodefv2_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = iodefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_data_set_ptr_ref_fast(*data, type, ptr, len);
        if ( ret < 0 )
                iodefv2_data_destroy(*data);

        return ret;
}



int iodefv2_data_new_ptr_dup_fast(iodefv2_data_t **data, iodefv2_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = iodefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_data_set_ptr_dup_fast(*data, type, ptr, len);
        if ( ret < 0 )
                iodefv2_data_destroy(*data);

        return ret;
}



int iodefv2_data_new_ptr_nodup_fast(iodefv2_data_t **data, iodefv2_data_type_t type, void *ptr, size_t len)
{
        int ret;

        ret = iodefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_data_set_ptr_nodup_fast(*data, type, ptr, len);
        if ( ret < 0 )
                iodefv2_data_destroy(*data);

        return ret;
}




/**
 * iodefv2_data_copy_ref:
 * @src: Source #iodefv2_data_t object.
 * @dst: Destination #iodefv2_data_t object.
 *
 * Makes @dst reference the same buffer as @src.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_data_copy_ref(const iodefv2_data_t *src, iodefv2_data_t *dst)
{
        libiodefv2_return_val_if_fail(src, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(dst, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        iodefv2_data_destroy_internal(dst);

        dst->type = src->type;
        dst->len = src->len;
        dst->data = src->data;
        dst->flags &= ~IODEFV2_DATA_OWN_DATA;

        return 0;
}




/**
 * iodefv2_data_copy_dup:
 * @src: Source #iodefv2_data_t object.
 * @dst: Destination #iodefv2_data_t object.
 *
 * Copies @src to @dst, including the associated buffer.
 * This is an alternative to iodefv2_data_clone().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_data_copy_dup(const iodefv2_data_t *src, iodefv2_data_t *dst)
{
        int ret;

        libiodefv2_return_val_if_fail(src, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(dst, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        iodefv2_data_destroy_internal(dst);

        dst->type = src->type;
        dst->flags |= IODEFV2_DATA_OWN_DATA;
        dst->len = src->len;

        if ( src->type == IODEFV2_DATA_TYPE_TIME ) {
                ret = iodefv2_time_clone((iodefv2_time_t *) src->data.rw_data, (iodefv2_time_t **) &dst->data.rw_data);
                if ( ret < 0 )
                        return ret;
        }

        else if ( src->type == IODEFV2_DATA_TYPE_CHAR_STRING || src->type == IODEFV2_DATA_TYPE_BYTE_STRING ) {
                dst->data.rw_data = malloc(src->len);
                if ( ! dst->data.rw_data )
                        return -1;

                memcpy(dst->data.rw_data, src->data.ro_data, src->len);
        } else {
                dst->data = src->data;
        }

        return 0;
}



int iodefv2_data_clone(const iodefv2_data_t *src, iodefv2_data_t **dst)
{
        int ret;

        libiodefv2_return_val_if_fail(src, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_data_new(dst);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_data_copy_dup(src, *dst);
        if ( ret < 0 )
                iodefv2_data_destroy(*dst);

        return ret;
}



const char *iodefv2_data_get_char_string(const iodefv2_data_t *data)
{
        libiodefv2_return_val_if_fail(data, NULL);
        return data->data.ro_data;
}



const unsigned char *iodefv2_data_get_byte_string(const iodefv2_data_t *data)
{
        libiodefv2_return_val_if_fail(data, NULL);
        return data->data.ro_data;
}



/**
 * iodefv2_data_get_type
 * @data: Pointer to an #iodefv2_data_t object.
 *
 * Returns: the type of the embedded data.
 */
iodefv2_data_type_t iodefv2_data_get_type(const iodefv2_data_t *data)
{
        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return data->type;
}




/**
 * iodefv2_data_get_len:
 * @data: Pointer to an #iodefv2_data_t object.
 *
 * Returns: the length of data contained within @data object.
 */
size_t iodefv2_data_get_len(const iodefv2_data_t *data)
{
        libiodefv2_return_val_if_fail(data, 0);
        return data->len;
}




/**
 * iodefv2_data_get_data:
 * @data: Pointer to an #iodefv2_data_t object.
 *
 * Returns: the data contained within @data object.
 */
const void *iodefv2_data_get_data(const iodefv2_data_t *data)
{
        libiodefv2_return_val_if_fail(data, NULL);

        switch ( data->type ) {
        case IODEFV2_DATA_TYPE_UNKNOWN:
                return NULL;

        case IODEFV2_DATA_TYPE_CHAR_STRING:
        case IODEFV2_DATA_TYPE_BYTE_STRING:
        case IODEFV2_DATA_TYPE_TIME:
                return data->data.ro_data;

        default:
                return &data->data;
        }

        return NULL;
}



/**
 * iodefv2_data_is_empty:
 * @data: Pointer to an #iodefv2_data_t object.
 *
 * Returns: TRUE if empty, FALSE otherwise.
 */
libiodefv2_bool_t iodefv2_data_is_empty(const iodefv2_data_t *data)
{
        libiodefv2_return_val_if_fail(data, TRUE);
        return (data->len == 0) ? TRUE : FALSE;
}



static int bytes_to_string(libiodefv2_string_t *out, const unsigned char *src, size_t size)
{
        char c;
        int ret;
        static const char b64tbl[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        while ( size ) {
                ret = libiodefv2_string_ncat(out, &b64tbl[(src[0] >> 2) & 0x3f], 1);
                if ( ret < 0 )
                        return ret;

                c = b64tbl[((src[0] << 4) + ((--size) ? src[1] >> 4 : 0)) & 0x3f];

                ret = libiodefv2_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                c = (size) ? b64tbl[((src[1] << 2) + ((--size) ? src[2] >> 6 : 0)) & 0x3f] : '=';

                ret = libiodefv2_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                c = (size && size--) ? b64tbl[src[2] & 0x3f] : '=';

                ret = libiodefv2_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                src += 3;
        }

        return 0;
}




/**
 * iodefv2_data_to_string:
 * @data: Pointer to an #iodefv2_data_t object.
 * @out: Pointer to a #libiodefv2_string_t to store the formated data into.
 *
 * Formats data contained within @data to be printable,
 * and stores the result in the provided @out buffer.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_data_to_string(const iodefv2_data_t *data, libiodefv2_string_t *out)
{
        int ret = 0;

        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(out, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        switch ( data->type ) {
        case IODEFV2_DATA_TYPE_UNKNOWN:
                return 0;

        case IODEFV2_DATA_TYPE_CHAR:
                ret = libiodefv2_string_sprintf(out, "%c", data->data.char_data);
                break;

        case IODEFV2_DATA_TYPE_BYTE:
                /*
                 * %hh convertion specifier is not portable.
                 */
                ret = libiodefv2_string_sprintf(out, "%u", (unsigned int) data->data.byte_data);
                break;

        case IODEFV2_DATA_TYPE_UINT32:
        case IODEFV2_DATA_TYPE_INT:
                ret = libiodefv2_string_sprintf(out, "%" LIBIODEFV2_PRId64, data->data.int_data);
                break;

        case IODEFV2_DATA_TYPE_FLOAT:
                ret = libiodefv2_string_sprintf(out, "%f", data->data.float_data);
                break;

        case IODEFV2_DATA_TYPE_CHAR_STRING:
                ret = libiodefv2_string_sprintf(out, "%s", (const char *) data->data.ro_data);
                break;

        case IODEFV2_DATA_TYPE_BYTE_STRING:
                ret = bytes_to_string(out, data->data.ro_data, data->len);
                break;

        case IODEFV2_DATA_TYPE_TIME:
                ret = iodefv2_time_to_string(data->data.ro_data, out);
                break;
        }

        return ret;
}



/*
 *  This function cannot be declared static because it is invoked
 *  from iodefv2-tree-wrap.c
 */
void iodefv2_data_destroy_internal(iodefv2_data_t *ptr)
{
        libiodefv2_return_if_fail(ptr);

        if ( ptr->type == IODEFV2_DATA_TYPE_TIME && ptr->flags & IODEFV2_DATA_OWN_DATA )
                iodefv2_time_destroy(ptr->data.rw_data);

        else if ( (ptr->type == IODEFV2_DATA_TYPE_CHAR_STRING || ptr->type == IODEFV2_DATA_TYPE_BYTE_STRING) &&
             ptr->flags & IODEFV2_DATA_OWN_DATA ) {
                free(ptr->data.rw_data);
                ptr->data.rw_data = NULL;
        }

        /*
         * free() should be done by the caller
         */
}




/**
 * iodefv2_data_destroy:
 * @data: Pointer to an #iodefv2_data_t object.
 *
 * Frees @data. The buffer pointed by @data will be freed if
 * the @data object is marked as _dup or _nodup.
 */
void iodefv2_data_destroy(iodefv2_data_t *data)
{
        libiodefv2_return_if_fail(data);

        if ( --data->refcount )
                return;

        iodefv2_data_destroy_internal(data);

        if ( data->flags & IODEFV2_DATA_OWN_STRUCTURE )
                free(data);
}


int iodefv2_data_new_char_string_ref_fast(iodefv2_data_t **data, const char *ptr, size_t len)
{
        return iodefv2_data_new_ptr_ref_fast(data, IODEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodefv2_data_new_char_string_dup_fast(iodefv2_data_t **data, const char *ptr, size_t len)
{
        return iodefv2_data_new_ptr_dup_fast(data, IODEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodefv2_data_new_char_string_nodup_fast(iodefv2_data_t **data, char *ptr, size_t len)
{
        return iodefv2_data_new_ptr_nodup_fast(data, IODEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodefv2_data_set_char_string_ref_fast(iodefv2_data_t *data, const char *ptr, size_t len)
{
        return iodefv2_data_set_ptr_ref_fast(data, IODEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodefv2_data_set_char_string_dup_fast(iodefv2_data_t *data, const char *ptr, size_t len)
{
        return iodefv2_data_set_ptr_dup_fast(data, IODEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodefv2_data_set_char_string_nodup_fast(iodefv2_data_t *data, char *ptr, size_t len)
{
        return iodefv2_data_set_ptr_nodup_fast(data, IODEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}


int iodefv2_data_new_char_string_ref(iodefv2_data_t **data, const char *ptr)
{
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return iodefv2_data_new_char_string_ref_fast(data, ptr, strlen(ptr));
}

int iodefv2_data_new_char_string_dup(iodefv2_data_t **data, const char *ptr)
{
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return iodefv2_data_new_char_string_dup_fast(data, ptr, strlen(ptr));
}

int iodefv2_data_new_char_string_nodup(iodefv2_data_t **data, char *ptr)
{
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return iodefv2_data_new_char_string_nodup_fast(data, ptr, strlen(ptr));
}

int iodefv2_data_set_char_string_ref(iodefv2_data_t *data, const char *ptr)
{
        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return iodefv2_data_set_char_string_ref_fast(data, ptr, strlen(ptr));
}

int iodefv2_data_set_char_string_dup(iodefv2_data_t *data, const char *ptr)
{
        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return iodefv2_data_set_char_string_dup_fast(data, ptr, strlen(ptr));
}

int iodefv2_data_set_char_string_nodup(iodefv2_data_t *data, char *ptr)
{
        libiodefv2_return_val_if_fail(data, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return iodefv2_data_set_char_string_nodup_fast(data, ptr, strlen(ptr));
}


/*
 *
 */
int iodefv2_data_new_byte_string_ref(iodefv2_data_t **data, const unsigned char *ptr, size_t len)
{
        return iodefv2_data_new_ptr_ref_fast(data, IODEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodefv2_data_new_byte_string_dup(iodefv2_data_t **data, const unsigned char *ptr, size_t len)
{
        return iodefv2_data_new_ptr_dup_fast(data, IODEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodefv2_data_new_byte_string_nodup(iodefv2_data_t **data, unsigned char *ptr, size_t len)
{
        return iodefv2_data_new_ptr_nodup_fast(data, IODEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodefv2_data_set_byte_string_ref(iodefv2_data_t *data, const unsigned char *ptr, size_t len)
{
        return iodefv2_data_set_ptr_ref_fast(data, IODEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodefv2_data_set_byte_string_dup(iodefv2_data_t *data, const unsigned char *ptr, size_t len)
{
        return iodefv2_data_set_ptr_dup_fast(data, IODEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodefv2_data_set_byte_string_nodup(iodefv2_data_t *data, unsigned char *ptr, size_t len)
{
        return iodefv2_data_set_ptr_nodup_fast(data, IODEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}



int iodefv2_data_compare(const iodefv2_data_t *data1, const iodefv2_data_t *data2)
{
        if ( ! data1 && ! data2 )
                return 0;

        else if ( ! data1 || ! data2 )
                return (data1) ? 1 : -1;

        else if ( data1->len != data2->len )
                return (data1->len > data2->len) ? 1 : -1;

        else if ( data1->type != data2->type )
                return -1;

        if ( data1->type == IODEFV2_DATA_TYPE_TIME )
                return iodefv2_time_compare(data1->data.ro_data, data2->data.ro_data);

        else if ( data1->type == IODEFV2_DATA_TYPE_CHAR_STRING || data1->type == IODEFV2_DATA_TYPE_BYTE_STRING )
                return memcmp(data1->data.ro_data, data2->data.ro_data, data1->len);

        else
                return memcmp(&data1->data.char_data, &data2->data.char_data, data1->len);
}
