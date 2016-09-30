/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_IODEFV2_CRITERION_VALUE_H
#define _LIBIODEFV2_IODEFV2_CRITERION_VALUE_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct iodefv2_criterion_value iodefv2_criterion_value_t;

typedef enum {
        IODEFV2_CRITERION_VALUE_TYPE_ERROR            = -1,
        IODEFV2_CRITERION_VALUE_TYPE_VALUE            =  0,
        IODEFV2_CRITERION_VALUE_TYPE_REGEX            =  1,
        IODEFV2_CRITERION_VALUE_TYPE_BROKEN_DOWN_TIME =  2
} iodefv2_criterion_value_type_t;

         
#include "iodefv2-criteria.h"
#include "iodefv2-value.h"
 

int iodefv2_criterion_value_new(iodefv2_criterion_value_t **cv);

int iodefv2_criterion_value_new_regex(iodefv2_criterion_value_t **cv, const char *regex, iodefv2_criterion_operator_t op);

int iodefv2_criterion_value_new_value(iodefv2_criterion_value_t **cv, iodefv2_value_t *value,
                                    iodefv2_criterion_operator_t op);

int iodefv2_criterion_value_new_from_string(iodefv2_criterion_value_t **cv, iodefv2_path_t *path,
                                          const char *value, iodefv2_criterion_operator_t op);

int iodefv2_criterion_value_new_broken_down_time(iodefv2_criterion_value_t **cv, const char *time, iodefv2_criterion_operator_t op);
         
int iodefv2_criterion_value_clone(const iodefv2_criterion_value_t *src, iodefv2_criterion_value_t **dst);

void iodefv2_criterion_value_destroy(iodefv2_criterion_value_t *value);

int iodefv2_criterion_value_print(iodefv2_criterion_value_t *value, libiodefv2_io_t *fd);

int iodefv2_criterion_value_to_string(iodefv2_criterion_value_t *value, libiodefv2_string_t *out);

int iodefv2_criterion_value_match(iodefv2_criterion_value_t *cv, iodefv2_value_t *value, iodefv2_criterion_operator_t op);

const iodefv2_value_t *iodefv2_criterion_value_get_value(iodefv2_criterion_value_t *cv);

const char *iodefv2_criterion_value_get_regex(iodefv2_criterion_value_t *cv);

const struct tm *iodefv2_criterion_value_get_broken_down_time(iodefv2_criterion_value_t *cv);
         
iodefv2_criterion_value_type_t iodefv2_criterion_value_get_type(iodefv2_criterion_value_t *cv);

#ifdef __cplusplus
 }
#endif

         
#endif /* _LIBIODEFV2_IODEFV2_CRITERION_VALUE_H */
