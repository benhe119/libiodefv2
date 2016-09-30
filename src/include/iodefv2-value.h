/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@libiodefv2-ids.com>
* Author: Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
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

#ifndef _LIBIODEFV2_IODEFV2_VALUE_H
#define _LIBIODEFV2_IODEFV2_VALUE_H


typedef struct iodefv2_value iodefv2_value_t;

#include "libiodefv2-io.h"
#include "iodefv2-value-type.h"
#include "libiodefv2-string.h"
#include "iodefv2-class.h"
#include "iodefv2-path.h"

#ifdef __cplusplus
 extern "C" {
#endif

int iodefv2_value_new_int8(iodefv2_value_t **value, int8_t val);
int iodefv2_value_new_uint8(iodefv2_value_t **value, uint8_t val);
int iodefv2_value_new_int16(iodefv2_value_t **value, int16_t val);
int iodefv2_value_new_uint16(iodefv2_value_t **value, uint16_t val);
int iodefv2_value_new_int32(iodefv2_value_t **value, int32_t val);
int iodefv2_value_new_uint32(iodefv2_value_t **value, uint32_t val);
int iodefv2_value_new_int64(iodefv2_value_t **value, int64_t val);
int iodefv2_value_new_uint64(iodefv2_value_t **value, uint64_t val);
int iodefv2_value_new_float(iodefv2_value_t **value, float val);
int iodefv2_value_new_double(iodefv2_value_t **value, double val);
int iodefv2_value_new_string(iodefv2_value_t **value, libiodefv2_string_t *string);
int iodefv2_value_new_time(iodefv2_value_t **value, iodefv2_time_t *time);
int iodefv2_value_new_data(iodefv2_value_t **value, iodefv2_data_t *data);
int iodefv2_value_new_class(iodefv2_value_t **value, iodefv2_class_id_t classid, void *ptr);
int iodefv2_value_new_list(iodefv2_value_t **value);
int iodefv2_value_new_enum(iodefv2_value_t **value, iodefv2_class_id_t classid, const char *buf);
int iodefv2_value_new_enum_from_string(iodefv2_value_t **value, iodefv2_class_id_t classid, const char *buf);
int iodefv2_value_new_enum_from_numeric(iodefv2_value_t **value, iodefv2_class_id_t classid, int val);

int iodefv2_value_set_int8(iodefv2_value_t *value, int8_t val);
int iodefv2_value_set_uint8(iodefv2_value_t *value, uint8_t val);
int iodefv2_value_set_int16(iodefv2_value_t *value, int16_t val);
int iodefv2_value_set_uint16(iodefv2_value_t *value, uint16_t val);
int iodefv2_value_set_int32(iodefv2_value_t *value, int32_t val);
int iodefv2_value_set_uint32(iodefv2_value_t *value, uint32_t val);
int iodefv2_value_set_int64(iodefv2_value_t *value, int64_t val);
int iodefv2_value_set_uint64(iodefv2_value_t *value, uint64_t val);
int iodefv2_value_set_float(iodefv2_value_t *value, float val);
int iodefv2_value_set_double(iodefv2_value_t *value, double val);
int iodefv2_value_set_string(iodefv2_value_t *value, libiodefv2_string_t *string);
int iodefv2_value_set_time(iodefv2_value_t *value, iodefv2_time_t *time);
int iodefv2_value_set_data(iodefv2_value_t *value, iodefv2_data_t *data);
int iodefv2_value_set_enum(iodefv2_value_t *value, iodefv2_class_id_t classid, const char *buf);
int iodefv2_value_set_enum_from_string(iodefv2_value_t *value, iodefv2_class_id_t classid, const char *buf);
int iodefv2_value_set_enum_from_numeric(iodefv2_value_t *value, iodefv2_class_id_t classid, int no);
int iodefv2_value_set_class(iodefv2_value_t *value, iodefv2_class_id_t classid, void *ptr);

int iodefv2_value_new(iodefv2_value_t **value, iodefv2_value_type_id_t type, void *ptr);
int iodefv2_value_new_from_path(iodefv2_value_t **value, iodefv2_path_t *path, const char *buf);
int iodefv2_value_new_from_string(iodefv2_value_t **value, iodefv2_value_type_id_t type, const char *buf);

int8_t iodefv2_value_get_int8(const iodefv2_value_t *val);
uint8_t iodefv2_value_get_uint8(const iodefv2_value_t *val);
int16_t iodefv2_value_get_int16(const iodefv2_value_t *val);
uint16_t iodefv2_value_get_uint16(const iodefv2_value_t *val);
int32_t iodefv2_value_get_int32(const iodefv2_value_t *val);
uint32_t iodefv2_value_get_uint32(const iodefv2_value_t *val);
int64_t iodefv2_value_get_int64(const iodefv2_value_t *val);
uint64_t iodefv2_value_get_uint64(const iodefv2_value_t *val);
int iodefv2_value_get_enum(const iodefv2_value_t *val);
float iodefv2_value_get_float(const iodefv2_value_t *val);
double iodefv2_value_get_double(const iodefv2_value_t *val);

iodefv2_time_t *iodefv2_value_get_time(const iodefv2_value_t *val);
iodefv2_data_t *iodefv2_value_get_data(const iodefv2_value_t *val);
libiodefv2_string_t *iodefv2_value_get_string(const iodefv2_value_t *val);

int iodefv2_value_list_add(iodefv2_value_t *list, iodefv2_value_t *item);
libiodefv2_bool_t iodefv2_value_is_list(const iodefv2_value_t *list);
libiodefv2_bool_t iodefv2_value_list_is_empty(const iodefv2_value_t *list);

int iodefv2_value_have_own_data(iodefv2_value_t *value);
int iodefv2_value_dont_have_own_data(iodefv2_value_t *value);

iodefv2_value_type_id_t iodefv2_value_get_type(const iodefv2_value_t *value);
iodefv2_class_id_t iodefv2_value_get_class(const iodefv2_value_t *value);

void *iodefv2_value_get_object(const iodefv2_value_t *value);

int iodefv2_value_iterate(const iodefv2_value_t *value, int (*callback)(iodefv2_value_t *ptr, void *extra), void *extra);

int iodefv2_value_iterate_reversed(const iodefv2_value_t *value, int (*callback)(iodefv2_value_t *ptr, void *extra), void *extra);

int iodefv2_value_get_nth2(const iodefv2_value_t *val, int index, iodefv2_value_t **ret);

iodefv2_value_t *iodefv2_value_get_nth(const iodefv2_value_t *val, int n);

int iodefv2_value_get_count(const iodefv2_value_t *val);

int iodefv2_value_clone(const iodefv2_value_t *val, iodefv2_value_t **dst);

iodefv2_value_t *iodefv2_value_ref(iodefv2_value_t *val);

int iodefv2_value_print(const iodefv2_value_t *val, libiodefv2_io_t *fd);

int iodefv2_value_to_string(const iodefv2_value_t *value, libiodefv2_string_t *out);

int iodefv2_value_get(const iodefv2_value_t *val, void *res);

int iodefv2_value_match(iodefv2_value_t *val1, iodefv2_value_t *val2, iodefv2_criterion_operator_t op);

int iodefv2_value_check_operator(const iodefv2_value_t *value, iodefv2_criterion_operator_t op);

int iodefv2_value_get_applicable_operators(const iodefv2_value_t *value, iodefv2_criterion_operator_t *result);

void iodefv2_value_destroy(iodefv2_value_t *val);

#ifndef SWIG

int _iodefv2_value_copy_internal(const iodefv2_value_t *val,
                               iodefv2_value_type_id_t res_type, iodefv2_class_id_t res_id, void *res);

int _iodefv2_value_cast(iodefv2_value_t *val, iodefv2_value_type_id_t target_type, iodefv2_class_id_t enum_class);
#endif

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_IODEFV2_VALUE_H */
