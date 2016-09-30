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

#ifndef _LIBIODEFV2_LIBIODEFV2_FAILOVER_H
#define _LIBIODEFV2_LIBIODEFV2_FAILOVER_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libiodefv2_failover libiodefv2_failover_t;

void libiodefv2_failover_destroy(libiodefv2_failover_t *failover);

int libiodefv2_failover_new(libiodefv2_failover_t **ret, const char *dirname);

void libiodefv2_failover_set_quota(libiodefv2_failover_t *failover, size_t limit);

int libiodefv2_failover_save_msg(libiodefv2_failover_t *failover, libiodefv2_msg_t *msg);

ssize_t libiodefv2_failover_get_saved_msg(libiodefv2_failover_t *failover, libiodefv2_msg_t **out);

unsigned long libiodefv2_failover_get_deleted_msg_count(libiodefv2_failover_t *failover);

unsigned long libiodefv2_failover_get_available_msg_count(libiodefv2_failover_t *failover);

void libiodefv2_failover_enable_transaction(libiodefv2_failover_t *failover);

void libiodefv2_failover_disable_transaction(libiodefv2_failover_t *failover);

int libiodefv2_failover_commit(libiodefv2_failover_t *failover, libiodefv2_msg_t *msg);

int libiodefv2_failover_rollback(libiodefv2_failover_t *failover, libiodefv2_msg_t *msg);

#ifdef __cplusplus
 }
#endif

#endif
