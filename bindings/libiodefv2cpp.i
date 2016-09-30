/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@libiodefv2-ids.com>
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

#if defined(SWIGPYTHON) || defined(SWIGLUA)
%module iodefv2
#else
%module Iodef
#endif

%warnfilter(509);
%feature("nothread", "1");

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%{
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <list>
#include <sstream>

#include "libiodefv2.hxx"
#include "libiodefv2-error.hxx"
#include "iodefv2-criteria.hxx"
#include "iodefv2-value.hxx"
#include "iodefv2-path.hxx"
#include "iodefv2-time.hxx"
#include "iodefv2.hxx"

using namespace Iodef;
%}


typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

%ignore libiodefv2_error_t;
typedef signed int libiodefv2_error_t;

%ignore iodefv2_class_id_t;
typedef int iodefv2_class_id_t;

typedef long long time_t;


%exception {
        try {
                $action
        } catch(Iodef::LibIodefError &e) {
                SWIG_exception(SWIG_RuntimeError, e.what());
                SWIG_fail;
        }
}


#ifdef SWIGPERL
%include perl/libiodefv2cpp-perl.i
#endif

#ifdef SWIGPYTHON
%include python/libiodefv2cpp-python.i
#endif

#ifdef SWIGRUBY
%include ruby/libiodefv2cpp-ruby.i
#endif

#ifdef SWIGLUA
%include lua/libiodefv2cpp-lua.i
#endif


%ignore operator <<(std::ostream &os, const Iodef::IODEFV2 &iodefv2);
%ignore operator >>(std::istream &is, const Iodef::IODEFV2 &iodefv2);


%template() std::vector<std::string>;
%template() std::vector<Iodef::IODEFV2>;
%template() std::vector<Iodef::IODEFV2Value>;
%template() std::vector<Iodef::Connection>;

#ifdef SWIG_COMPILE_LIBIODEFV2
%extend Iodef::IODEFV2 {
        Iodef::IODEFV2Value get(const char *path) {
                Iodef::IODEFV2Value value;
                Iodef::IODEFV2Path ipath = Iodef::IODEFV2Path(*self, path);

                value = ipath.get(*self);
                if ( value.isNull() && ipath.isAmbiguous() ) {
                        std::vector<Iodef::IODEFV2Value> v;
                        return Iodef::IODEFV2Value(v);
                }

                return value;
        }
}

%extend Iodef::IODEFV2Path {
        Iodef::IODEFV2Value get(Iodef::IODEFV2 &message) {
                Iodef::IODEFV2Value value;

                value = self->get(message);
                if ( value.isNull() && self->isAmbiguous() ) {
                        std::vector<Iodef::IODEFV2Value> v;
                        return Iodef::IODEFV2Value(v);
                }

                return value;
        }
}
#endif

%ignore Iodef::IODEFV2::get;
%ignore Iodef::IODEFV2Path::get;


%fragment("SWIG_FromBytePtrAndSize", "header", fragment="SWIG_FromCharPtrAndSize") %{
#ifndef SWIG_FromBytePtrAndSize
# define SWIG_FromBytePtrAndSize(arg, len) SWIG_FromCharPtrAndSize(arg, len)
#endif
%}


