/*****
*
* Copyright (C) 2002, 2003, 2004 Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
* All Rights Reserved
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


#ifndef _LIBIODEFV2_IODEFV2_CLASS_H
#define _LIBIODEFV2_IODEFV2_CLASS_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef int iodefv2_class_id_t;
typedef int iodefv2_class_child_id_t;

#include "iodefv2-value.h"

/*
 *
 */
libiodefv2_bool_t iodefv2_class_is_child_list(iodefv2_class_id_t classid, iodefv2_class_child_id_t child);

libiodefv2_bool_t iodefv2_class_is_child_keyed_list(iodefv2_class_id_t classid, iodefv2_class_child_id_t child);

libiodefv2_bool_t iodefv2_class_is_child_union_member(iodefv2_class_id_t classid, iodefv2_class_child_id_t child);

int iodefv2_class_get_child_union_id(iodefv2_class_id_t classid, iodefv2_class_child_id_t child);

iodefv2_class_id_t iodefv2_class_get_child_class(iodefv2_class_id_t classid, iodefv2_class_child_id_t child);

size_t iodefv2_class_get_child_count(iodefv2_class_id_t classid);

iodefv2_value_type_id_t iodefv2_class_get_child_value_type(iodefv2_class_id_t classid, iodefv2_class_child_id_t child);


/*
 *
 */
int iodefv2_class_enum_to_numeric(iodefv2_class_id_t classid, const char *val);

const char *iodefv2_class_enum_to_string(iodefv2_class_id_t classid, int val);


/*
 *
 */
int iodefv2_class_get_child(void *ptr, iodefv2_class_id_t classid, iodefv2_class_child_id_t child, void **childptr);

int iodefv2_class_new_child(void *ptr, iodefv2_class_id_t classid, iodefv2_class_child_id_t child, int n, void **childptr);

int iodefv2_class_destroy_child(void *ptr, iodefv2_class_id_t classid, iodefv2_class_child_id_t child, int n);


/*
 *
 */
iodefv2_class_id_t iodefv2_class_find(const char *name);

iodefv2_class_child_id_t iodefv2_class_find_child(iodefv2_class_id_t classid, const char *name);

/*
 *
 */
const char *iodefv2_class_get_name(iodefv2_class_id_t classid);

const char *iodefv2_class_get_child_name(iodefv2_class_id_t classid, iodefv2_class_child_id_t child);


/*
 *
 */
int iodefv2_class_copy(iodefv2_class_id_t classid, const void *src, void *dst);

int iodefv2_class_clone(iodefv2_class_id_t classid, const void *src, void **dst);

int iodefv2_class_compare(iodefv2_class_id_t classid, const void *c1, const void *c2);

int iodefv2_class_ref(iodefv2_class_id_t classid, void *obj);

int iodefv2_class_print(iodefv2_class_id_t classid, void *obj, libiodefv2_io_t *fd);

int iodefv2_class_print_json(iodefv2_class_id_t classid, void *obj, libiodefv2_io_t *fd);

int iodefv2_class_print_binary(iodefv2_class_id_t classid, void *obj, libiodefv2_io_t *fd);

int iodefv2_class_destroy(iodefv2_class_id_t classid, void *obj);

libiodefv2_bool_t iodefv2_class_is_listed(iodefv2_class_id_t classid);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_IODEFV2_CLASS_H */
