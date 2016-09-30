/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_HASH_H
#define _LIBIODEFV2_HASH_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libiodefv2_hash libiodefv2_hash_t;

int libiodefv2_hash_new(libiodefv2_hash_t **hash,
                     unsigned int (*hash_func)(const void *),
                     int (*key_cmp_func)(const void *, const void *),
                     void (*key_destroy_func)(void *),
                     void (*value_destroy_func)(void *));

int libiodefv2_hash_new2(libiodefv2_hash_t **hash, size_t size,
                      unsigned int (*hash_func)(const void *),
                      int (*key_cmp_func)(const void *, const void *),
                      void (*key_destroy_func)(void *),
                     void (*value_destroy_func)(void *));

void libiodefv2_hash_destroy(libiodefv2_hash_t *hash);

int libiodefv2_hash_set(libiodefv2_hash_t *hash, void *key, void *value);

int libiodefv2_hash_add(libiodefv2_hash_t *hash, void *key, void *value);

void *libiodefv2_hash_get(libiodefv2_hash_t *hash, const void *key);

int libiodefv2_hash_elem_destroy(libiodefv2_hash_t *hash, const void *key);

void libiodefv2_hash_iterate(libiodefv2_hash_t *hash, void (*cb)(void *data));

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEFV2_HASH_H */
