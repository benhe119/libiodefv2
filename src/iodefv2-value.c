/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@libiodefv2-ids.com>
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

#include "config.h"

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "libiodefv2-list.h"
#include "libiodefv2-inttypes.h"
#include "libiodefv2-string.h"
#include "libiodefv2-error.h"
#include "common.h"

#include "iodefv2.h"
#include "iodefv2-value-type.h"

#define CHUNK_SIZE 16
#define FLOAT_TOLERANCE 0.0001


#ifndef MAX
# define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef ABS
# define ABS(x) (((x) < 0) ? -(x) : (x))
#endif


#define iodefv2_value_new_decl(mtype, vname, vtype)                    \
int iodefv2_value_new_ ## vname (iodefv2_value_t **value, vtype val) {   \
        int ret;                                                     \
                                                                     \
        ret = iodefv2_value_create(value, IODEFV2_VALUE_TYPE_ ## mtype); \
        if ( ret < 0 )                                               \
                return ret;                                          \
                                                                     \
        (*value)->type.data. vname ## _val = val;                    \
                                                                     \
        return 0;                                                    \
}


#define iodefv2_value_set_decl(mtype, vname, vtype)                    \
int iodefv2_value_set_ ## vname (iodefv2_value_t *value, vtype val)      \
{                                                                    \
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION)); \
                                                                     \
        if ( value->own_data )                                       \
                iodefv2_value_type_destroy(&value->type);              \
                                                                     \
        value->type.id = IODEFV2_VALUE_TYPE_ ## mtype;                 \
        value->type.data. vname ## _val = val;                       \
        value->own_data = TRUE;                                      \
                                                                     \
        return 0;                                                    \
}

#define iodefv2_value_get_decl(mtype, vname, vtype)                \
vtype iodefv2_value_get_ ## vname (const iodefv2_value_t *val)       \
{                                                                \
        libiodefv2_return_val_if_fail(val, (vtype) 0);              \
                                                                 \
        if ( val->type.id != IODEFV2_VALUE_TYPE_ ## mtype )        \
                return (vtype) 0;                                \
                                                                 \
        return val->type.data. vname ## _val;                    \
}


#define iodefv2_value_decl(mtype, vname, vtype)     \
        iodefv2_value_new_decl(mtype, vname, vtype) \
        iodefv2_value_get_decl(mtype, vname, vtype) \
        iodefv2_value_set_decl(mtype, vname, vtype)



#define CASTCHK(ntype, fval, src, dst)                                             \
  if ( ! (src == dst && (src < 1) == (dst < 1)) )                                  \
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_GENERIC,                \
                        "Value '%" fval "' is incompatible with output type '%s'", \
                        src, iodefv2_value_type_to_string(ntype))



#define FLOATCHK(ntype, fval, src, dst)                                            \
        if ( reldif(src, dst) > FLOAT_TOLERANCE )                                  \
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_GENERIC,                \
                        "Value '%" fval "' is incompatible with output type '%s'", \
                        src, iodefv2_value_type_to_string(ntype))


