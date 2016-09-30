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

#include <string.h>
#include <sstream>

#include "libiodefv2-error.hxx"
#include "iodefv2.hxx"
#include "iodefv2-value.hxx"
#include "iodefv2-criteria.hxx"

using namespace Iodef;


IODEFV2Value::~IODEFV2Value()
{
        if ( _value )
                iodefv2_value_destroy(_value);
}


IODEFV2Value::IODEFV2ValueTypeEnum IODEFV2Value::getType() const
{
        return (IODEFV2Value::IODEFV2ValueTypeEnum) iodefv2_value_get_type(_value);
}


bool IODEFV2Value::isNull() const
{
        return (! _value) ? TRUE : FALSE;
}


IODEFV2Value::IODEFV2Value()
{
        _value = NULL;
}


IODEFV2Value::IODEFV2Value(const IODEFV2Value &value)
{
        iodefv2_value_t *v = NULL;

        if ( value._value )
                v = iodefv2_value_ref(value._value);

        _value = v;
}


void IODEFV2Value::_InitFromString(const char *value, size_t len)
{
        int ret;
        libiodefv2_string_t *str;

        ret = libiodefv2_string_new_dup_fast(&str, value, len);
        if ( ret < 0 )
                throw LibIodefError(ret);

        ret = iodefv2_value_new_string(&_value, str);
        if ( ret < 0 ) {
                libiodefv2_string_destroy(str);
                throw LibIodefError(ret);
        }
}


IODEFV2Value::IODEFV2Value(const char *value)
{
        _InitFromString(value, strlen(value));
}


IODEFV2Value::IODEFV2Value(const std::string &value)
{
        _InitFromString(value.c_str(), value.size());
}


