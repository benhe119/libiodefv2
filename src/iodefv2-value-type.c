/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
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
#include <ctype.h>
#include <assert.h>

#include "common.h"
#include "libiodefv2-inttypes.h"
#include "libiodefv2-string.h"

#define LIBIODEFV2_ERROR_SOURCE_DEFAULT LIBIODEFV2_ERROR_SOURCE_IODEFV2_VALUE_TYPE
#include "libiodefv2-error.h"
#include "libiodefv2-inttypes.h"
#include "libiodefv2-linked-object.h"
#include "iodefv2-object-prv.h"

#include "iodefv2-time.h"
#include "iodefv2-data.h"
#include "iodefv2-value-type.h"


#define CLASS_OPERATOR  IODEFV2_CRITERION_OPERATOR_NULL|IODEFV2_CRITERION_OPERATOR_NOT| \
                        IODEFV2_CRITERION_OPERATOR_EQUAL

#define DATA_OPERATOR    IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOT| \
                         IODEFV2_CRITERION_OPERATOR_LESSER|IODEFV2_CRITERION_OPERATOR_GREATER|IODEFV2_CRITERION_OPERATOR_SUBSTR

#define TIME_OPERATOR    IODEFV2_CRITERION_OPERATOR_LESSER|IODEFV2_CRITERION_OPERATOR_GREATER| \
                         IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOT

#define STRING_OPERATOR  IODEFV2_CRITERION_OPERATOR_SUBSTR|IODEFV2_CRITERION_OPERATOR_EQUAL| \
                         IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_NOCASE

#define INTEGER_OPERATOR IODEFV2_CRITERION_OPERATOR_LESSER|IODEFV2_CRITERION_OPERATOR_GREATER|\
                         IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOT

#define ENUM_OPERATOR    STRING_OPERATOR|INTEGER_OPERATOR


