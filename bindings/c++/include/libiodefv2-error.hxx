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

#ifndef _LIBIODEFV2_LIBIODEFV2_ERROR_HXX
#define _LIBIODEFV2_LIBIODEFV2_ERROR_HXX

#include <string>
#include <exception>

#define libiodefv2_except_if_fail(cond) do {                                   \
        if ( ! (cond) )                                                     \
                throw LibIodefError(libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION)); \
} while(0)


namespace Iodef {
        class LibIodefError: public std::exception {
            protected:
                int _error;
                std::string _message;

            public:
                virtual ~LibIodefError() throw() {};
                LibIodefError(void) throw();
                LibIodefError(int error) throw();
                LibIodefError(const std::string &message) throw();

                int getCode(void) const;

                virtual const char *what() const throw();
                operator const char *() const;
                operator int () const;
        };
};

#endif