#define VALUE_CAST_CHECK(v, itype_t, itype, iformat, ntype) do {      \
        itype_t src = iodefv2_value_get_ ##itype(value);                \
                                                                      \
        if ( ntype == IODEFV2_VALUE_TYPE_INT8 ) {                       \
                CASTCHK(ntype, iformat, src, (int8_t) src);           \
                iodefv2_value_set_int8(value, src);                     \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_UINT8 ) {               \
                CASTCHK(ntype, iformat, src, (uint8_t) src);          \
                iodefv2_value_set_uint8(value, src);                    \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_INT16 ) {               \
                CASTCHK(ntype, iformat, src, (int16_t) src);          \
                iodefv2_value_set_int16(value, src);                    \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_UINT16 ) {              \
                CASTCHK(ntype, iformat, src, (uint16_t) src);         \
                iodefv2_value_set_uint16(value, src);                   \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_INT32 ) {               \
                CASTCHK(ntype, iformat, src, (int32_t) src);          \
                iodefv2_value_set_int32(value, src);                    \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_UINT32 ) {              \
                CASTCHK(ntype, iformat, src, (uint32_t) src);         \
                iodefv2_value_set_uint32(value, src);                   \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_INT64 ) {               \
                CASTCHK(ntype, iformat, src, (int64_t) src);          \
                iodefv2_value_set_int64(value, src);                    \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_UINT64 ) {              \
                CASTCHK(ntype, iformat, src, (uint64_t) src);         \
                iodefv2_value_set_uint64(value, src);                   \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_FLOAT ) {               \
                FLOATCHK(ntype, iformat, src, (float) src);           \
                iodefv2_value_set_float(value, src);                    \
                                                                      \
        } else if ( ntype == IODEFV2_VALUE_TYPE_DOUBLE ) {              \
                FLOATCHK(ntype, iformat, src, (double) src);          \
                iodefv2_value_set_double(value, src);                   \
                                                                      \
        } else return libiodefv2_error_verbose(LIBIODEFV2_ERROR_GENERIC,    \
                                          "Unable to handle output type '%s' for integer cast", \
                                           iodefv2_value_type_to_string(ntype));                  \
} while(0)



typedef struct compare {
        unsigned int match;
        iodefv2_value_t *val2;
        iodefv2_criterion_operator_t operator;
} compare_t;



struct iodefv2_value {
        int list_elems;
        int list_max;
        int refcount;
        libiodefv2_bool_t own_data;
        iodefv2_value_t **list;
        iodefv2_value_type_t type;
};


/*
 * Returns the relative difference of two real numbers: 0.0 if they are
 * exactly the same, otherwise, the ratio of the difference to the
 * larger of the two.
 */
static double reldif(double a, double b)
{
        double c = ABS(a);
        double d = ABS(b);

        d = MAX(c, d);

        return d == 0.0 ? 0.0 : ABS(a - b) / d;
}


static int string_isdigit(const char *s)
{
        while ( *s ) {
                if ( ! isdigit((int) *s) )
                        return -1;
                s++;
        }

        return 0;
}


static int iodefv2_value_create(iodefv2_value_t **ret, iodefv2_value_type_id_t type_id)
{
        *ret = calloc(1, sizeof(**ret));
        if ( ! *ret )
                return libiodefv2_error_from_errno(errno);

        (*ret)->refcount = 1;
        (*ret)->own_data = TRUE;
        (*ret)->type.id = type_id;

        return 0;
}



iodefv2_value_decl(INT8, int8, int8_t)
iodefv2_value_decl(UINT8, uint8, uint8_t)
iodefv2_value_decl(INT16, int16, int16_t)
iodefv2_value_decl(UINT16, uint16, uint16_t)
iodefv2_value_decl(INT32, int32, int32_t)
iodefv2_value_decl(UINT32, uint32, uint32_t)
iodefv2_value_decl(INT64, int64, int64_t)
iodefv2_value_decl(UINT64, uint64, uint64_t)
iodefv2_value_decl(FLOAT, float, float)
iodefv2_value_decl(DOUBLE, double, double)
iodefv2_value_decl(STRING, string, libiodefv2_string_t *)
iodefv2_value_decl(DATA, data, iodefv2_data_t *)
iodefv2_value_decl(TIME, time, iodefv2_time_t *)



int iodefv2_value_get_enum(const iodefv2_value_t *value)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( value->type.id != IODEFV2_VALUE_TYPE_ENUM )
                return 0;

        return value->type.data.enum_val.value;
}


int iodefv2_value_set_class(iodefv2_value_t *value, iodefv2_class_id_t class, void *object)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(object, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( value->own_data )
                iodefv2_value_type_destroy(&value->type);

        value->own_data = TRUE;
        value->type.data.class_val.object = object;
        value->type.data.class_val.class_id = class;

        return 0;
}