#define GENERIC_ONE_BASE_RW_FUNC(scanfmt, printfmt, name, type)                            \
        static int name ## _read(iodefv2_value_type_t *dst, const char *buf)                 \
        {                                                                                  \
                char *endptr;                                                              \
                                                                                           \
                (dst)->data. name ##_val = strtod(buf, &endptr);                           \
                if ( buf == endptr || *endptr != '\0' || errno == ERANGE )                 \
                        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_PARSE, \
                        "Reading " #name " value failed with '%s'", buf);                  \
                return 1;                                                                  \
        }                                                                                  \
                                                                                           \
        static int name ## _write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)    \
        {                                                                                  \
                return libiodefv2_string_sprintf(out, (printfmt), src->data.name ##_val);     \
        }


#define GENERIC_TWO_BASES_RW_FUNC(rfunc, fmt_dec, name, type, min, max)                              \
        static int name ## _read(iodefv2_value_type_t *dst, const char *buf)                           \
        {                                                                                            \
                type tmp;                                                                            \
                char *endptr;                                                                        \
                                                                                                     \
                tmp = rfunc(buf, &endptr, 0);                                                        \
                if ( buf == endptr || *endptr != '\0' || tmp < min || tmp > max || errno == ERANGE ) \
                        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_PARSE,           \
                        "Value out of range, required: [" # min " - " # max "], got '%s'", buf);     \
                                                                                                     \
                dst->data.name ##_val = tmp;                                                         \
                return 0;                                                                            \
        }                                                                                            \
                                                                                                     \
        static int name ## _write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)              \
        {                                                                                            \
                return libiodefv2_string_sprintf(out, (fmt_dec), src->data.name ##_val);                \
        }



typedef struct {
        const char *name;

        size_t len;

        iodefv2_criterion_operator_t operator;

        int (*copy)(const iodefv2_value_type_t *src, void *dst, size_t size);
        int (*clone)(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst, size_t size);
        int (*ref)(const iodefv2_value_type_t *src);

        void (*destroy)(iodefv2_value_type_t *type);
        int (*compare)(const iodefv2_value_type_t *t1, const iodefv2_value_type_t *t2, size_t size, iodefv2_criterion_operator_t op);

        int (*read)(iodefv2_value_type_t *dst, const char *buf);
        int (*write)(const iodefv2_value_type_t *src, libiodefv2_string_t *out);

} iodefv2_value_type_operation_t;



/*
 * We specify a type bigger than the one handled, in order to catch min/max error.
 */
GENERIC_TWO_BASES_RW_FUNC(strtol, "%d", int8, int, LIBIODEFV2_INT8_MIN, LIBIODEFV2_INT8_MAX)
GENERIC_TWO_BASES_RW_FUNC(strtol, "%d", uint8, unsigned int, 0, LIBIODEFV2_UINT8_MAX)
GENERIC_TWO_BASES_RW_FUNC(strtol, "%hd", int16, int, LIBIODEFV2_INT16_MIN, LIBIODEFV2_INT16_MAX)
GENERIC_TWO_BASES_RW_FUNC(strtoul, "%hu", uint16, unsigned int, 0, LIBIODEFV2_UINT16_MAX)
GENERIC_TWO_BASES_RW_FUNC(strtol, "%d", int32, int64_t, LIBIODEFV2_INT32_MIN, LIBIODEFV2_INT32_MAX)
GENERIC_TWO_BASES_RW_FUNC(strtoul, "%u", uint32, uint64_t, 0, LIBIODEFV2_UINT32_MAX)
GENERIC_TWO_BASES_RW_FUNC(strtoll, "%" LIBIODEFV2_PRId64, int64, int64_t, LIBIODEFV2_INT64_MIN, LIBIODEFV2_INT64_MAX)
GENERIC_TWO_BASES_RW_FUNC(strtoull, "%" LIBIODEFV2_PRIu64, uint64, uint64_t, 0, LIBIODEFV2_UINT64_MAX)

GENERIC_ONE_BASE_RW_FUNC("%f", "%f", float, float)
GENERIC_ONE_BASE_RW_FUNC("%lf", "%f", double, double)




/*
 * generic functions.
 */
static int charstring_compare(const char *s1, const char *s2, iodefv2_criterion_operator_t op)
{
        if ( (s1 && !s2) || (s2 && !s1) )
                return -1;

        if ( op == (IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOCASE) && strcasecmp(s1, s2) == 0 )
                return 0;

        else if ( op == IODEFV2_CRITERION_OPERATOR_EQUAL && strcmp(s1, s2) == 0 )
                return 0;

        else if ( op == (IODEFV2_CRITERION_OPERATOR_SUBSTR|IODEFV2_CRITERION_OPERATOR_NOCASE) && strcasestr(s1, s2) )
                return 0;

        else if ( op == IODEFV2_CRITERION_OPERATOR_SUBSTR && strstr(s1, s2) )
                return 0;

        return -1;
}



static int generic_copy(const iodefv2_value_type_t *src, void *dst, size_t size)
{
        memcpy(dst, &src->data, size);
        return 0;
}




static int generic_clone(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst, size_t size)
{
        memcpy(&dst->data, &src->data, size);
        return 0;
}



static int generic_compare(const iodefv2_value_type_t *t1, const iodefv2_value_type_t *t2,
                           size_t size, iodefv2_criterion_operator_t op)
{
        int ret;

        if ( (t1 && ! t2) || (t2 && ! t1) )
                return -1;

        ret = memcmp(&t1->data, &t2->data, size);

        if ( ret == 0 && op & IODEFV2_CRITERION_OPERATOR_EQUAL )
                return 0;

        if ( ret < 0 && op & IODEFV2_CRITERION_OPERATOR_LESSER )
                return 0;

        if ( ret > 0 && op & IODEFV2_CRITERION_OPERATOR_GREATER )
                return 0;

        return -1;
}



/*
 * Enum specific
 */
static int enum_copy(const iodefv2_value_type_t *src, void *dst, size_t size)
{
        *(int *)dst = src->data.enum_val.value;
        return 0;
}


static int enum_read(iodefv2_value_type_t *dst, const char *buf)
{
        int ret;

        ret = sscanf(buf, "%d", &(dst)->data.enum_val.value);

        return (ret == 1) ? 0 : libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_PARSE, "Reading enum value failed");
}



static int enum_write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)
{
        const char *str;

        str = iodefv2_class_enum_to_string(src->data.enum_val.class_id, src->data.enum_val.value);
        if ( ! str )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_PARSE, "Enumeration conversion from numeric to string failed");

        return libiodefv2_string_cat(out, str);
}


static int enum_compare(const iodefv2_value_type_t *src, const iodefv2_value_type_t *dst, size_t size, iodefv2_criterion_operator_t op)
{
        const char *s1;

        if ( (src && ! dst) || (dst && ! src) )
                return -1;

        if ( dst->id == IODEFV2_VALUE_TYPE_STRING ) {
                s1 = iodefv2_class_enum_to_string(src->data.enum_val.class_id, src->data.enum_val.value);
                if ( ! s1 )
                        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_PARSE, "Enumeration conversion from numeric to string failed");

                return charstring_compare(s1, libiodefv2_string_get_string(dst->data.string_val), op);
        }

        return generic_compare(src, dst, size, op);
}


