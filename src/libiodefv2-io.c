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

#include "config.h"
#include "libmissing.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/ioctl.h>

#include "libiodefv2-inttypes.h"

#ifdef HAVE_SYS_FILIO_H
# include <sys/filio.h>
#endif


#include "libiodefv2-log.h"
#include "libiodefv2-io.h"
#include "common.h"


#define LIBIODEFV2_ERROR_SOURCE_DEFAULT LIBIODEFV2_ERROR_SOURCE_IO
#include "libiodefv2-error.h"


#define CHUNK_SIZE 1024

#ifndef MIN
# define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

struct libiodefv2_io {

        int fd;
        void *fd_ptr;

        size_t size;
        size_t rindex;

        int (*close)(libiodefv2_io_t *pio);
        ssize_t (*read)(libiodefv2_io_t *pio, void *buf, size_t count);
        ssize_t (*write)(libiodefv2_io_t *pio, const void *buf, size_t count);
        ssize_t (*pending)(libiodefv2_io_t *pio);
};



/*
 * Buffer IO functions.
 */
static ssize_t buffer_read(libiodefv2_io_t *pio, void *buf, size_t count)
{
        if ( pio->size - pio->rindex == 0 )
                return 0;

        count = MIN(count, pio->size - pio->rindex);

        memcpy(buf, (unsigned char *) pio->fd_ptr + pio->rindex, count);
        pio->rindex += count;

        return count;
}



static ssize_t buffer_write(libiodefv2_io_t *pio, const void *buf, size_t count)
{
        unsigned char *new;

        if ( pio->size + count <= pio->size )
                return -1;

        new = _libiodefv2_realloc(pio->fd_ptr, pio->size + count);
        if ( ! new )
                return libiodefv2_error_from_errno(errno);

        memcpy(new + pio->size, buf, count);

        pio->fd_ptr = new;
        pio->size += count;

        return count;
}



static int buffer_close(libiodefv2_io_t *pio)
{
        if ( pio->fd_ptr ) {
                free(pio->fd_ptr);
                pio->fd_ptr = NULL;
                pio->size = pio->rindex = 0;
        }

        return 0;
}



static ssize_t buffer_pending(libiodefv2_io_t *pio)
{
        return pio->size - pio->rindex;
}




/*
 * System IO functions.
 */
static ssize_t sys_read(libiodefv2_io_t *pio, void *buf, size_t count)
{
        ssize_t ret;

        do {
                ret = read(pio->fd, buf, count);
        } while ( ret < 0 && errno == EINTR );

        if ( ret == 0 )
                return libiodefv2_error(LIBIODEFV2_ERROR_EOF);

        if ( ret < 0 ) {
#ifdef ECONNRESET
                if ( errno == ECONNRESET )
                        return libiodefv2_error(LIBIODEFV2_ERROR_EOF);
#endif
                return libiodefv2_error_from_errno(errno);
        }

        return ret;
}



static ssize_t sys_write(libiodefv2_io_t *pio, const void *buf, size_t count)
{
        ssize_t ret;

        do {
                ret = write(pio->fd, buf, count);
        } while ( ret < 0 && errno == EINTR );

        if ( ret < 0 )
                return libiodefv2_error_from_errno(errno);

        return ret;
}



static int sys_close(libiodefv2_io_t *pio)
{
        int ret;

        do {
                ret = close(pio->fd);
        } while ( ret < 0 && errno == EINTR );

        return (ret >= 0) ? ret : libiodefv2_error_from_errno(errno);
}



static ssize_t sys_pending(libiodefv2_io_t *pio)
{
        long ret = 0;

        if ( ioctl(pio->fd, FIONREAD, &ret) < 0 )
                return libiodefv2_error_from_errno(errno);

        return ret;
}




/*
 * Buffered IO functions.
 */
