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

#ifndef _LIBIODEFV2_LIBIODEFV2_IO_H
#define _LIBIODEFV2_LIBIODEFV2_IO_H

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include "libiodefv2-inttypes.h"


typedef struct libiodefv2_io libiodefv2_io_t;

/*
 * Object creation / destruction functions.
 */
int libiodefv2_io_new(libiodefv2_io_t **ret);

void libiodefv2_io_destroy(libiodefv2_io_t *pio);

void libiodefv2_io_set_file_io(libiodefv2_io_t *pio, FILE *fd);

void libiodefv2_io_set_tls_io(libiodefv2_io_t *pio, void *tls);

void libiodefv2_io_set_sys_io(libiodefv2_io_t *pio, int fd);

int libiodefv2_io_set_buffer_io(libiodefv2_io_t *pio);


/*
 *
 */
void libiodefv2_io_set_fdptr(libiodefv2_io_t *pio, void *ptr);
void libiodefv2_io_set_write_callback(libiodefv2_io_t *pio, ssize_t (*write)(libiodefv2_io_t *io, const void *buf, size_t count));
void libiodefv2_io_set_read_callback(libiodefv2_io_t *pio, ssize_t (*read)(libiodefv2_io_t *io, void *buf, size_t count));
void libiodefv2_io_set_pending_callback(libiodefv2_io_t *pio, ssize_t (*pending)(libiodefv2_io_t *io));


/*
 * IO operations.
 */
int libiodefv2_io_close(libiodefv2_io_t *pio);

ssize_t libiodefv2_io_read(libiodefv2_io_t *pio, void *buf, size_t count);

ssize_t libiodefv2_io_read_wait(libiodefv2_io_t *pio, void *buf, size_t count);

ssize_t libiodefv2_io_read_delimited(libiodefv2_io_t *pio, unsigned char **buf);


ssize_t libiodefv2_io_write(libiodefv2_io_t *pio, const void *buf, size_t count);

ssize_t libiodefv2_io_write_delimited(libiodefv2_io_t *pio, const void *buf, uint16_t count);


ssize_t libiodefv2_io_forward(libiodefv2_io_t *dst, libiodefv2_io_t *src, size_t count);

int libiodefv2_io_get_fd(libiodefv2_io_t *pio);

void *libiodefv2_io_get_fdptr(libiodefv2_io_t *pio);

ssize_t libiodefv2_io_pending(libiodefv2_io_t *pio);

libiodefv2_bool_t libiodefv2_io_is_error_fatal(libiodefv2_io_t *pio, int error);

#ifdef __cplusplus
  }
#endif

#endif /* _LIBIODEFV2_LIBIODEFV2_IO_H */