/*
 * time specific function.
 */
static int time_compare(const iodefv2_value_type_t *t1, const iodefv2_value_type_t *t2,
                        size_t size, iodefv2_criterion_operator_t op)
{
        int ret;

        if ( (t1 && ! t2) || (t2 && ! t1) )
                return -1;

        ret = iodefv2_time_compare(t1->data.time_val, t2->data.time_val);
        if ( op & IODEFV2_CRITERION_OPERATOR_EQUAL && ret == 0 )
                return 0;

        else if ( op & IODEFV2_CRITERION_OPERATOR_LESSER && ret < 0 )
                return 0;

        else if ( op & IODEFV2_CRITERION_OPERATOR_GREATER && ret > 0 )
                return 0;

        return -1;
}



static int time_read(iodefv2_value_type_t *dst, const char *buf)
{
        int ret;

        ret = iodefv2_time_new_from_ntpstamp(&dst->data.time_val, buf);
        if ( ret == 0 )
                return 0;

        ret = iodefv2_time_new_from_string(&dst->data.time_val, buf);
        if ( ret == 0 )
                return 0;

        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_PARSE,
                                     "Invalid time format specified: '%s'", buf);
}



static int time_write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)
{
        return iodefv2_time_to_string(src->data.time_val, out);
}



static int time_copy(const iodefv2_value_type_t *src, void *dst, size_t size)
{
        return iodefv2_time_copy(src->data.time_val, dst);
}



static int time_clone(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst, size_t size)
{
        return iodefv2_time_clone(src->data.time_val, &dst->data.time_val);
}


static int time_ref(const iodefv2_value_type_t *src)
{
        iodefv2_time_ref(src->data.time_val);
        return 0;
}


static void time_destroy(iodefv2_value_type_t *type)
{
        iodefv2_time_destroy(type->data.time_val);
}



/*
 *
 */
static int string_compare(const iodefv2_value_type_t *t1, const iodefv2_value_type_t *t2,
                          size_t size, iodefv2_criterion_operator_t op)
{
        const char *s1 = NULL, *s2 = NULL;

        if ( t1 && t1->data.string_val )
                s1 = libiodefv2_string_get_string(t1->data.string_val);

        if ( t2 && t2->data.string_val )
                s2 = libiodefv2_string_get_string(t2->data.string_val);

        return charstring_compare(s1, s2, op);
}



static int string_read(iodefv2_value_type_t *dst, const char *buf)
{
        return libiodefv2_string_new_dup(&dst->data.string_val, buf);
}



static int string_copy(const iodefv2_value_type_t *src, void *dst, size_t size)
{
        return libiodefv2_string_copy_dup(src->data.string_val, dst);
}


static int string_ref(const iodefv2_value_type_t *src)
{
        libiodefv2_string_ref(src->data.string_val);
        return 0;
}


static int string_clone(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst, size_t size)
{
        return libiodefv2_string_clone(src->data.string_val, &dst->data.string_val);
}


static void string_destroy(iodefv2_value_type_t *type)
{
        libiodefv2_string_destroy(type->data.string_val);
}



static int string_write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)
{
        return libiodefv2_string_sprintf(out, "%s",
                       libiodefv2_string_get_string_or_default(src->data.string_val, "<empty>"));
}



/*
 * data specific functions
 */
static int data_compare(const iodefv2_value_type_t *t1, const iodefv2_value_type_t *t2,
                        size_t len, iodefv2_criterion_operator_t op)
{
        int ret;
        size_t s1_len, s2_len;
        const void *s1 = NULL, *s2 = NULL;

        if ( t1 && t1->data.data_val )
                s1 = iodefv2_data_get_data(t1->data.data_val);

        if ( t2 && t2->data.string_val )
                s2 = iodefv2_data_get_data(t2->data.data_val);

        if ( (s1 && !s2) || (s2 && !s1) )
                return -1;

        if ( op & IODEFV2_CRITERION_OPERATOR_SUBSTR ) {
                s1_len = iodefv2_data_get_len(t1->data.data_val);
                s2_len = iodefv2_data_get_len(t2->data.data_val);
                return ( memmem(s1, s1_len, s2, s2_len) ) ? 0 : -1;
        }

        ret = iodefv2_data_compare(t1->data.data_val, t2->data.data_val);
        if ( ret == 0 && op & IODEFV2_CRITERION_OPERATOR_EQUAL )
                return 0;

        else if ( ret < 0 && op & IODEFV2_CRITERION_OPERATOR_LESSER )
                return 0;

        else if ( ret > 0 && op & IODEFV2_CRITERION_OPERATOR_GREATER )
                return 0;


        return -1;
}



