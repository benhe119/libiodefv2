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

#include "libiodefv2-error.hxx"
#include "iodefv2-time.hxx"

using namespace Iodef;


IODEFV2Time::~IODEFV2Time()
{
        if ( _time )
                iodefv2_time_destroy(_time);
}


IODEFV2Time::IODEFV2Time(const IODEFV2Time &time)
{
        _time = (time._time) ? iodefv2_time_ref(time._time) : NULL;
}


IODEFV2Time::IODEFV2Time()
{
        int ret;

        ret = iodefv2_time_new_from_gettimeofday(&_time);
        if ( ret < 0 )
                throw LibIodefError(ret);

}


IODEFV2Time::IODEFV2Time(iodefv2_time_t *time)
{
        _time = time;
}


IODEFV2Time::IODEFV2Time(time_t time)
{
        int ret;

        ret = iodefv2_time_new_from_time(&_time, &time);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Time::IODEFV2Time(const char *str)
{
        int ret;

        ret = iodefv2_time_new_from_string(&_time, str);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Time::IODEFV2Time(const struct timeval *tv)
{
        int ret;

        ret = iodefv2_time_new_from_timeval(&_time, tv);
        if ( ret < 0 )
                throw LibIodefError(ret);
}



void IODEFV2Time::set(const time_t *t)
{
        iodefv2_time_set_from_time(_time, t);
}


void IODEFV2Time::set(const struct timeval *tv)
{
        int ret;

        ret = iodefv2_time_set_from_timeval(_time, tv);
        if ( ret < 0 )
                throw LibIodefError(ret);
}



void IODEFV2Time::set(const char *str)
{
        int ret;

        ret = iodefv2_time_set_from_string(_time, str);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Time::set()
{
        int ret;

        ret = iodefv2_time_set_from_gettimeofday(_time);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


void IODEFV2Time::setSec(uint32_t sec)
{
        iodefv2_time_set_sec(_time, sec);
}


void IODEFV2Time::setUSec(uint32_t usec)
{
        iodefv2_time_set_usec(_time, usec);
}


void IODEFV2Time::setGmtOffset(int32_t gmtoff)
{
        iodefv2_time_set_gmt_offset(_time, gmtoff);
}


uint32_t IODEFV2Time::getSec() const
{
        return iodefv2_time_get_sec(_time);
}


uint32_t IODEFV2Time::getUSec() const
{
        return iodefv2_time_get_usec(_time);
}


int32_t IODEFV2Time::getGmtOffset() const
{
        return iodefv2_time_get_gmt_offset(_time);
}


IODEFV2Time IODEFV2Time::clone() const
{
        int ret;
        iodefv2_time_t *clone;

        ret = iodefv2_time_clone(_time, &clone);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return IODEFV2Time(clone);
}


const std::string IODEFV2Time::toString() const
{
        int ret;
        std::string cs;
        libiodefv2_string_t *str = NULL;

        ret = libiodefv2_string_new(&str);
        if ( ret < 0 )
                throw LibIodefError(ret);

        ret = iodefv2_time_to_string(_time, str);
        if ( ret < 0 )
                throw LibIodefError(ret);

        cs = libiodefv2_string_get_string(str);
        libiodefv2_string_destroy(str);

        return cs;
}


bool IODEFV2Time::operator <= (const IODEFV2Time &time) const
{
        return ( (double) *this <= (double) time );
}


bool IODEFV2Time::operator < (const IODEFV2Time &time) const
{
        return ( (double) *this < (double) time );
}


bool IODEFV2Time::operator >= (const IODEFV2Time &time) const
{
        return ( (double) *this >= (double) time );
}


bool IODEFV2Time::operator > (const IODEFV2Time &time) const
{
        return ( (double) *this > (double) time );
}


bool IODEFV2Time::operator != (const IODEFV2Time &time) const
{
        return ( (double) *this != (double) time );
}


bool IODEFV2Time::operator == (const IODEFV2Time &time) const
{
        return ( (double) *this == (double) time );
}


IODEFV2Time::operator int() const
{
        return getSec();
}


IODEFV2Time::operator long() const
{
        return getSec();
}


IODEFV2Time::operator double() const
{
        return getSec() + (getUSec() * 1e-6);
}


IODEFV2Time::operator const std::string() const
{
        return toString();
}


IODEFV2Time::operator iodefv2_time_t *() const
{
        return _time;
}


IODEFV2Time &IODEFV2Time::operator=(const IODEFV2Time &time)
{
        if ( this != &time && _time != time._time ) {
                if ( _time )
                        iodefv2_time_destroy(_time);

                _time = (time._time) ? iodefv2_time_ref(time._time) : NULL;
        }

        return *this;
}

