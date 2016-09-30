/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann@libiodefv2-ids.com>
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

#include "iodefv2.hxx"
#include "iodefv2-path.hxx"
#include "iodefv2-class.hxx"
#include "libiodefv2-error.hxx"

#include "iodefv2-object-prv.h"

using namespace Iodef;


IODEFV2Path::IODEFV2Path(const char *buffer)
{
        int ret;

        ret = iodefv2_path_new_fast(&_path, buffer);
        if ( ret < 0 )
                throw LibIodefError(ret);
}



IODEFV2Path::IODEFV2Path(IODEFV2 &iodefv2, const char *buffer)
{
        int ret;
        iodefv2_object_t *obj = (iodefv2_object_t *) iodefv2;

        ret = iodefv2_path_new_from_root_fast(&_path, obj->_iodefv2_object_id, buffer);
        if ( ret < 0 )
                throw LibIodefError(ret);
}



IODEFV2Path::IODEFV2Path(iodefv2_path_t *path)
{
        _path = path;
}


IODEFV2Path::IODEFV2Path(const IODEFV2Path &path)
{
        _path = (path._path) ? iodefv2_path_ref(path._path) : NULL;
}


IODEFV2Path::~IODEFV2Path()
{
        iodefv2_path_destroy(_path);
}


IODEFV2Value IODEFV2Path::get(const IODEFV2 &message) const
{
        int ret;
        iodefv2_value_t *value;

        ret = iodefv2_path_get(_path, message, &value);
        if ( ret < 0 )
                throw LibIodefError(ret);

        else if ( ret == 0 )
                return IODEFV2Value((iodefv2_value_t *) NULL);

        return IODEFV2Value(value);
}



Iodef::IODEFV2Value::IODEFV2ValueTypeEnum IODEFV2Path::getValueType(int depth) const
{
        return (Iodef::IODEFV2Value::IODEFV2ValueTypeEnum) iodefv2_path_get_value_type(_path, depth);
}


int IODEFV2Path::checkOperator(iodefv2_criterion_operator_t op) const
{
        return iodefv2_path_check_operator(_path, op);
}



iodefv2_criterion_operator_t IODEFV2Path::getApplicableOperators() const
{
        int ret;
        iodefv2_criterion_operator_t res;

        ret = iodefv2_path_get_applicable_operators(_path, &res);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return res;
}



void IODEFV2Path::set(IODEFV2 &message, const std::vector<IODEFV2> &value) const
{
        int ret;
        IODEFV2Value v = value;

        ret = iodefv2_path_set(_path, message, v);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, IODEFV2 *value) const
{
        int ret;

        if ( ! value )
                ret = iodefv2_path_set(_path, message, NULL);
        else
                ret = iodefv2_path_set(_path, message, IODEFV2Value(value));

        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, IODEFV2Value *value) const
{
        int ret;

        if ( ! value )
                ret = iodefv2_path_set(_path, message, NULL);
        else
                ret = iodefv2_path_set(_path, message, *value);

        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, const std::vector<IODEFV2Value> &value) const
{
        int ret;
        IODEFV2Value v = value;

        ret = iodefv2_path_set(_path, message, v);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, IODEFV2Value &value) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, value);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, const std::string &value) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, IODEFV2Value(value));
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, const char *value) const
{
        int ret;

        if ( value )
                ret = iodefv2_path_set(_path, message, IODEFV2Value(value));
        else
                ret = iodefv2_path_set(_path, message, (iodefv2_value_t *) NULL);

        if ( ret < 0 )
                throw LibIodefError(ret);
}



void IODEFV2Path::set(IODEFV2 &message, int32_t value) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, IODEFV2Value(value));
        if ( ret < 0 )
                throw LibIodefError(ret);
}



void IODEFV2Path::set(IODEFV2 &message, int64_t value) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, IODEFV2Value(value));
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, uint64_t value) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, IODEFV2Value(value));
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, float value) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, IODEFV2Value(value));
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, double value) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, IODEFV2Value(value));
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Path::set(IODEFV2 &message, IODEFV2Time &time) const
{
        int ret;

        ret = iodefv2_path_set(_path, message, IODEFV2Value(time));
        if ( ret < 0 )
                throw LibIodefError(ret);
}



IODEFV2Class IODEFV2Path::getClass(int depth) const
{
        return IODEFV2Class(iodefv2_path_get_class(_path, depth));
}


int IODEFV2Path::setIndex(unsigned int index, int depth)
{
        if ( depth < 0 )
                depth = getDepth();

        return iodefv2_path_set_index(_path, depth, index);
}


int IODEFV2Path::undefineIndex(int depth)
{
        if ( depth < 0 )
                depth = getDepth();

        return iodefv2_path_undefine_index(_path, depth);
}


int IODEFV2Path::getIndex(int depth) const
{
        if ( depth < 0 )
                depth = getDepth();

        return iodefv2_path_get_index(_path, depth);
}


int IODEFV2Path::makeChild(const char *child_name, unsigned int index=0)
{
        return iodefv2_path_make_child(_path, child_name, index);
}



int IODEFV2Path::makeParent()
{
        return iodefv2_path_make_parent(_path);
}


int IODEFV2Path::compare(IODEFV2Path *path, int depth) const
{
        int ret;

        if ( depth < 0 )
                ret = iodefv2_path_compare(_path, path->_path);
        else
                ret = iodefv2_path_ncompare(_path, path->_path, depth);

        return ret;
}


IODEFV2Path IODEFV2Path::clone() const
{
        int ret;
        iodefv2_path_t *cpath;

        ret = iodefv2_path_clone(_path, &cpath);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return IODEFV2Path(cpath);
}


const char *IODEFV2Path::getName(int depth) const
{
        return iodefv2_path_get_name(_path, depth);
}


bool IODEFV2Path::isAmbiguous() const
{
        return iodefv2_path_is_ambiguous(_path);
}


int IODEFV2Path::hasLists() const
{
        return iodefv2_path_has_lists(_path);
}


bool IODEFV2Path::isList(int depth) const
{
        return iodefv2_path_is_list(_path, depth);
}


unsigned int IODEFV2Path::getDepth() const
{
        return iodefv2_path_get_depth(_path);
}


IODEFV2Path &IODEFV2Path::operator=(const IODEFV2Path &path)
{
        if ( this != &path && _path != path._path ) {
                if ( _path )
                        iodefv2_path_destroy(_path);

                _path = (path._path) ? iodefv2_path_ref(path._path) : NULL;
        }

        return *this;
}


IODEFV2Path::operator iodefv2_path_t *() const
{
        return _path;
}