static ssize_t file_read(libiodefv2_io_t *pio, void *buf, size_t count)
{
        FILE *fd;
        size_t ret;

        /*
         * ferror / clearerror can be macro that might dereference fd_ptr.
         */
        fd = pio->fd_ptr;
        libiodefv2_return_val_if_fail(fd, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = fread(buf, count, 1, fd);
        if ( ret <= 0 ) {
                ret = ferror(fd) ? libiodefv2_error_from_errno(errno) : libiodefv2_error(LIBIODEFV2_ERROR_EOF);
                clearerr(fd);
                return ret;
        }

        /*
         * fread return the number of *item* read.
         */
        return count;
}



static ssize_t file_write(libiodefv2_io_t *pio, const void *buf, size_t count)
{
        size_t ret;

        libiodefv2_return_val_if_fail(pio->fd_ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = fwrite(buf, count, 1, pio->fd_ptr);
        if ( ret <= 0 )
                return ret;

        /*
         * fwrite return the number of *item* written.
         */
        return count;
}



static int file_close(libiodefv2_io_t *pio)
{
        libiodefv2_return_val_if_fail(pio->fd_ptr, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return fclose(pio->fd_ptr);
}



static ssize_t file_pending(libiodefv2_io_t *pio)
{
#ifdef ENOTSUP
        return libiodefv2_error_from_errno(ENOTSUP);
#else
        return libiodefv2_error(LIBIODEFV2_ERROR_GENERIC);
#endif
}




/*
 * Forward data from one fd to another using copy.
 */
static ssize_t copy_forward(libiodefv2_io_t *dst, libiodefv2_io_t *src, size_t count)
{
        int ret;
        size_t scount;
        unsigned char buf[8192];

        scount = count;

        while ( count ) {

                ret = (count < sizeof(buf)) ? count : sizeof(buf);

                ret = libiodefv2_io_read(src, buf, ret);
                if ( ret <= 0 )
                        return ret;

                count -= ret;

                ret = libiodefv2_io_write(dst, buf, ret);
                if ( ret < 0 )
                        return ret;
        }

        return scount;
}





/**
 * libiodefv2_io_forward:
 * @src: Pointer to a #libiodefv2_io_t object.
 * @dst: Pointer to a #libiodefv2_io_t object.
 * @count: Number of byte to forward from @src to @dst.
 *
 * libiodefv2_io_forward() attempts to transfer up to @count bytes from
 * the file descriptor identified by @src into the file descriptor
 * identified by @dst.
 *
 * Returns: If the transfer was successful, the number of bytes written
 * to @dst is returned.  On error, -1 is returned, and errno is set appropriately.
 */
ssize_t libiodefv2_io_forward(libiodefv2_io_t *dst, libiodefv2_io_t *src, size_t count)
{
        libiodefv2_return_val_if_fail(dst, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(src, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return copy_forward(dst, src, count);
}




/**
 * libiodefv2_io_read:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @buf: Pointer to the buffer to store data into.
 * @count: Number of bytes to read.
 *
 * libiodefv2_io_read() attempts to read up to @count bytes from the
 * file descriptor identified by @pio into the buffer starting at @buf.
 *
 * If @count is zero, libiodefv2_io_read() returns zero and has no other
 * results. If @count is greater than SSIZE_MAX, the result is unspecified.
 *
 * The case where the read function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes read is returned (zero
 * indicates end of file). It is not an error if this number is smaller
 * than the number of bytes requested; this may happen for example because
 * fewer bytes are actually available right now or because read() was
 * interrupted by a signal.
 *
 * On error, a negative value is returned. In this case it is left unspecified
 * whether the file position (if any) changes.
 */
ssize_t libiodefv2_io_read(libiodefv2_io_t *pio, void *buf, size_t count)
{
        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(pio->read, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return pio->read(pio, buf, count);
}




/**
 * libiodefv2_io_read_wait:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @buf: Pointer to the buffer to store data into.
 * @count: Number of bytes to read.
 *
 * libiodefv2_io_read_wait() attempts to read up to @count bytes from the
 * file descriptor identified by @pio into the buffer starting at @buf.
 *
 * If @count is zero, libiodefv2_io_read() returns zero and has no other
 * results. If @count is greater than SSIZE_MAX, the result is unspecified.
 *
 * The case where the read function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * libiodefv2_io_read_wait() always return the number of bytes requested.
 * Be carefull that this function is blocking.
 *
 * Returns: On success, the number of bytes read is returned (zero
 * indicates end of file).
 *
 * On error, -1 is returned, and errno is set appropriately. In this
 * case it is left unspecified whether the file position (if any) changes.
 */
ssize_t libiodefv2_io_read_wait(libiodefv2_io_t *pio, void *buf, size_t count)
{
        ssize_t ret;
        size_t n = 0;
        struct pollfd pfd;
        unsigned char *in = buf;

        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        pfd.fd = libiodefv2_io_get_fd(pio);
        pfd.events = POLLIN;

        do {
                ret = poll(&pfd, 1, -1);
                if ( ret < 0 )
                        return libiodefv2_error_from_errno(errno);

                if ( ! (pfd.revents & POLLIN) )
                        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_GENERIC, "expected POLLIN event");

                ret = libiodefv2_io_read(pio, &in[n], count - n);
                if ( ret < 0 )
                        return ret;

                n += (size_t) ret;

        } while ( n != count );

        return (ssize_t) n;
}



/**
 * libiodefv2_io_read_delimited:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @buf: Pointer to the address of a buffer to store address of data into.
 *
 * libiodefv2_io_read_delimited() read message written by libiodefv2_write_delimited().
 * Theses messages are sents along with the len of the message.
 *
 * Uppon return the @buf argument is updated to point on a newly allocated
 * buffer containing the data read. The @count argument is set to the number of
 * bytes the message was containing.
 *
 * The case where the read function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes read is returned (zero
 * indicates end of file).
 *
 * On error, -1 is returned, and errno is set appropriately. In this
 * case it is left unspecified whether the file position (if any) changes.
 */
ssize_t libiodefv2_io_read_delimited(libiodefv2_io_t *pio, unsigned char **buf)
{
        ssize_t ret;
        size_t count;
        uint16_t msglen;

        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = libiodefv2_io_read_wait(pio, &msglen, sizeof(msglen));
        if ( ret <= 0 )
                return ret;

        count = ntohs(msglen);

        *buf = malloc(count);
        if ( ! *buf )
                return libiodefv2_error_from_errno(errno);

        ret = libiodefv2_io_read_wait(pio, *buf, count);
        if ( ret < 0 )
                return ret;

        return count;
}




/**
 * libiodefv2_io_write:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @buf: Pointer to the buffer to write data from.
 * @count: Number of bytes to write.
 *
 * libiodefv2_io_write() writes up to @count bytes to the file descriptor
 * identified by @pio from the buffer starting at @buf. POSIX requires
 * that a read() which can be proved to occur after a write() has returned
 * returns the new data. Note that not all file systems are POSIX conforming.
 *
 * The case where the write() function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes written are returned (zero
 * indicates nothing was written). On error, -1 is returned, and errno
 * is set appropriately. If @count is zero and the file descriptor refers
 * to a regular file, 0 will be returned without causing any other effect.
 * For a special file, the results are not portable.
 */
ssize_t libiodefv2_io_write(libiodefv2_io_t *pio, const void *buf, size_t count)
{
        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(pio->write, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return pio->write(pio, buf, count);
}



/**
 * libiodefv2_io_write_delimited:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @buf: Pointer to the buffer to write data from.
 * @count: Number of bytes to write.
 *
 * libiodefv2_io_write_delimited() writes up to @count bytes to the file descriptor
 * identified by @pio from the buffer starting at @buf. POSIX requires
 * that a read() which can be proved to occur after a write() has returned
 * returns the new data. Note that not all file systems are POSIX conforming.
 *
 * libiodefv2_io_write_delimited() also write the len of the data to be sent.
 * which allow libiodefv2_io_read_delimited() to safely know if it got all the
 * data a given write contain.
 *
 * The case where the write() function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes written are returned (zero
 * indicates nothing was written). On error, -1 is returned, and errno
 * is set appropriately.
 */
ssize_t libiodefv2_io_write_delimited(libiodefv2_io_t *pio, const void *buf, uint16_t count)
{
        int ret;
        uint16_t nlen;

        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(buf, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        nlen = htons(count);

        ret = libiodefv2_io_write(pio, &nlen, sizeof(nlen));
        if ( ret <= 0 )
                return ret;

        ret = libiodefv2_io_write(pio, buf, count);
        if ( ret <= 0 )
                return ret;

        return count;
}




/**
 * libiodefv2_io_close:
 * @pio: Pointer to a #libiodefv2_io_t object.
 *
 * libiodefv2_io_close() closes the file descriptor indentified by @pio,
 *
 * The case where the close() function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * However, and especially when the underlaying layer is TLS, libiodefv2_io_close()
 * might return error. If this happen, you should continue calling the function
 * until it return zero.
 *
 * Returns: zero on success, or -1 if an error occurred.
 */
int libiodefv2_io_close(libiodefv2_io_t *pio)
{
        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(pio->close, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        return pio->close(pio);
}




/**
 * libiodefv2_io_new:
 * @ret: Pointer where to store the created #libiodefv2_io_t object.
 *
 * Create a new libiodefv2 IO object.
 *
 * Returns: 0 on success, or a negative value if an error occur.
 */
int libiodefv2_io_new(libiodefv2_io_t **ret)
{
        *ret = calloc(1, sizeof(**ret));
        if ( ! *ret )
                return libiodefv2_error_from_errno(errno);

        return 0;
}



/**
 * libiodefv2_io_set_file_io:
 * @pio: A pointer on the #libiodefv2_io_t object.
 * @fd: File descriptor identifying a file.
 *
 * Setup the @pio object to work with file I/O function.
 * The @pio object is then associated with @fd.
 */
void libiodefv2_io_set_file_io(libiodefv2_io_t *pio, FILE *fdptr)
{
        libiodefv2_return_if_fail(pio);
        libiodefv2_return_if_fail(fdptr);

        pio->fd = fileno(fdptr);
        pio->fd_ptr = fdptr;
        pio->read = file_read;
        pio->write = file_write;
        pio->close = file_close;
        pio->pending = file_pending;
}





/**
 * libiodefv2_io_set_sys_io:
 * @pio: A pointer on the #libiodefv2_io_t object.
 * @fd: A file descriptor.
 *
 * Setup the @pio object to work with system based I/O function.
 * The @pio object is then associated with @fd.
 */
void libiodefv2_io_set_sys_io(libiodefv2_io_t *pio, int fd)
{
        libiodefv2_return_if_fail(pio);

        pio->fd = fd;
        pio->fd_ptr = NULL;
        pio->read = sys_read;
        pio->write = sys_write;
        pio->close = sys_close;
        pio->pending = sys_pending;
}



int libiodefv2_io_set_buffer_io(libiodefv2_io_t *pio)
{
        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        pio->fd_ptr = NULL;
        pio->size = pio->rindex = 0;

        pio->read = buffer_read;
        pio->write = buffer_write;
        pio->close = buffer_close;
        pio->pending = buffer_pending;

        return 0;
}



/**
 * libiodefv2_io_get_fd:
 * @pio: A pointer on a #libiodefv2_io_t object.
 *
 * Returns: The FD associated with this object.
 */
int libiodefv2_io_get_fd(libiodefv2_io_t *pio)
{
        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return pio->fd;
}



/**
 * libiodefv2_io_get_fdptr:
 * @pio: A pointer on a #libiodefv2_io_t object.
 *
 * Returns: Pointer associated with this object (file, tls, buffer, or NULL).
 */
void *libiodefv2_io_get_fdptr(libiodefv2_io_t *pio)
{
        libiodefv2_return_val_if_fail(pio, NULL);
        return pio->fd_ptr;
}



/**
 * libiodefv2_io_destroy:
 * @pio: Pointer to a #libiodefv2_io_t object.
 *
 * Destroy the @pio object.
 */
void libiodefv2_io_destroy(libiodefv2_io_t *pio)
{
        libiodefv2_return_if_fail(pio);
        free(pio);
}




/**
 * libiodefv2_io_pending:
 * @pio: Pointer to a #libiodefv2_io_t object.
 *
 * libiodefv2_io_pending return the number of bytes waiting to
 * be read on an TLS or socket fd.
 *
 * Returns: Number of byte waiting to be read on @pio, or -1
 * if @pio is not of type TLS or socket.
 */
ssize_t libiodefv2_io_pending(libiodefv2_io_t *pio)
{
        libiodefv2_return_val_if_fail(pio, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return pio->pending(pio);
}




/**
 * libiodefv2_io_is_error_fatal:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @error: Error returned by one of the #libiodefv2_io_t function.
 *
 * Check whether the returned error is fatal, or not.
 *
 * Returns: TRUE if error is fatal, FALSE if it is not.
 */
libiodefv2_bool_t libiodefv2_io_is_error_fatal(libiodefv2_io_t *pio, int error)
{
        libiodefv2_error_code_t code;

        libiodefv2_return_val_if_fail(pio, FALSE);

        if ( ! error )
                return FALSE;

        code = libiodefv2_error_get_code(error);
        if ( code == LIBIODEFV2_ERROR_EAGAIN || code == LIBIODEFV2_ERROR_EINTR || code == LIBIODEFV2_ERROR_TLS_WARNING_ALERT )
                return FALSE;

        return TRUE;
}



/**
 * libiodefv2_io_set_write_callback:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @write: Callback function to be called on libiodefv2_io_write().
 *
 * Set an user defined write callback function to be called on
 * libiodefv2_io_write().
 */
void libiodefv2_io_set_write_callback(libiodefv2_io_t *pio, ssize_t (*write)(libiodefv2_io_t *io, const void *buf, size_t count))
{
        pio->write = write;
}


/**
 * libiodefv2_io_set_read_callback:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @read: Callback function to be called on libiodefv2_io_read().
 *
 * Set an user defined read callback function to be called on
 * libiodefv2_io_read().
 */
void libiodefv2_io_set_read_callback(libiodefv2_io_t *pio, ssize_t (*read)(libiodefv2_io_t *io, void *buf, size_t count))
{
        pio->read = read;
}



/**
 * libiodefv2_io_set_pending_callback:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @pending: Callback function to be called on libiodefv2_io_pending().
 *
 * Set an user defined read callback function to be called on
 * libiodefv2_io_pending().
 */
void libiodefv2_io_set_pending_callback(libiodefv2_io_t *pio, ssize_t (*pending)(libiodefv2_io_t *io))
{
        pio->pending = pending;
}



/**
 * libiodefv2_io_set_fdptr:
 * @pio: Pointer to a #libiodefv2_io_t object.
 * @ptr: Pointer to user defined data.
 *
 * Set an user defined pointer that might be retrieved using
 * libiodefv2_io_get_fdptr().
 */
void libiodefv2_io_set_fdptr(libiodefv2_io_t *pio, void *ptr)
{
        pio->fd_ptr = ptr;
}