int iodefv2_value_new_class(iodefv2_value_t **value, iodefv2_class_id_t class, void *object)
{
        int ret;

        libiodefv2_return_val_if_fail(object, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_value_create(value, IODEFV2_VALUE_TYPE_CLASS);
        if ( ret < 0 )
                return ret;

        (*value)->type.data.class_val.object = object;
        (*value)->type.data.class_val.class_id = class;

        return ret;
}


int iodefv2_value_set_enum_from_numeric(iodefv2_value_t *value, iodefv2_class_id_t class, int val)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( value->own_data )
                iodefv2_value_type_destroy(&value->type);

        value->own_data = TRUE;
        value->type.id = IODEFV2_VALUE_TYPE_ENUM;
        value->type.data.enum_val.value = val;
        value->type.data.enum_val.class_id = class;

        return 0;
}


int iodefv2_value_set_enum_from_string(iodefv2_value_t *value, iodefv2_class_id_t class, const char *buf)
{
        int ret;

        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_class_enum_to_numeric(class, buf);
        if ( ret < 0 )
                return ret;

        return iodefv2_value_set_enum_from_numeric(value, class, ret);
}


int iodefv2_value_set_enum(iodefv2_value_t *value, iodefv2_class_id_t class, const char *buf)
{
        int ret;

        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( string_isdigit(buf) == 0 )
                ret = iodefv2_value_set_enum_from_numeric(value, class, atoi(buf));
        else
                ret = iodefv2_value_set_enum_from_string(value, class, buf);

        return ret;
}


int iodefv2_value_new_enum_from_numeric(iodefv2_value_t **value, iodefv2_class_id_t class, int val)
{
        int ret;

        ret = iodefv2_value_create(value, IODEFV2_VALUE_TYPE_ENUM);
        if ( ret < 0 )
                return ret;

        (*value)->type.data.enum_val.value = val;
        (*value)->type.data.enum_val.class_id = class;

        return ret;
}


int iodefv2_value_new_enum_from_string(iodefv2_value_t **value, iodefv2_class_id_t class, const char *buf)
{
        int ret;

        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_class_enum_to_numeric(class, buf);
        if ( ret < 0 )
                return ret;

        return iodefv2_value_new_enum_from_numeric(value, class, ret);
}



int iodefv2_value_new_enum(iodefv2_value_t **value, iodefv2_class_id_t class, const char *buf)
{
        int ret;

        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( string_isdigit(buf) == 0 )
                ret = iodefv2_value_new_enum_from_numeric(value, class, atoi(buf));
        else
                ret = iodefv2_value_new_enum_from_string(value, class, buf);

        return ret;
}


int iodefv2_value_new_list(iodefv2_value_t **value)
{
        int ret;

        ret = iodefv2_value_create(value, IODEFV2_VALUE_TYPE_LIST);
        if ( ret < 0 )
                return ret;

        (*value)->list = malloc(CHUNK_SIZE * sizeof(iodefv2_value_t *));
        if ( ! (*value)->list ) {
                free(*value);
                    return libiodefv2_error_from_errno(errno);
        }

        (*value)->list_elems = 0;
        (*value)->list_max = CHUNK_SIZE - 1;

        return 0;
}



