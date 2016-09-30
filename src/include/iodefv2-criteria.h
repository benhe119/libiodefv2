/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_IODEFV2_CRITERIA_H
#define _LIBIODEFV2_IODEFV2_CRITERIA_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IODEFV2_CRITERION_OPERATOR_NOT               = 0x8000,
        IODEFV2_CRITERION_OPERATOR_NOCASE            = 0x4000,

        IODEFV2_CRITERION_OPERATOR_EQUAL             = 0x0001,
        IODEFV2_CRITERION_OPERATOR_EQUAL_NOCASE      = IODEFV2_CRITERION_OPERATOR_EQUAL|IODEFV2_CRITERION_OPERATOR_NOCASE,
        IODEFV2_CRITERION_OPERATOR_NOT_EQUAL         = IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_EQUAL,
        IODEFV2_CRITERION_OPERATOR_NOT_EQUAL_NOCASE  = IODEFV2_CRITERION_OPERATOR_NOT_EQUAL|IODEFV2_CRITERION_OPERATOR_EQUAL_NOCASE,

        IODEFV2_CRITERION_OPERATOR_LESSER            = 0x0002,
        IODEFV2_CRITERION_OPERATOR_LESSER_OR_EQUAL   = IODEFV2_CRITERION_OPERATOR_LESSER|IODEFV2_CRITERION_OPERATOR_EQUAL,

        IODEFV2_CRITERION_OPERATOR_GREATER           = 0x0004,
        IODEFV2_CRITERION_OPERATOR_GREATER_OR_EQUAL  = IODEFV2_CRITERION_OPERATOR_GREATER|IODEFV2_CRITERION_OPERATOR_EQUAL,

        IODEFV2_CRITERION_OPERATOR_SUBSTR            = 0x0008,
        IODEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE     = IODEFV2_CRITERION_OPERATOR_SUBSTR|IODEFV2_CRITERION_OPERATOR_NOCASE,
        IODEFV2_CRITERION_OPERATOR_NOT_SUBSTR        = IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_SUBSTR,
        IODEFV2_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE = IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE,

        IODEFV2_CRITERION_OPERATOR_REGEX             = 0x0010,
        IODEFV2_CRITERION_OPERATOR_REGEX_NOCASE      = IODEFV2_CRITERION_OPERATOR_REGEX|IODEFV2_CRITERION_OPERATOR_NOCASE,
        IODEFV2_CRITERION_OPERATOR_NOT_REGEX         = IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_REGEX,
        IODEFV2_CRITERION_OPERATOR_NOT_REGEX_NOCASE  = IODEFV2_CRITERION_OPERATOR_NOT|IODEFV2_CRITERION_OPERATOR_REGEX_NOCASE,

        IODEFV2_CRITERION_OPERATOR_NULL              = 0x0020,
        IODEFV2_CRITERION_OPERATOR_NOT_NULL          = IODEFV2_CRITERION_OPERATOR_NULL|IODEFV2_CRITERION_OPERATOR_NOT
} iodefv2_criterion_operator_t;


typedef struct iodefv2_criteria iodefv2_criteria_t;
typedef struct iodefv2_criterion iodefv2_criterion_t;

#include "iodefv2-path.h"
#include "iodefv2-criterion-value.h"

const char *iodefv2_criterion_operator_to_string(iodefv2_criterion_operator_t op);

int iodefv2_criterion_new(iodefv2_criterion_t **criterion, iodefv2_path_t *path,
                        iodefv2_criterion_value_t *value, iodefv2_criterion_operator_t op);

void iodefv2_criterion_destroy(iodefv2_criterion_t *criterion);
iodefv2_criteria_t *iodefv2_criteria_ref(iodefv2_criteria_t *criteria);
int iodefv2_criterion_clone(const iodefv2_criterion_t *criterion, iodefv2_criterion_t **dst);
int iodefv2_criterion_print(const iodefv2_criterion_t *criterion, libiodefv2_io_t *fd);
int iodefv2_criterion_to_string(const iodefv2_criterion_t *criterion, libiodefv2_string_t *out);
iodefv2_path_t *iodefv2_criterion_get_path(const iodefv2_criterion_t *criterion);
iodefv2_criterion_value_t *iodefv2_criterion_get_value(const iodefv2_criterion_t *criterion);
iodefv2_criterion_operator_t iodefv2_criterion_get_operator(const iodefv2_criterion_t *criterion);
int iodefv2_criterion_match(const iodefv2_criterion_t *criterion, void *object);

int iodefv2_criteria_new(iodefv2_criteria_t **criteria);
void iodefv2_criteria_destroy(iodefv2_criteria_t *criteria);
int iodefv2_criteria_clone(iodefv2_criteria_t *src, iodefv2_criteria_t **dst);
int iodefv2_criteria_print(const iodefv2_criteria_t *criteria, libiodefv2_io_t *fd);
int iodefv2_criteria_to_string(const iodefv2_criteria_t *criteria, libiodefv2_string_t *out);
libiodefv2_bool_t iodefv2_criteria_is_criterion(const iodefv2_criteria_t *criteria);
iodefv2_criterion_t *iodefv2_criteria_get_criterion(const iodefv2_criteria_t *criteria);
void iodefv2_criteria_set_criterion(iodefv2_criteria_t *criteria, iodefv2_criterion_t *criterion);

void iodefv2_criteria_or_criteria(iodefv2_criteria_t *criteria, iodefv2_criteria_t *criteria2);

int iodefv2_criteria_and_criteria(iodefv2_criteria_t *criteria, iodefv2_criteria_t *criteria2);

int iodefv2_criteria_match(const iodefv2_criteria_t *criteria, void *object);

iodefv2_criteria_t *iodefv2_criteria_get_or(const iodefv2_criteria_t *criteria);

iodefv2_criteria_t *iodefv2_criteria_get_and(const iodefv2_criteria_t *criteria);

int iodefv2_criteria_new_from_string(iodefv2_criteria_t **criteria, const char *str);

void iodefv2_criteria_set_negation(iodefv2_criteria_t *criteria, libiodefv2_bool_t negate);

libiodefv2_bool_t iodefv2_criteria_get_negation(const iodefv2_criteria_t *criteria);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_IODEFV2_CRITERIA_H */
