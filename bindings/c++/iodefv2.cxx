/*****
*
* Copyright (C) 2009-2016 CS-SI. All Rights Reserved.
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

#include <iostream>

#include <string.h>
#include <iodefv2.h>
#include <iodefv2-path.h>
#include <iodefv2-document-print.h>
#include <iodefv2-object-prv.h>

#include "libiodefv2-error.hxx"
#include "iodefv2-path.hxx"
#include "iodefv2.hxx"

using namespace Iodef;


IODEFV2::IODEFV2() : _object(NULL)
{
        int ret;

        ret = iodefv2_document_new((iodefv2_document_t **) &_object);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2::IODEFV2(const char *json) : _object(NULL)
{
        int ret;

        ret = iodefv2_object_new_from_json(&_object, json);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2::IODEFV2(const IODEFV2 &iodefv2)
{
        _object = (iodefv2._object) ? iodefv2_object_ref(iodefv2._object) : NULL;
}


IODEFV2::IODEFV2(iodefv2_object_t *object)
{
        _object = object;
}


IODEFV2::~IODEFV2()
{
        if ( _object )
                iodefv2_object_destroy(_object);
}

void IODEFV2::set(const char *path, const std::vector<IODEFV2> &value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, IODEFV2 *value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, const std::vector<IODEFV2Value> &value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, IODEFV2Value *value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, IODEFV2Value &value)
{
        IODEFV2Path(*this, path).set(*this, &value);
}


void IODEFV2::set(const char *path, const std::string &value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, const char *value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, int32_t value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, int64_t value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, uint64_t value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, float value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, double value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


void IODEFV2::set(const char *path, IODEFV2Time &value)
{
        IODEFV2Path(*this, path).set(*this, value);
}


IODEFV2Value IODEFV2::get(const char *path)
{
        return IODEFV2Path(*this, path).get(*this);
}


int IODEFV2::operator == (const IODEFV2 *iodefv2)
{
        if ( ! iodefv2 )
                return 0;

        if ( _object == iodefv2->_object )
                return 1;

        return iodefv2_object_compare(_object, iodefv2->_object) == 0 ? 1 : 0;
}


IODEFV2 IODEFV2::clone()
{
        int ret;
        iodefv2_object_t *clone;

        ret = iodefv2_object_clone(_object, &clone);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return IODEFV2(clone);
}


iodefv2_class_id_t IODEFV2::getId() const
{
        return _object->_iodefv2_object_id;
}


const std::string IODEFV2::toString() const
{
        int ret;
        std::string str;
        libiodefv2_io_t *fd;

        ret = libiodefv2_io_new(&fd);
        if ( ret < 0 )
                throw LibIodefError(ret);

        libiodefv2_io_set_buffer_io(fd);
        iodefv2_object_print(_object, fd);

        str.assign((const char *) libiodefv2_io_get_fdptr(fd), libiodefv2_io_pending(fd));

        libiodefv2_io_close(fd);
        libiodefv2_io_destroy(fd);

        return str;
}


const std::string IODEFV2::toJSON() const
{
        int ret;
        std::string str;
        libiodefv2_io_t *fd;

        ret = libiodefv2_io_new(&fd);
        if ( ret < 0 )
                throw LibIodefError(ret);

        libiodefv2_io_set_buffer_io(fd);
        iodefv2_object_print_json(_object, fd);

        str.assign((const char *) libiodefv2_io_get_fdptr(fd), libiodefv2_io_pending(fd));

        libiodefv2_io_close(fd);
        libiodefv2_io_destroy(fd);

        return str;
}

const std::string IODEFV2::toBinary() const
{
        int ret;
        std::string str;
        libiodefv2_io_t *fd;

        ret = libiodefv2_io_new(&fd);
        if ( ret < 0 )
                throw LibIodefError(ret);

        libiodefv2_io_set_buffer_io(fd);
        iodefv2_object_print_binary(_object, fd);

        str.assign((const char *) libiodefv2_io_get_fdptr(fd), libiodefv2_io_pending(fd));

        libiodefv2_io_close(fd);
        libiodefv2_io_destroy(fd);

        return str;
}


IODEFV2::operator const std::string() const
{
        return toString();
}


IODEFV2::operator iodefv2_object_t *() const
{
        return (iodefv2_object_t *) _object;
}


IODEFV2 &IODEFV2::operator = (const IODEFV2 &iodefv2)
{
        if ( this != &iodefv2 && _object != iodefv2._object ) {
                if ( _object )
                        iodefv2_object_destroy(_object);

                _object = (iodefv2._object) ? iodefv2_object_ref(iodefv2._object) : NULL;
        }

        return *this;
}
