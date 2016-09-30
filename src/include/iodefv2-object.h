/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
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

#ifndef _LIBIODEFV2_IODEFV2_OBJECT_H
#define _LIBIODEFV2_IODEFV2_OBJECT_H

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libiodefv2-io.h"
#include "libiodefv2-list.h"

typedef struct iodefv2_object iodefv2_object_t;

iodefv2_class_id_t iodefv2_object_get_class(iodefv2_object_t *obj);

iodefv2_object_t *iodefv2_object_ref(iodefv2_object_t *obj);

void iodefv2_object_destroy(iodefv2_object_t *obj);

int iodefv2_object_compare(iodefv2_object_t *obj1, iodefv2_object_t *obj2);

int iodefv2_object_clone(iodefv2_object_t *obj, iodefv2_object_t **dst);

int iodefv2_object_copy(iodefv2_object_t *src, iodefv2_object_t *dst);

int iodefv2_object_print(iodefv2_object_t *obj, libiodefv2_io_t *fd);

int iodefv2_object_print_json(iodefv2_object_t *obj, libiodefv2_io_t *fd);

int iodefv2_object_print_binary(iodefv2_object_t *obj, libiodefv2_io_t *fd);

void iodefv2_object_add(libiodefv2_list_t *head, iodefv2_object_t *obj);

void iodefv2_object_add_tail(libiodefv2_list_t *head, iodefv2_object_t *obj);

void iodefv2_object_del(iodefv2_object_t *object);

void iodefv2_object_del_init(iodefv2_object_t *object);

void *iodefv2_object_get_list_entry(libiodefv2_list_t *listm);

int iodefv2_object_new_from_json(iodefv2_object_t **object, const char * json_message);

#ifdef __cplusplus
  }
#endif

#endif /* _LIBIODEFV2_IODEFV2_OBJECT_H */