static int data_read(iodefv2_value_type_t *dst, const char *src)
{
        return iodefv2_data_new_char_string_dup_fast(&dst->data.data_val, src, strlen(src));
}



static int data_write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)
{
        return iodefv2_data_to_string(src->data.data_val, out);
}



static int data_copy(const iodefv2_value_type_t *src, void *dst, size_t size)
{
        return iodefv2_data_copy_dup(src->data.data_val, dst);
}



static int data_clone(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst, size_t size)
{
        return iodefv2_data_clone(src->data.data_val, &dst->data.data_val);
}


static int data_ref(const iodefv2_value_type_t *src)
{
        iodefv2_data_ref(src->data.data_val);
        return 0;
}


static void data_destroy(iodefv2_value_type_t *type)
{
        iodefv2_data_destroy(type->data.data_val);
}



/*
 * class specific
 */
static int class_compare(const iodefv2_value_type_t *c1,
                         const iodefv2_value_type_t *c2, size_t len, iodefv2_criterion_operator_t op)
{
        if ( (c1 && !c2) || (c2 && !c1) )
                return -1;

        return iodefv2_class_compare(c1->data.class_val.class_id,
                                   c1->data.class_val.object, c2->data.class_val.object);
}


static int class_copy(const iodefv2_value_type_t *src, void *dst, size_t size)
{
        return iodefv2_class_copy(src->data.class_val.class_id, src->data.class_val.object, dst);
}


static int class_clone(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst, size_t size)
{
        dst->data.class_val.class_id = src->data.class_val.class_id;
        return iodefv2_class_clone(src->data.class_val.class_id, src->data.class_val.object, &dst->data.class_val.object);
}


static int class_ref(const iodefv2_value_type_t *src)
{
        return iodefv2_class_ref(src->data.class_val.class_id, src->data.class_val.object);
}


static void class_destroy(iodefv2_value_type_t *type)
{
        iodefv2_class_destroy(type->data.class_val.class_id, type->data.class_val.object);
}



static int list_to_string(const char *cname, iodefv2_class_id_t class, libiodefv2_list_t *head, libiodefv2_string_t *out,
                          int (*item_to_string_cb)(int class, void *item, libiodefv2_string_t *out))
{
        void *obj;
        int ret, j;
        libiodefv2_list_t *tmp;

        if ( libiodefv2_list_is_empty(head) )
                return 0;

        if ( cname )
                ret = libiodefv2_string_sprintf(out, " %s:", cname);
        else
                ret = libiodefv2_string_cat(out, " ");

        if ( ret < 0 )
                return ret;

        ret = libiodefv2_string_cat(out, "(");
        if ( ret < 0 )
                return ret;

        j = 0;
        libiodefv2_list_for_each(head, tmp) {
                if ( j++ > 0 ) {
                        ret = libiodefv2_string_cat(out, ", ");
                        if ( ret < 0 )
                                return ret;
                }

                if ( ! cname )
                        obj = iodefv2_linked_object_get_object(tmp);
                else
                        obj = libiodefv2_linked_object_get_object(tmp);

                ret = item_to_string_cb(class, obj, out);
                if ( ret < 0 )
                        return ret;
        }

        return libiodefv2_string_cat(out, ")");
}



static int listed_str_to_string(iodefv2_class_id_t parent_class, void *parent, libiodefv2_string_t *out)
{
        return libiodefv2_string_cat(out, libiodefv2_string_get_string(parent));
}


