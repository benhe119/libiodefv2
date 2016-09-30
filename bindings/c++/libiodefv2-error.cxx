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

#include <string>
#include <iostream>

#include "libiodefv2-error.hxx"
#include <iodefv2.h>


using namespace Iodef;


LibIodefError::LibIodefError(void) throw()
{
        _error = -1;
}


LibIodefError::LibIodefError(const std::string &message) throw()
{
        _error = -1;
        _message = message;
}


LibIodefError::LibIodefError(int error) throw()
{
        _error = error;
        _message = libiodefv2_strerror(error);
}


const char *LibIodefError::what() const throw()
{
        return _message.c_str();
}


int LibIodefError::getCode() const
{
        return libiodefv2_error_get_code(_error);
}


LibIodefError::operator const char *() const
{
        return _message.c_str();
}


LibIodefError::operator int () const
{
        return libiodefv2_error_get_code(_error);
}
