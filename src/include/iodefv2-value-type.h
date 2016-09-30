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

#ifndef _IODEFV2_VALUE_TYPE_H
#define _IODEFV2_VALUE_TYPE_H

#include "iodefv2-time.h"
#include "iodefv2-data.h"
#include "libiodefv2-string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IODEFV2_VALUE_TYPE_ERROR   =  -1,
        IODEFV2_VALUE_TYPE_UNKNOWN =   0,
        IODEFV2_VALUE_TYPE_INT8    =   1,
        IODEFV2_VALUE_TYPE_UINT8   =   2,
        IODEFV2_VALUE_TYPE_INT16   =   3,
        IODEFV2_VALUE_TYPE_UINT16  =   4,
        IODEFV2_VALUE_TYPE_INT32   =   5,
        IODEFV2_VALUE_TYPE_UINT32  =   6,
        IODEFV2_VALUE_TYPE_INT64   =   7,
        IODEFV2_VALUE_TYPE_UINT64  =   8,
        IODEFV2_VALUE_TYPE_FLOAT   =   9,
        IODEFV2_VALUE_TYPE_DOUBLE  =  10,
        IODEFV2_VALUE_TYPE_STRING  =  11,
        IODEFV2_VALUE_TYPE_TIME    =  12,
        IODEFV2_VALUE_TYPE_DATA    =  13,
        IODEFV2_VALUE_TYPE_ENUM    =  14,
        IODEFV2_VALUE_TYPE_LIST    =  15,
        IODEFV2_VALUE_TYPE_CLASS   =  16
} iodefv2_value_type_id_t;


typedef struct {
        void *object;
        int class_id;
} iodefv2_value_type_class_t;

typedef struct {
        int value;
        int class_id;
} iodefv2_value_type_enum_t;


typedef union {
        int8_t int8_val;
        uint8_t uint8_val;
        int16_t int16_val;
        uint16_t uint16_val;
        int32_t int32_val;
        uint32_t uint32_val;
        int64_t int64_val;
        uint64_t uint64_val;
        float float_val;
        double double_val;
        libiodefv2_string_t *string_val;
        iodefv2_time_t *time_val;
        iodefv2_data_t *data_val;
        libiodefv2_list_t list_val;
        iodefv2_value_type_enum_t enum_val;
        iodefv2_value_type_class_t class_val;
} iodefv2_value_type_data_t;


typedef struct {
        iodefv2_value_type_id_t id;
        iodefv2_value_type_data_t data;
} iodefv2_value_type_t;


#include "iodefv2-criteria.h"

int iodefv2_value_type_ref(const iodefv2_value_type_t *src);

int iodefv2_value_type_copy(const iodefv2_value_type_t *src, void *dst);

int iodefv2_value_type_read(iodefv2_value_type_t *dst, const char *buf);

int iodefv2_value_type_write(const iodefv2_value_type_t *src, libiodefv2_string_t *out);

void iodefv2_value_type_destroy(iodefv2_value_type_t *type);

int iodefv2_value_type_clone(const iodefv2_value_type_t *src, iodefv2_value_type_t *dst);

int iodefv2_value_type_compare(const iodefv2_value_type_t *type1, const iodefv2_value_type_t *type2,
                             iodefv2_criterion_operator_t op);

int iodefv2_value_type_check_operator(iodefv2_value_type_id_t type, iodefv2_criterion_operator_t op);

int iodefv2_value_type_get_applicable_operators(iodefv2_value_type_id_t type, iodefv2_criterion_operator_t *result);

const char *iodefv2_value_type_to_string(iodefv2_value_type_id_t type);

#ifdef __cplusplus
 }
#endif

#endif
