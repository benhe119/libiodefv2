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
#include "iodefv2-criteria.hxx"
#include "libiodefv2-error.hxx"

using namespace Iodef;

IODEFV2Criteria::~IODEFV2Criteria()
{
        if ( _criteria )
                iodefv2_criteria_destroy(_criteria);
}


IODEFV2Criteria::IODEFV2Criteria(const IODEFV2Criteria &criteria)
{
        _criteria = (criteria._criteria) ? iodefv2_criteria_ref(criteria._criteria) : NULL;
}


IODEFV2Criteria::IODEFV2Criteria(iodefv2_criteria_t *criteria)
{
        _criteria = criteria;
}


IODEFV2Criteria::IODEFV2Criteria(const char *criteria)
{
        int ret;

        ret = iodefv2_criteria_new_from_string(&_criteria, criteria);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Criteria::IODEFV2Criteria(const std::string &criteria)
{
        int ret;

        ret = iodefv2_criteria_new_from_string(&_criteria, criteria.c_str());
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Criteria::IODEFV2Criteria()
{
        int ret;

        ret = iodefv2_criteria_new(&_criteria);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEFV2Criteria IODEFV2Criteria::clone() const
{
        int ret;
        iodefv2_criteria_t *cl;

        ret = iodefv2_criteria_clone(this->_criteria, &cl);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return IODEFV2Criteria(cl);
}


void IODEFV2Criteria::andCriteria(const IODEFV2Criteria &criteria)
{
        iodefv2_criteria_and_criteria(this->_criteria, criteria._criteria);
}


void IODEFV2Criteria::orCriteria(const IODEFV2Criteria &criteria)
{
        iodefv2_criteria_or_criteria(this->_criteria, criteria._criteria);
}


int IODEFV2Criteria::match(IODEFV2 *message) const
{
        int ret;

        ret = iodefv2_criteria_match(this->_criteria, *message);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return ret;
}


const std::string IODEFV2Criteria::toString() const
{
        int ret;
        std::string s;
        libiodefv2_string_t *str;

        ret = libiodefv2_string_new(&str);
        if ( ret < 0 )
                throw LibIodefError(ret);

        ret = iodefv2_criteria_to_string(_criteria, str);
        if ( ret < 0 ) {
                libiodefv2_string_destroy(str);
                throw LibIodefError(ret);
        }

        s = libiodefv2_string_get_string(str);
        libiodefv2_string_destroy(str);

        return s;
}


IODEFV2Criteria::operator const std::string() const
{
        return toString();
}



IODEFV2Criteria::operator iodefv2_criteria_t *() const
{
        return _criteria;
}



IODEFV2Criteria &IODEFV2Criteria::operator=(const IODEFV2Criteria &criteria)
{
        if ( this != &criteria && _criteria != criteria._criteria ) {
                if ( _criteria )
                        iodefv2_criteria_destroy(_criteria);

                _criteria = (criteria._criteria) ? iodefv2_criteria_ref(criteria._criteria) : NULL;
        }

        return *this;
}
