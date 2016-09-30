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

#ifndef _LIBIODEFV2_IODEFV2_TIME_HXX
#define _LIBIODEFV2_IODEFV2_TIME_HXX

#include "iodefv2.h"

namespace Iodef {
        class IODEFV2Time {
            protected:
                iodefv2_time_t *_time;

            public:
                IODEFV2Time();
                IODEFV2Time(iodefv2_time_t *time);
                IODEFV2Time(const time_t time);
                IODEFV2Time(const char *string);
                IODEFV2Time(const struct timeval *tv);
                IODEFV2Time(const IODEFV2Time &value);
                ~IODEFV2Time();

                void set();
                void set(const time_t *time);
                void set(const char *string);
                void set(const struct timeval *tv);
                void setSec(uint32_t sec);
                void setUSec(uint32_t usec);
                void setGmtOffset(int32_t gmtoff);

                uint32_t getSec() const;
                uint32_t getUSec() const;
                int32_t getGmtOffset() const;
                double getTime() const { return (double) *this; };

                IODEFV2Time clone() const;
                const std::string toString() const;

                operator int() const;
                operator long() const;
                operator double() const;
                operator const std::string() const;
                operator iodefv2_time_t *() const;

                bool operator != (const IODEFV2Time &time) const;
                bool operator >= (const IODEFV2Time &time) const;
                bool operator <= (const IODEFV2Time &time) const;
                bool operator == (const IODEFV2Time &time) const;
                bool operator > (const IODEFV2Time &time) const;
                bool operator < (const IODEFV2Time &time) const;

                IODEFV2Time & operator = (const IODEFV2Time &p);
        };
};

#endif