static int class_to_string(iodefv2_class_id_t parent_class, void *parent, libiodefv2_string_t *out)
{
        int ret, i;
        void *childptr;
        iodefv2_class_id_t childclass;
        iodefv2_value_type_id_t vtype;
        const char *pname = iodefv2_class_get_name(parent_class);

        ret = libiodefv2_string_sprintf(out, "<IODEFV2%c%s",  toupper(*pname), pname + 1);
        if ( ret < 0 )
                return ret;

        for ( i = 0; ; i++ ) {
                ret = iodefv2_class_get_child(parent, parent_class, i, &childptr);
                if ( ret < 0 )
                        break;

                if ( ! childptr )
                        continue;

                vtype = iodefv2_class_get_child_value_type(parent_class, i);
                childclass = iodefv2_class_get_child_class(parent_class, i);

                if ( vtype != IODEFV2_VALUE_TYPE_CLASS ) {
                        const char *cname = iodefv2_class_get_child_name(parent_class, i);

                        if ( ! iodefv2_class_is_child_list(parent_class, i) ) {
                                ret = libiodefv2_string_sprintf(out, " %s:", cname);
                                if ( ret < 0 ) {
                                        iodefv2_value_destroy(childptr);
                                        return ret;
                                }

                                ret = iodefv2_value_to_string(childptr, out);
                                iodefv2_value_destroy(childptr);
                                if ( ret < 0 )
                                        return ret;
                        }

                        else {
                                ret = list_to_string(cname, childclass, childptr, out, listed_str_to_string);
                                if ( ret < 0 )
                                        return ret;
                        }

                        continue;
                }

                if ( ! iodefv2_class_is_child_list(parent_class, i) ) {
                        ret = libiodefv2_string_cat(out, " ");
                        if ( ret < 0 )
                                return ret;

                        ret = class_to_string(childclass, childptr, out);
                        if ( ret < 0 )
                                return ret;
                }

                else {
                        ret = list_to_string(NULL, childclass, childptr, out, class_to_string);
                        if ( ret < 0 )
                                return ret;
                }
        }

        return libiodefv2_string_cat(out, ">");
}


static int class_write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)
{
        return class_to_string(src->data.class_val.class_id, src->data.class_val.object, out);
}