int iodefv2_value_list_add(iodefv2_value_t *list, iodefv2_value_t *item)
{
        libiodefv2_return_val_if_fail(list, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( list->list_elems == list->list_max ) {

                list->list = realloc(list->list, (list->list_max + 1 + CHUNK_SIZE) * sizeof(iodefv2_value_t *));
                if ( ! list->list )
                        return libiodefv2_error_from_errno(errno);

                list->list_max += CHUNK_SIZE;
        }

        list->list[list->list_elems++] = item;

        return 0;
}



libiodefv2_bool_t iodefv2_value_is_list(const iodefv2_value_t *list)
{
        libiodefv2_return_val_if_fail(list, FALSE);
        return (list->list) ? TRUE : FALSE;
}



libiodefv2_bool_t iodefv2_value_list_is_empty(const iodefv2_value_t *list)
{
        libiodefv2_return_val_if_fail(list, TRUE);
        return (list->list_elems) ? FALSE : TRUE;
}




int iodefv2_value_new(iodefv2_value_t **value, iodefv2_value_type_id_t type, void *ptr)
{
        int ret;

        libiodefv2_return_val_if_fail(ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_value_create(value, type);
        if ( ret < 0 )
                return ret;

        (*value)->type.data.data_val = ptr;

        return 0;
}



int iodefv2_value_new_from_string(iodefv2_value_t **value, iodefv2_value_type_id_t type, const char *buf)
{
        int ret;

        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_value_create(value, type);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_value_type_read(&(*value)->type, buf);
        if ( ret < 0 ) {
                free(*value);
                return ret;
        }

        return 0;
}



int iodefv2_value_new_from_path(iodefv2_value_t **value, iodefv2_path_t *path, const char *buf)
{
        int ret;
        iodefv2_class_id_t class;
        iodefv2_value_type_id_t value_type;

        libiodefv2_return_val_if_fail(path, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        value_type = iodefv2_path_get_value_type(path, -1);
        if ( value_type < 0 )
                return value_type;

        if ( value_type != IODEFV2_VALUE_TYPE_ENUM )
                ret = iodefv2_value_new_from_string(value, value_type, buf);
        else {
                class = iodefv2_path_get_class(path, -1);
                if ( class < 0 )
                        return class;

                ret = iodefv2_value_new_enum(value, class, buf);
        }

        return ret;
}



static int iodefv2_value_set_own_data(iodefv2_value_t *value, libiodefv2_bool_t own_data)
{
        int cnt;

        if ( ! value->list )
                value->own_data = own_data;

        else for ( cnt = 0 ; cnt < value->list_elems; cnt++ ) {
                if ( value->list[cnt] )
                        iodefv2_value_set_own_data(value->list[cnt], own_data);
        }

        return 0;
}




int iodefv2_value_have_own_data(iodefv2_value_t *value)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return iodefv2_value_set_own_data(value, TRUE);
}



int iodefv2_value_dont_have_own_data(iodefv2_value_t *value)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return iodefv2_value_set_own_data(value, FALSE);
}



iodefv2_value_type_id_t iodefv2_value_get_type(const iodefv2_value_t *value)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return value->type.id;
}



iodefv2_class_id_t iodefv2_value_get_class(const iodefv2_value_t *value)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( value->type.id == IODEFV2_VALUE_TYPE_CLASS )
                return value->type.data.class_val.class_id;

        else if ( value->type.id == IODEFV2_VALUE_TYPE_ENUM )
                return value->type.data.enum_val.class_id;

        return -1;
}



void *iodefv2_value_get_object(const iodefv2_value_t *value)
{
        libiodefv2_return_val_if_fail(value, NULL);
        return (value->type.id == IODEFV2_VALUE_TYPE_CLASS) ? value->type.data.class_val.object : NULL;
}


inline static iodefv2_value_t *value_ro2rw(const iodefv2_value_t *value)
{
        union {
                iodefv2_value_t *rw;
                const iodefv2_value_t *ro;
        } val;

        val.ro = value;

        return val.rw;
}