IODEFV2Value::IODEFV2Value(int32_t value)
{
        int ret = iodefv2_value_new_int32(&_value, value);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Value::IODEFV2Value(int64_t value)
{
        int ret = iodefv2_value_new_int64(&_value, value);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Value::IODEFV2Value(uint64_t value)
{
        int ret = iodefv2_value_new_uint64(&_value, value);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Value::IODEFV2Value(float value)
{
        int ret = iodefv2_value_new_float(&_value, value);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Value::IODEFV2Value(double value)
{
        int ret = iodefv2_value_new_double(&_value, value);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Value::IODEFV2Value(IODEFV2Time &time)
{
        int ret = iodefv2_value_new_time(&_value, iodefv2_time_ref(time));
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Value::IODEFV2Value(IODEFV2 *iodefv2)
{
        int ret;
        iodefv2_value_t *v = NULL;

        if (  iodefv2 ) {
                ret = iodefv2_value_new_class(&v, iodefv2->getId(), iodefv2_object_ref((iodefv2_object_t *) *iodefv2));
                if ( ret < 0 )
                       throw LibIodefError(ret);
        }

        _value = v;
}


IODEFV2Value::IODEFV2Value(const std::vector<IODEFV2> &value)
{
        int ret;
        iodefv2_value_t *vitem;
        std::vector<Iodef::IODEFV2>::const_iterator i;

        ret = iodefv2_value_new_list(&_value);
        if ( ret < 0 )
                throw LibIodefError(ret);

        for ( i = value.begin(); i != value.end(); i++ ) {
                ret = iodefv2_value_new_class(&vitem, i->getId(), iodefv2_object_ref((iodefv2_object_t *) *i));
                if ( ret < 0 )
                        throw LibIodefError(ret);

                iodefv2_value_list_add(_value, vitem);
        }
}


IODEFV2Value::IODEFV2Value(const std::vector<IODEFV2Value> &value)
{
        int ret;
        std::vector<Iodef::IODEFV2Value>::const_iterator i;

        ret = iodefv2_value_new_list(&_value);
        if ( ret < 0 )
                throw LibIodefError(ret);

        for ( i = value.begin(); i != value.end(); i++ )
                iodefv2_value_list_add(_value, iodefv2_value_ref(*i));
}

IODEFV2Value::IODEFV2Value(iodefv2_value_t *value)
{
        _value = value;
}


int IODEFV2Value::match(const IODEFV2Value &value, int op) const
{
        int ret;

        ret = iodefv2_value_match(this->_value, value._value, (iodefv2_criterion_operator_t) op);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return ret;
}


IODEFV2Value IODEFV2Value::clone() const
{
        int ret;
        iodefv2_value_t *clone;

        ret = iodefv2_value_clone(_value, &clone);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return IODEFV2Value(clone);
}


const std::string IODEFV2Value::toString() const
{
        int ret;
        std::string s;
        libiodefv2_string_t *str;

        ret = libiodefv2_string_new(&str);
        if ( ret < 0 )
                throw LibIodefError(ret);

        ret = iodefv2_value_to_string(_value, str);
        if ( ret < 0 ) {
                libiodefv2_string_destroy(str);
                throw LibIodefError(ret);
        }

        s = libiodefv2_string_get_string(str);
        libiodefv2_string_destroy(str);

        return s;
}


static int iterate_cb(iodefv2_value_t *value, void *extra)
{
        std::vector<IODEFV2Value> *vlist = (std::vector<IODEFV2Value> *) extra;

        if ( value )
                value = iodefv2_value_ref(value);

        vlist->push_back(IODEFV2Value(value));

        return 0;
}


IODEFV2Value::operator std::vector<IODEFV2Value> () const
{
        std::vector<IODEFV2Value> vlist;

        if ( ! _value )
                return vlist;

        if ( getType() != TYPE_LIST ) {
                std::stringstream s;
                s << "Left value doesn't fit '" << iodefv2_value_type_to_string((iodefv2_value_type_id_t) getType()) << "' requirement";
                throw LibIodefError(s.str());
        }

        iodefv2_value_iterate(_value, iterate_cb, &vlist);

        return vlist;
}


IODEFV2Value::operator IODEFV2Time () const
{
        libiodefv2_except_if_fail(_value);

        if ( getType() != TYPE_TIME ) {
                std::stringstream s;
                s << "Left value doesn't fit '" << iodefv2_value_type_to_string((iodefv2_value_type_id_t) getType()) << "' requirement";
                throw LibIodefError(s.str());
        }

        return IODEFV2Time(iodefv2_time_ref(iodefv2_value_get_time(_value)));
}



IODEFV2Value::operator int32_t () const
{
        IODEFV2ValueTypeEnum vtype;

        libiodefv2_except_if_fail(_value);
        vtype = getType();

        if ( vtype == TYPE_INT8 )
                return iodefv2_value_get_int8(_value);

        else if ( vtype == TYPE_UINT8 )
                return iodefv2_value_get_uint8(_value);

        else if ( vtype == TYPE_INT16 )
                return iodefv2_value_get_int16(_value);

        else if ( vtype == TYPE_UINT16 )
                return iodefv2_value_get_uint16(_value);

        else if ( vtype == TYPE_INT32 )
                return iodefv2_value_get_int32(_value);

        else if ( vtype == TYPE_ENUM )
                return iodefv2_value_get_enum(_value);

        std::stringstream s;
        s << "Left value doesn't fit '" << iodefv2_value_type_to_string((iodefv2_value_type_id_t) vtype) << "' requirement";
        throw LibIodefError(s.str());
}


IODEFV2Value::operator uint32_t () const
{
        libiodefv2_except_if_fail(_value);

        if ( getType() == TYPE_UINT32 )
                return iodefv2_value_get_uint32(_value);
        else
                return (int32_t) *this;
}


IODEFV2Value::operator int64_t () const
{
        libiodefv2_except_if_fail(_value);

        if ( getType() == TYPE_INT64 )
                return iodefv2_value_get_int64(_value);
        else
                return (uint32_t) *this;
}


IODEFV2Value::operator uint64_t () const
{
        libiodefv2_except_if_fail(_value);

        if ( getType() == TYPE_UINT64 )
                return iodefv2_value_get_uint64(_value);
        else
                return (int64_t) *this;
}


IODEFV2Value::operator float () const
{
        IODEFV2ValueTypeEnum vtype;

        libiodefv2_except_if_fail(_value);
        vtype = getType();

        if ( vtype == TYPE_FLOAT )
                return iodefv2_value_get_float(_value);

        else if ( vtype == TYPE_DATA ) {
                iodefv2_data_t *d = iodefv2_value_get_data(_value);

                if ( iodefv2_data_get_type(d) == IODEFV2_DATA_TYPE_FLOAT )
                        return iodefv2_data_get_float(d);
        }

        std::stringstream s;
        s << "Left value doesn't fit '" << iodefv2_value_type_to_string((iodefv2_value_type_id_t) vtype) << "' requirement";
        throw LibIodefError(s.str());
}


IODEFV2Value::operator double () const
{
        libiodefv2_except_if_fail(_value);

        if ( getType() == TYPE_DOUBLE )
                return iodefv2_value_get_double(_value);
        else
                return (float) *this;
}


std::string IODEFV2Value::convert_string() const
{
        std::stringstream s;
        libiodefv2_except_if_fail(_value);

        if ( getType() == TYPE_STRING )
                return libiodefv2_string_get_string(iodefv2_value_get_string(_value));

        else if ( getType() == TYPE_TIME )
                return IODEFV2Time(iodefv2_time_ref(iodefv2_value_get_time(_value)));

        else if ( getType() == TYPE_ENUM )
                return iodefv2_class_enum_to_string(iodefv2_value_get_class(_value), iodefv2_value_get_enum(_value));

        else if ( getType() == TYPE_DATA ) {
                iodefv2_data_t *d = iodefv2_value_get_data(_value);
                iodefv2_data_type_t t = iodefv2_data_get_type(d);

                if ( t == IODEFV2_DATA_TYPE_CHAR_STRING )
                        return (const char *) iodefv2_data_get_char_string(d);

                else if ( t == IODEFV2_DATA_TYPE_CHAR ) {
                        s << iodefv2_data_get_char(d);
                        return s.str();
                }

                else if ( t == IODEFV2_DATA_TYPE_FLOAT ) {
                        s << iodefv2_data_get_float(d);
                        return s.str();
                }

                else if ( t == IODEFV2_DATA_TYPE_INT ) {
                        s << iodefv2_data_get_int(d);
                        return s.str();
                }

                else {
                        s << "Left value doesn't fit 'data' type '" << t << "' requirement";
                        throw LibIodefError(s.str());
                }
        }

        s << "Left value doesn't fit '" << iodefv2_value_type_to_string((iodefv2_value_type_id_t) getType()) << "' requirement";
        throw LibIodefError(s.str());
}


IODEFV2Value &IODEFV2Value::operator=(const IODEFV2Value &p)
{
        if ( this != &p && _value != p._value ) {
                if ( _value )
                        iodefv2_value_destroy(_value);

                _value = (p._value) ? iodefv2_value_ref(p._value) : NULL;
        }

        return *this;
}


IODEFV2Value::operator const char*() const
{
        static std::string t;
        t = this->convert_string();
        return t.c_str();
}


IODEFV2Value::operator iodefv2_value_t *() const
{
        return _value;
}


bool IODEFV2Value::operator <= (const IODEFV2Value &value) const
{
        return this->match(value, IODEFV2Criterion::OPERATOR_LESSER|IODEFV2Criterion::OPERATOR_EQUAL);
}


bool IODEFV2Value::operator >= (const IODEFV2Value &value) const
{
        return this->match(value, IODEFV2Criterion::OPERATOR_GREATER|IODEFV2Criterion::OPERATOR_EQUAL);
}


bool IODEFV2Value::operator < (const IODEFV2Value &value) const
{
        return this->match(value, IODEFV2Criterion::OPERATOR_LESSER);
}


bool IODEFV2Value::operator > (const IODEFV2Value &value) const
{
        return this->match(value, IODEFV2Criterion::OPERATOR_GREATER);
}


bool IODEFV2Value::operator == (const IODEFV2Value &value) const
{
        return this->match(value, IODEFV2Criterion::OPERATOR_EQUAL);
}


bool IODEFV2Value::operator == (const std::vector<IODEFV2Value> &vlist) const
{
        return this->match(vlist, IODEFV2Criterion::OPERATOR_EQUAL);
}


bool IODEFV2Value::operator != (const IODEFV2Value &value) const
{
        return this->match(value, IODEFV2Criterion::OPERATOR_NOT|IODEFV2Criterion::OPERATOR_EQUAL);
}