static const iodefv2_value_type_operation_t ops_tbl[] = {
        { "unknown", 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL                     },
        { "int8", sizeof(int8_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, int8_read, int8_write             },
        { "uint8", sizeof(uint8_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, uint8_read, uint8_write           },
        { "int16", sizeof(int16_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, int16_read, int16_write           },
        { "uint16", sizeof(uint16_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, uint16_read, uint16_write         },
        { "int32", sizeof(int32_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, int32_read, int32_write           },
        { "uint32", sizeof(uint32_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, uint32_read, uint32_write         },
        { "int64", sizeof(int64_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, int64_read, int64_write           },
        { "uint64", sizeof(uint64_t), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, uint64_read, uint64_write         },
        { "float", sizeof(float), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, float_read, float_write           },
        { "double", sizeof(double), INTEGER_OPERATOR, generic_copy,
          generic_clone, NULL, NULL, generic_compare, double_read, double_write         },
        { "string", 0, STRING_OPERATOR, string_copy, string_clone,
          string_ref, string_destroy, string_compare, string_read, string_write         },
        { "time", 0, TIME_OPERATOR, time_copy, time_clone,
          time_ref, time_destroy, time_compare, time_read, time_write                   },
        { "data", 0, DATA_OPERATOR, data_copy, data_clone,
          data_ref, data_destroy, data_compare, data_read, data_write                   },
        { "enum", sizeof(iodefv2_value_type_enum_t), ENUM_OPERATOR, enum_copy,
          generic_clone, NULL, NULL, enum_compare, enum_read, enum_write,               },
        { "list", 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL                        },
        { "class", 0, CLASS_OPERATOR, class_copy, class_clone,
          class_ref, class_destroy, class_compare, NULL, class_write                    },
};



static int is_type_valid(iodefv2_value_type_id_t type)
{
        if ( type < 0 || (size_t) type >= (sizeof(ops_tbl) / sizeof(*ops_tbl)) )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_UNKNOWN, "Unknown IODEFV2 type id: '%d'", type);

        return 0;
}



const char *iodefv2_value_type_to_string(iodefv2_value_type_id_t type)
{
        int ret;

        ret = is_type_valid(type);
        if ( ret < 0 )
                return NULL;

        return ops_tbl[type].name;
}



int iodefv2_value_type_clone(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst)
{
        int ret;

        assert(dst->id == src->id);

        ret = is_type_valid(dst->id);
        if ( ret < 0 )
                return ret;

        if ( ! ops_tbl[dst->id].clone )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_CLONE_UNAVAILABLE,
                                             "Object type '%s' does not support clone operation",
                                             iodefv2_value_type_to_string(dst->id));

        return ops_tbl[dst->id].clone(src, dst, ops_tbl[dst->id].len);
}




int iodefv2_value_type_copy(const iodefv2_value_type_t *src, void *dst)
{
        int ret;

        ret = is_type_valid(src->id);
        if ( ret < 0 )
                return ret;

        if ( ! ops_tbl[src->id].copy )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_COPY_UNAVAILABLE,
                                             "Object type '%s' does not support copy operation",
                                             iodefv2_value_type_to_string(src->id));

        return ops_tbl[src->id].copy(src, dst, ops_tbl[src->id].len);
}



int iodefv2_value_type_ref(const iodefv2_value_type_t *vt)
{
        int ret;

        ret = is_type_valid(vt->id);
        if ( ret < 0 )
                return ret;

        if ( ! ops_tbl[vt->id].ref )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_REF_UNAVAILABLE,
                                             "Object type '%s' does not support ref operation",
                                             iodefv2_value_type_to_string(vt->id));

        return ops_tbl[vt->id].ref(vt);
}


int iodefv2_value_type_compare(const iodefv2_value_type_t *type1,
                             const iodefv2_value_type_t *type2,
                             iodefv2_criterion_operator_t op)
{
        int ret;
        iodefv2_value_type_id_t tid;

        libiodefv2_return_val_if_fail(type1 || type2, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        tid = (type1) ? type1->id : type2->id;

        ret = is_type_valid(tid);
        if ( ret < 0 )
                return ret;

        if ( type1 && type2 && type1->id != type2->id ) {
                if ( type1->id != IODEFV2_VALUE_TYPE_ENUM && type2->id != IODEFV2_VALUE_TYPE_STRING )
                        return libiodefv2_error(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_COMPARE_MISMATCH);
        }

        if ( ! (op & ops_tbl[tid].operator) )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_COMPARE_UNAVAILABLE,
                                             "Object type '%s' does not support operator '%s'",
                                             iodefv2_value_type_to_string(tid), iodefv2_criterion_operator_to_string(op));

        if ( ! ops_tbl[tid].compare )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_COMPARE_UNAVAILABLE,
                                             "Object type '%s' does not support compare operation",
                                             iodefv2_value_type_to_string(tid));

        ret = ops_tbl[tid].compare(type1, type2, ops_tbl[tid].len, op & ~IODEFV2_CRITERION_OPERATOR_NOT);
        if ( ret < 0 ) /* not an error -> no match */
                ret = 1;

        if ( op & IODEFV2_CRITERION_OPERATOR_NOT )
                return (ret == 0) ? 1 : 0;
        else
                return ret;
}




int iodefv2_value_type_read(iodefv2_value_type_t *dst, const char *buf)
{
        int ret;

        ret = is_type_valid(dst->id);
        if ( ret < 0 )
                return ret;

        if ( ! ops_tbl[dst->id].read )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_READ_UNAVAILABLE,
                                             "Object type '%s' does not support read operation",
                                             iodefv2_value_type_to_string(dst->id));

        ret = ops_tbl[dst->id].read(dst, buf);
        return (ret < 0) ? ret : 0;
}




int iodefv2_value_type_write(const iodefv2_value_type_t *src, libiodefv2_string_t *out)
{
        int ret;

        ret = is_type_valid(src->id);
        if ( ret < 0 )
                return ret;

        if ( ! ops_tbl[src->id].write )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_VALUE_TYPE_WRITE_UNAVAILABLE,
                                             "Object type '%s' does not support write operation",
                                             iodefv2_value_type_to_string(src->id));

        return ops_tbl[src->id].write(src, out);
}



void iodefv2_value_type_destroy(iodefv2_value_type_t *type)
{
        int ret;

        ret = is_type_valid(type->id);
        if ( ret < 0 )
                return;

        if ( ! ops_tbl[type->id].destroy )
                return;

        ops_tbl[type->id].destroy(type);
}




int iodefv2_value_type_check_operator(iodefv2_value_type_id_t type, iodefv2_criterion_operator_t op)
{
        int ret;

        ret = is_type_valid(type);
        if ( ret < 0 )
                return ret;

        if ( (~ops_tbl[type].operator & op) == 0 )
                return 0;

        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_CRITERION_UNSUPPORTED_OPERATOR,
                                     "Object type '%s' does not support operator '%s'",
                                     iodefv2_value_type_to_string(type), iodefv2_criterion_operator_to_string(op));
}



int iodefv2_value_type_get_applicable_operators(iodefv2_value_type_id_t type, iodefv2_criterion_operator_t *result)
{
        int ret;

        ret = is_type_valid(type);
        if ( ret < 0 )
                return ret;

        *result = ops_tbl[type].operator;

        return 0;
}