int iodefv2_value_iterate(const iodefv2_value_t *value,
                        int (*callback)(iodefv2_value_t *ptr, void *extra), void *extra)
{
        int i, ret;

        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(callback, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( ! value->list )
                return callback(value_ro2rw(value), extra);

        for ( i = 0; i < value->list_elems; i++ ) {

                ret = callback(value->list[i], extra);
                if ( ret < 0 )
                        return ret;
        }

        return 0;
}



int iodefv2_value_iterate_reversed(const iodefv2_value_t *value,
                                 int (*callback)(iodefv2_value_t *ptr, void *extra), void *extra)
{
        int i, ret;

        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(callback, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( ! value->list )
                return callback(value_ro2rw(value), extra);

        for ( i = value->list_elems - 1; i >= 0; i-- ) {

                ret = callback(value->list[i], extra);
                if ( ret < 0 )
                        return ret;
        }

        return 0;
}



int iodefv2_value_get_nth2(const iodefv2_value_t *val, int index, iodefv2_value_t **ret)
{
        libiodefv2_return_val_if_fail(val, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(index <= val->list_elems, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( index == 0 && ! val->list ) {
                *ret = value_ro2rw(val);
                return 1;
        }

        else if ( index >= 0 && index < val->list_elems ) {
                *ret = val->list[index];
                return 1;
        }

        else if ( index >= val->list_elems )
                return 0;

        else return -1;
}



iodefv2_value_t *iodefv2_value_get_nth(const iodefv2_value_t *val, int n)
{
        int ret;
        iodefv2_value_t *rv;

        libiodefv2_return_val_if_fail(val, NULL);

        ret = iodefv2_value_get_nth2(val, n, &rv);
        if ( ret != 1 )
                return NULL;

        return rv;
}



int iodefv2_value_get_count(const iodefv2_value_t *val)
{
        libiodefv2_return_val_if_fail(val, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return val->list ? val->list_elems : 1;
}




static int iodefv2_value_list_clone(const iodefv2_value_t *val, iodefv2_value_t **dst)
{
        int cnt, ret;

        ret = iodefv2_value_create(dst, val->type.id);
        if ( ret < 0 )
                return ret;

        (*dst)->list_elems = val->list_elems;
        (*dst)->list_max = val->list_max;
        (*dst)->list = malloc(((*dst)->list_elems + 1) * sizeof((*dst)->list));

        for ( cnt = 0; cnt < (*dst)->list_elems; cnt++ ) {
                if ( ! val->list[cnt] ) {
                        (*dst)->list[cnt] = NULL;
                        continue;
                }

                ret = iodefv2_value_clone(val->list[cnt], &((*dst)->list[cnt]));
                if ( ret < 0 ) {
                        while ( --cnt >= 0 ) {
                                if ( (*dst)->list[cnt] )
                                        iodefv2_value_destroy((*dst)->list[cnt]);
                        }
                }

                free((*dst)->list);
                free(*dst);

                return -1;
        }

        return 0;
}



int iodefv2_value_clone(const iodefv2_value_t *val, iodefv2_value_t **dst)
{
        int ret;

        libiodefv2_return_val_if_fail(val, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( val->list )
                return iodefv2_value_list_clone(val, dst);

        ret = iodefv2_value_create(dst, val->type.id);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_value_type_clone(&val->type, &(*dst)->type);
        if ( ret < 0 )
                free(*dst);

        return ret;
}



iodefv2_value_t *iodefv2_value_ref(iodefv2_value_t *val)
{
        libiodefv2_return_val_if_fail(val, NULL);

        val->refcount++;

        return val;
}



int iodefv2_value_to_string(const iodefv2_value_t *value, libiodefv2_string_t *out)
{
        int i, ret;
        iodefv2_value_t *val;

        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(out, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( iodefv2_value_is_list(value) ) {
                ret = libiodefv2_string_cat(out, "(");
                if ( ret < 0 )
                        return ret;
        }

        for ( i = 0; i < iodefv2_value_get_count(value); i++ ) {
                val = iodefv2_value_get_nth(value, i);

                if ( iodefv2_value_is_list(val) ) {
                        ret = libiodefv2_string_cat(out, "(");
                        if ( ret < 0 )
                                return ret;

                        ret = iodefv2_value_to_string(val, out);
                        if ( ret < 0 )
                                return ret;

                        ret = libiodefv2_string_cat(out, ")");
                        if ( ret < 0 )
                                return ret;
                } else {
                        if ( i ) {
                                ret = libiodefv2_string_cat(out, ", ");
                                if ( ret < 0 )
                                        return ret;
                        }

                        ret = iodefv2_value_type_write(&val->type, out);
                        if ( ret < 0 )
                                return ret;
                }
        }

        if ( iodefv2_value_is_list(value) ) {
                ret = libiodefv2_string_cat(out, ")");
                if ( ret < 0 )
                        return ret;
        }

        return 0;
}



int iodefv2_value_print(const iodefv2_value_t *val, libiodefv2_io_t *fd)
{
        int ret;
        libiodefv2_string_t *out;

        libiodefv2_return_val_if_fail(val, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(fd, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = libiodefv2_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_value_type_write(&val->type, out);
        if ( ret < 0 ) {
                libiodefv2_string_destroy(out);
                return ret;
        }

        return libiodefv2_io_write(fd, libiodefv2_string_get_string(out), libiodefv2_string_get_len(out));
}



int iodefv2_value_get(const iodefv2_value_t *val, void *res)
{
        libiodefv2_return_val_if_fail(val, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(res, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return iodefv2_value_type_copy(&val->type, res);
}



static int iodefv2_value_match_internal(iodefv2_value_t *val1, void *extra)
{
        int ret;
        compare_t *compare = extra;

        if ( val1 && iodefv2_value_is_list(val1) )
                ret = iodefv2_value_iterate(val1, iodefv2_value_match_internal, extra);

        else if ( compare->val2 && iodefv2_value_is_list(compare->val2) ) {
                ret = iodefv2_value_match(compare->val2, val1, compare->operator);
                if ( ret < 0 )
                        return ret;

                compare->match += ret;
        }

        else {
                ret = iodefv2_value_type_compare((val1) ? &val1->type : NULL, &compare->val2->type, compare->operator);
                if ( ret == 0 )
                        compare->match++;
        }

        return ret;
}



/**
 * iodefv2_value_match:
 * @val1: Pointer to a #iodefv2_value_t object.
 * @val2: Pointer to a #iodefv2_value_t object.
 * @op: operator to use for matching.
 *
 * Match @val1 and @val2 using @op.
 *
 * Returns: the number of match, 0 for none, a negative value if an error occured.
 */
int iodefv2_value_match(iodefv2_value_t *val1, iodefv2_value_t *val2, iodefv2_criterion_operator_t op)
{
        int ret;
        compare_t compare;

        libiodefv2_return_val_if_fail(val1 || val2, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        compare.match = 0;
        compare.val2 = val2;
        compare.operator = op;

        if ( val1 )
                ret = iodefv2_value_iterate(val1, iodefv2_value_match_internal, &compare);
        else
                ret = iodefv2_value_match_internal(val1, &compare);

        if ( ret < 0 )
                return ret;

        return compare.match;
}



/**
 * iodefv2_value_check_operator:
 * @value: Pointer to a #iodefv2_value_t object.
 * @op: Type of operator to check @value for.
 *
 * Check whether @op can apply to @value.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_value_check_operator(const iodefv2_value_t *value, iodefv2_criterion_operator_t op)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return iodefv2_value_type_check_operator(value->type.id, op);
}



/**
 * iodefv2_value_get_applicable_operators:
 * @value: Pointer to a #iodefv2_value_t object.
 * @result: Pointer where the result will be stored.
 *
 * Store all operator supported by @value in @result.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */

int iodefv2_value_get_applicable_operators(const iodefv2_value_t *value, iodefv2_criterion_operator_t *result)
{
        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return iodefv2_value_type_get_applicable_operators(value->type.id, result);
}



/**
 * iodefv2_value_destroy:
 * @val: Pointer to a #iodefv2_value_t object.
 *
 * Decrement refcount and destroy @value if it reach 0.
 */
void iodefv2_value_destroy(iodefv2_value_t *val)
{
        int i;

        libiodefv2_return_if_fail(val);

        if ( --val->refcount )
                return;

        if ( val->list ) {
                for ( i = 0; i < val->list_elems; i++ ) {
                        if ( val->list[i] )
                                iodefv2_value_destroy(val->list[i]);
                }

                free(val->list);
        }

        /*
         * Actual destructor starts here
         */
        if ( val->own_data )
                iodefv2_value_type_destroy(&val->type);

        free(val);
}


static int cast_to_data(iodefv2_value_t *input)
{
        int ret;
        iodefv2_data_t *data;
        iodefv2_value_type_id_t vtype = iodefv2_value_get_type(input);

        if ( vtype == IODEFV2_VALUE_TYPE_STRING ) {
                libiodefv2_string_t *str = iodefv2_value_get_string(input);

                ret = iodefv2_data_new_char_string_dup_fast(&data, libiodefv2_string_get_string(str), libiodefv2_string_get_len(str));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_INT8 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_int8(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_UINT8 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_uint8(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_INT16 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_int16(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_UINT16 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_uint16(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_INT32 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_int32(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_UINT32 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_uint32(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_INT64 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_int64(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_UINT64 ) {
                ret = iodefv2_data_new_int(&data, iodefv2_value_get_uint64(input));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_TIME ) {
                ret = iodefv2_data_new_time(&data, iodefv2_time_ref(iodefv2_value_get_time(input)));
                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_FLOAT ) {
                int64_t v = iodefv2_value_get_float(input);

                if ( v == iodefv2_value_get_float(input) )
                        ret = iodefv2_data_new_int(&data, v);
                else
                        ret = iodefv2_data_new_float(&data, iodefv2_value_get_float(input));

                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        else if ( vtype == IODEFV2_VALUE_TYPE_DOUBLE ) {
                int64_t v = iodefv2_value_get_double(input);

                if ( v == iodefv2_value_get_double(input) )
                        ret = iodefv2_data_new_int(&data, v);
                else
                        ret = iodefv2_data_new_float(&data, iodefv2_value_get_double(input));

                if ( ret < 0 )
                        return ret;

                return iodefv2_value_set_data(input, data);
        }

        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_GENERIC,
                                     "Unable to cast input type '%s' to 'data'",
                                     iodefv2_value_type_to_string(vtype));
}


static int cast_to_time(iodefv2_value_t *value)
{
        int ret = -1;
        iodefv2_time_t *time;

        if ( iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_STRING ) {
                ret = iodefv2_time_new_from_string(&time, libiodefv2_string_get_string(iodefv2_value_get_string(value)));
                if ( ret < 0 )
                        return ret;

                iodefv2_value_set_time(value, time);
        }

        else if ( iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT32 ) {
                time_t val = iodefv2_value_get_int32(value);

                ret = iodefv2_time_new_from_time(&time, &val);
                if ( ret < 0 )
                        return ret;

                iodefv2_value_set_time(value, time);
        }

        else if ( iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_UINT32 ) {
                time_t val = iodefv2_value_get_uint32(value);

                ret = iodefv2_time_new_from_time(&time, &val);
                if ( ret < 0 )
                        return ret;

                iodefv2_value_set_time(value, time);
        }

        else if ( iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT64 ) {
                time_t val = iodefv2_value_get_int64(value);

                ret = iodefv2_time_new_from_time(&time, &val);
                if ( ret < 0 )
                        return ret;

                iodefv2_value_set_time(value, time);
        }

        else if ( iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_UINT64 ) {
                time_t val = iodefv2_value_get_uint64(value);

                ret = iodefv2_time_new_from_time(&time, &val);
                if ( ret < 0 )
                        return ret;

                iodefv2_value_set_time(value, time);
        }

        return ret;
}



static int cast_to_string(iodefv2_value_t *value)
{
        int ret;
        libiodefv2_string_t *out;

        ret = libiodefv2_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_value_type_write(&value->type, out);
        if ( ret < 0 ) {
                libiodefv2_string_destroy(out);
                return ret;
        }

        iodefv2_value_set_string(value, out);

        return 0;
}



static int cast_from_string(iodefv2_value_t *value, iodefv2_value_type_id_t ntype)
{
        int ret;
        iodefv2_value_type_t vt;
        libiodefv2_string_t *str = iodefv2_value_get_string(value);

        vt.id = ntype;

        ret = iodefv2_value_type_read(&vt, libiodefv2_string_get_string(str));
        if ( ret < 0 )
                return ret;

        if ( value->own_data )
                iodefv2_value_type_destroy(&value->type);

        memcpy(&value->type, &vt, sizeof(value->type));

        return 0;
}



int _iodefv2_value_cast(iodefv2_value_t *value, iodefv2_value_type_id_t ntype, iodefv2_class_id_t id)
{
        iodefv2_value_type_id_t otype;

        libiodefv2_return_val_if_fail(value, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        otype = iodefv2_value_get_type(value);
        libiodefv2_log_debug(3, "converting '%s' to '%s'.\n",
                          iodefv2_value_type_to_string(otype),
                          iodefv2_value_type_to_string(ntype));

        if ( ntype == IODEFV2_VALUE_TYPE_DATA )
                return cast_to_data(value);

        else if ( ntype == IODEFV2_VALUE_TYPE_TIME )
                return cast_to_time(value);

        else if ( ntype == IODEFV2_VALUE_TYPE_STRING )
                return cast_to_string(value);

        else if ( ntype == IODEFV2_VALUE_TYPE_ENUM && iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_STRING ) {
                libiodefv2_string_t *str = iodefv2_value_get_string(value);
                return iodefv2_value_set_enum_from_string(value, id, libiodefv2_string_get_string(str));
        }

        else if ( otype == IODEFV2_VALUE_TYPE_INT8 )
                VALUE_CAST_CHECK(value, int8_t, int8, LIBIODEFV2_PRId8, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_UINT8 )
                VALUE_CAST_CHECK(value, uint8_t, uint8, LIBIODEFV2_PRIu8, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_INT16 )
                VALUE_CAST_CHECK(value, int16_t, int16, LIBIODEFV2_PRId16, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_UINT16 )
                VALUE_CAST_CHECK(value, uint16_t, uint16, LIBIODEFV2_PRIu16, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_INT32 )
                VALUE_CAST_CHECK(value, int32_t, int32, LIBIODEFV2_PRId32, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_UINT32 )
                VALUE_CAST_CHECK(value, uint32_t, uint32, LIBIODEFV2_PRIu32, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_INT64 )
                VALUE_CAST_CHECK(value, int64_t, int64, LIBIODEFV2_PRId64, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_UINT64 )
                VALUE_CAST_CHECK(value, uint64_t, uint64, LIBIODEFV2_PRIu64, ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_FLOAT )
                VALUE_CAST_CHECK(value, float, float, "f", ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_DOUBLE )
                VALUE_CAST_CHECK(value, double, double, "f", ntype);

        else if ( otype == IODEFV2_VALUE_TYPE_STRING )
                return cast_from_string(value, ntype);

        else return libiodefv2_error_verbose(LIBIODEFV2_ERROR_GENERIC,
                                          "Unable to cast input type '%s' to '%s'",
                                           iodefv2_value_type_to_string(value->type.id),
                                           iodefv2_value_type_to_string(ntype));

        return 0;
}



int _iodefv2_value_copy_internal(const iodefv2_value_t *val,
                               iodefv2_value_type_id_t res_type, iodefv2_class_id_t res_id, void *res)
{
        int ret;

        libiodefv2_return_val_if_fail(val, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(res, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( res_type == val->type.id )
                ret = iodefv2_value_type_copy(&val->type, res);
        else {
                iodefv2_value_t copy;

                memcpy(&copy, val, sizeof(copy));
                iodefv2_value_dont_have_own_data(&copy);

                ret = _iodefv2_value_cast(&copy, res_type, res_id);
                if ( ret < 0 )
                        return ret;

                ret = iodefv2_value_type_copy(&copy.type, res);
                if ( val->own_data )
                        iodefv2_value_type_destroy(&copy.type);
        }

        return ret;
}
