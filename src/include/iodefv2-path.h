/*****
*
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodefv2-ids.com>
* Author: Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
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

#ifndef _LIBIODEFV2_IODEFV2_PATH_H
#define _LIBIODEFV2_IODEFV2_PATH_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

typedef struct iodefv2_path iodefv2_path_t;

#include <stdarg.h>
#include "libiodefv2-macros.h"
#include "iodefv2-value.h"
#include "iodefv2-tree-wrap.h"

int iodefv2_path_get(const iodefv2_path_t *path, void *object, iodefv2_value_t **ret);

int iodefv2_path_set(const iodefv2_path_t *path, void *object, iodefv2_value_t *value);

int iodefv2_path_new(iodefv2_path_t **path, const char *format, ...) LIBIODEFV2_FMT_PRINTF(2, 3);

int iodefv2_path_new_v(iodefv2_path_t **path, const char *format, va_list args) LIBIODEFV2_FMT_PRINTF(2, 0);

int iodefv2_path_new_from_root_fast(iodefv2_path_t **path, iodefv2_class_id_t rootclass, const char *buffer);

int iodefv2_path_new_fast(iodefv2_path_t **path, const char *buffer);

iodefv2_class_id_t iodefv2_path_get_class(const iodefv2_path_t *path, int depth);

iodefv2_value_type_id_t iodefv2_path_get_value_type(const iodefv2_path_t *path, int depth);

int iodefv2_path_set_index(iodefv2_path_t *path, unsigned int depth, int index);

int iodefv2_path_undefine_index(iodefv2_path_t *path, unsigned int depth);

int iodefv2_path_get_index(const iodefv2_path_t *path, unsigned int depth);

int iodefv2_path_get_key(const iodefv2_path_t *path, unsigned int depth, const char **key);

int iodefv2_path_make_child(iodefv2_path_t *path, const char *child_name, int index);

int iodefv2_path_make_parent(iodefv2_path_t *path);

void iodefv2_path_destroy(iodefv2_path_t *path);

int iodefv2_path_ncompare(const iodefv2_path_t *p1, const iodefv2_path_t *p2, unsigned int depth);

int iodefv2_path_compare(const iodefv2_path_t *p1, const iodefv2_path_t *p2);

int iodefv2_path_clone(const iodefv2_path_t *src, iodefv2_path_t **dst);

iodefv2_path_t *iodefv2_path_ref(iodefv2_path_t *path);

const char *iodefv2_path_get_name(const iodefv2_path_t *path, int depth);

libiodefv2_bool_t iodefv2_path_is_ambiguous(const iodefv2_path_t *path);

int iodefv2_path_has_lists(const iodefv2_path_t *path);

libiodefv2_bool_t iodefv2_path_is_list(const iodefv2_path_t *path, int depth);

unsigned int iodefv2_path_get_depth(const iodefv2_path_t *path);

int iodefv2_path_check_operator(const iodefv2_path_t *path, iodefv2_criterion_operator_t op);

int iodefv2_path_get_applicable_operators(const iodefv2_path_t *path, iodefv2_criterion_operator_t *result);

#ifndef SWIG
void _iodefv2_path_cache_lock(void);

void _iodefv2_path_cache_reinit(void);

void _iodefv2_path_cache_unlock(void);

void _iodefv2_path_cache_destroy(void);
#endif

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_IODEFV2_PATH_H */
