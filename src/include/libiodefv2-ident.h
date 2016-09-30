/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_LIBIODEFV2_IDENT_H
#define _LIBIODEFV2_LIBIODEFV2_IDENT_H

#include "libiodefv2-inttypes.h"
#include "libiodefv2-string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libiodefv2_ident libiodefv2_ident_t;

int libiodefv2_ident_generate(libiodefv2_ident_t *ident, libiodefv2_string_t *out);

uint64_t libiodefv2_ident_inc(libiodefv2_ident_t *ident);

void libiodefv2_ident_destroy(libiodefv2_ident_t *ident);

int libiodefv2_ident_new(libiodefv2_ident_t **ret);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_LIBIODEFV2_IDENT_H */