%fragment("IODEFV2Value_to_SWIG", "header", fragment="SWIG_From_double",
                                          fragment="SWIG_From_float",
                                          fragment="SWIG_From_int", fragment="SWIG_From_unsigned_SS_int",
                                          fragment="SWIG_From_long_SS_long", fragment="SWIG_From_unsigned_SS_long_SS_long",
                                          fragment="SWIG_FromCharPtr", fragment="SWIG_FromCharPtrAndSize", fragment="SWIG_FromBytePtrAndSize",
                                          fragment="IODEFV2ValueList_to_SWIG") {

int IODEFV2Value_to_SWIG(TARGET_LANGUAGE_SELF self, const Iodef::IODEFV2Value &result, void *extra, TARGET_LANGUAGE_OUTPUT_TYPE ret)
{
        iodefv2_value_t *value = result;
        Iodef::IODEFV2Value::IODEFV2ValueTypeEnum type = result.getType();

        if ( type == Iodef::IODEFV2Value::TYPE_STRING ) {
                libiodefv2_string_t *str = iodefv2_value_get_string(value);
                *ret = SWIG_FromCharPtrAndSize(libiodefv2_string_get_string(str), libiodefv2_string_get_len(str));
        }

        else if ( type == Iodef::IODEFV2Value::TYPE_INT8 )
                *ret = SWIG_From_int(iodefv2_value_get_int8(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_UINT8 )
                *ret = SWIG_From_unsigned_SS_int(iodefv2_value_get_uint8(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_INT16 )
                *ret = SWIG_From_int(iodefv2_value_get_int16(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_UINT16 )
                *ret = SWIG_From_unsigned_SS_int(iodefv2_value_get_uint16(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_INT32 )
                *ret = SWIG_From_int(iodefv2_value_get_int32(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_UINT32 )
                *ret = SWIG_From_unsigned_SS_int(iodefv2_value_get_uint32(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_INT64 )
                *ret = SWIG_From_long_SS_long(iodefv2_value_get_int64(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_UINT64 )
                *ret = SWIG_From_unsigned_SS_long_SS_long(iodefv2_value_get_uint64(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_FLOAT )
                *ret = SWIG_From_float(iodefv2_value_get_float(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_DOUBLE )
                *ret = SWIG_From_double(iodefv2_value_get_double(value));

        else if ( type == Iodef::IODEFV2Value::TYPE_ENUM ) {
                const char *s = iodefv2_class_enum_to_string(iodefv2_value_get_class(value), iodefv2_value_get_enum(value));
                *ret = SWIG_FromCharPtr(s);
        }

        else if ( type == Iodef::IODEFV2Value::TYPE_TIME ) {
                Iodef::IODEFV2Time t = result;
                *ret = SWIG_NewPointerObj(new Iodef::IODEFV2Time(t), $descriptor(Iodef::IODEFV2Time *), 1);
        }

        else if ( type == Iodef::IODEFV2Value::TYPE_LIST )
                *ret = IODEFV2ValueList_to_SWIG(self, result, extra);

        else if ( type == Iodef::IODEFV2Value::TYPE_DATA ) {
                iodefv2_data_t *d = iodefv2_value_get_data(value);
                iodefv2_data_type_t t = iodefv2_data_get_type(d);

                if ( t == IODEFV2_DATA_TYPE_BYTE || t == IODEFV2_DATA_TYPE_BYTE_STRING )
                        *ret = SWIG_FromBytePtrAndSize((const char *)iodefv2_data_get_data(d), iodefv2_data_get_len(d));

                else if ( t == IODEFV2_DATA_TYPE_CHAR )
                        *ret = SWIG_FromCharPtrAndSize((const char *)iodefv2_data_get_data(d), iodefv2_data_get_len(d));

                else if ( t == IODEFV2_DATA_TYPE_CHAR_STRING )
                        *ret = SWIG_FromCharPtrAndSize((const char *)iodefv2_data_get_data(d), iodefv2_data_get_len(d) - 1);

                else if ( t == IODEFV2_DATA_TYPE_FLOAT )
                        *ret = SWIG_From_float(iodefv2_data_get_float(d));

                else if ( t == IODEFV2_DATA_TYPE_UINT32 || IODEFV2_DATA_TYPE_INT )
                        *ret = SWIG_From_unsigned_SS_long_SS_long(iodefv2_data_get_int(d));
        }

        else if ( type == Iodef::IODEFV2Value::TYPE_CLASS ) {
                iodefv2_object_t *obj = (iodefv2_object_t *) iodefv2_value_get_object(value);
                *ret = SWIG_NewPointerObj(new Iodef::IODEFV2(iodefv2_object_ref(obj)), $descriptor(Iodef::IODEFV2 *), 1);
        }

        else return -1;

        return 1;
}
}

%ignore Iodef::IODEFV2Value::operator const char*() const;
%ignore Iodef::IODEFV2Value::operator std::vector<IODEFV2Value>() const;
%ignore Iodef::IODEFV2Value::operator Iodef::IODEFV2Time() const;
%ignore Iodef::IODEFV2Value::operator int8_t() const;
%ignore Iodef::IODEFV2Value::operator uint8_t() const;
%ignore Iodef::IODEFV2Value::operator int16_t() const;
%ignore Iodef::IODEFV2Value::operator uint16_t() const;
%ignore Iodef::IODEFV2Value::operator int32_t() const;
%ignore Iodef::IODEFV2Value::operator uint32_t() const;
%ignore Iodef::IODEFV2Value::operator int64_t() const;
%ignore Iodef::IODEFV2Value::operator uint64_t() const;
%ignore Iodef::IODEFV2Value::operator float() const;
%ignore Iodef::IODEFV2Value::operator double() const;

/*
 * Force SWIG to use the IODEFV2Value * version of the Set() function,
 * so that the user might provide NULL IODEFV2Value. Force usage of the
 * std::string value, for binary data.
 */
%ignore Iodef::IODEFV2::set(char const *, int8_t);
%ignore Iodef::IODEFV2::set(char const *, uint8_t);
%ignore Iodef::IODEFV2::set(char const *, int16_t);
%ignore Iodef::IODEFV2::set(char const *, uint16_t);
%ignore Iodef::IODEFV2::set(char const *, char const *);
%ignore Iodef::IODEFV2::set(char const *, Iodef::IODEFV2Value &value);
%ignore Iodef::IODEFV2Path::set(Iodef::IODEFV2 &, int8_t) const;
%ignore Iodef::IODEFV2Path::set(Iodef::IODEFV2 &, uint8_t) const;
%ignore Iodef::IODEFV2Path::set(Iodef::IODEFV2 &, int16_t) const;
%ignore Iodef::IODEFV2Path::set(Iodef::IODEFV2 &, uint16_t) const;
%ignore Iodef::IODEFV2Path::set(Iodef::IODEFV2 &, char const *) const;
%ignore Iodef::IODEFV2Path::set(Iodef::IODEFV2 &, Iodef::IODEFV2Value &) const;
%ignore Iodef::IODEFV2Value::IODEFV2Value(char const *);

%ignore iodefv2_path_t;
%ignore iodefv2_object_t;
%ignore iodefv2_criteria_t;
%ignore operator iodefv2_path_t *() const;
%ignore operator iodefv2_criteria_t *() const;
%ignore operator iodefv2_object_t *() const;
%ignore operator iodefv2_document_t *() const;
%ignore operator iodefv2_time_t *() const;
%ignore operator iodefv2_value_t *() const;

/*
 * We need to unlock the interpreter lock before calling certain methods
 * because they might acquire internal libiodefv2 mutex that may also be
 * acquired undirectly through the libiodefv2 asynchronous stack.
 *
 * [Thread 2]: Libiodefv2 async stack
 * -> Lock internal mutexX
 *    -> libiodefv2_log()
 *       -> SWIG/C log callback
 *          -> Wait on Interpreter lock [DEADLOCK]
 *             -> Python logging callback (never called)
 *
 * [Thread 1] ConnectionPool::Recv()
 *  -> Acquire Interpreter lock
 *      *** At this time, thread 2 lock internal mutexX
 *      -> Wait on internal mutexX [DEADLOCK]
 *
 * In this situation, [Thread 1] hold the Interpreter lock and is
 * waiting on mutexX, which itself cannot be released by [Thread 2]
 * until [Thread 1] unlock the Interpreter lock.
 *
 * One rule to prevent deadlock is to always acquire mutex in the same
 * order. We thus need to make sure the interpreter lock is released
 * before calling C++ method that are susceptible to lock a mutex that
 * is also triggered from the asynchronous interface.
 *
 * Note that we are not releasing the Interpreter lock in all C++ call,
 * because it come at a performance cost, so we only try to do it when
 * needed.
 */

#ifdef SWIG_COMPILE_LIBIODEFV2
%feature("exceptionclass") Iodef::LibIodefError;
%feature("kwargs") Iodef::IODEFV2Class::getPath;
%feature("kwargs") Iodef::IODEFV2Path::getClass;
%feature("kwargs") Iodef::IODEFV2Path::getValueType;
%feature("kwargs") Iodef::IODEFV2Path::setIndex;
%feature("kwargs") Iodef::IODEFV2Path::getIndex;
%feature("kwargs") Iodef::IODEFV2Path::undefineIndex;
%feature("kwargs") Iodef::IODEFV2Path::compare;
%feature("kwargs") Iodef::IODEFV2Path::getName;
%feature("kwargs") Iodef::IODEFV2Path::isList;

%include libiodefv2.hxx
%include libiodefv2-error.hxx
%include iodefv2-criteria.hxx
%include iodefv2-value.hxx
%include iodefv2-path.hxx
%include iodefv2-time.hxx
%include iodefv2-class.hxx
%include iodefv2.hxx
#endif
