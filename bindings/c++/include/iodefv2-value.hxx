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

#ifndef _LIBIODEFV2_IODEFV2_VALUE_HXX
#define _LIBIODEFV2_IODEFV2_VALUE_HXX

#include <vector>
#include "iodefv2-time.hxx"

namespace Iodef
{
        class IODEFV2;
}

namespace Iodef {
        class IODEFV2Value {
            private:
                void _InitFromString(const char *value, size_t size);
                std::string convert_string() const;

            protected:
                iodefv2_value_t *_value;
                std::string _myconv;

            public:
                enum IODEFV2ValueTypeEnum {
                        TYPE_UNKNOWN    = IODEFV2_VALUE_TYPE_UNKNOWN,
                        TYPE_INT8       = IODEFV2_VALUE_TYPE_INT8,
                        TYPE_UINT8      = IODEFV2_VALUE_TYPE_UINT8,
                        TYPE_INT16      = IODEFV2_VALUE_TYPE_INT16,
                        TYPE_UINT16     = IODEFV2_VALUE_TYPE_UINT16,
                        TYPE_INT32      = IODEFV2_VALUE_TYPE_INT32,
                        TYPE_UINT32     = IODEFV2_VALUE_TYPE_UINT32,
                        TYPE_INT64      = IODEFV2_VALUE_TYPE_INT64,
                        TYPE_UINT64     = IODEFV2_VALUE_TYPE_UINT64,
                        TYPE_FLOAT      = IODEFV2_VALUE_TYPE_FLOAT,
                        TYPE_DOUBLE     = IODEFV2_VALUE_TYPE_DOUBLE,
                        TYPE_STRING     = IODEFV2_VALUE_TYPE_STRING,
                        TYPE_TIME       = IODEFV2_VALUE_TYPE_TIME,
                        TYPE_DATA       = IODEFV2_VALUE_TYPE_DATA,
                        TYPE_ENUM       = IODEFV2_VALUE_TYPE_ENUM,
                        TYPE_LIST       = IODEFV2_VALUE_TYPE_LIST,
                        TYPE_CLASS      = IODEFV2_VALUE_TYPE_CLASS
                };

                IODEFV2ValueTypeEnum getType() const;
                bool isNull() const;

                IODEFV2Value();
                ~IODEFV2Value();
                IODEFV2Value(IODEFV2 *iodefv2);
                IODEFV2Value(const std::vector<IODEFV2> &value);
                IODEFV2Value(const IODEFV2Value &value);
                IODEFV2Value(const std::vector<IODEFV2Value> &value);
                IODEFV2Value(iodefv2_value_t *value);
                IODEFV2Value(const std::string &value);
                IODEFV2Value(const char *value);
                IODEFV2Value(int32_t value);
                IODEFV2Value(int64_t value);
                IODEFV2Value(uint64_t value);
                IODEFV2Value(float value);
                IODEFV2Value(double value);
                IODEFV2Value(Iodef::IODEFV2Time &time);

                int match(const IODEFV2Value &value, int op) const;
                IODEFV2Value clone() const;
                const std::string toString() const;

                operator int32_t() const;
                operator uint32_t() const;
                operator int64_t() const;
                operator uint64_t() const;
                operator double() const;
                operator std::vector<IODEFV2Value>() const;
                operator const char*() const;
                operator float() const;
                operator Iodef::IODEFV2Time() const;
                operator iodefv2_value_t *() const;

                IODEFV2Value & operator=(const IODEFV2Value &p);

                bool operator == (const std::vector<IODEFV2Value> &vlist) const;
                bool operator <= (const IODEFV2Value &value) const;
                bool operator >= (const IODEFV2Value &value) const;
                bool operator < (const IODEFV2Value &value) const;
                bool operator > (const IODEFV2Value &value) const;
                bool operator == (const IODEFV2Value &value) const;
                bool operator != (const IODEFV2Value &value) const;
        };
};

#endif
