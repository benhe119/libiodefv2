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

#ifndef _LIBIODEFV2_IODEFV2_HXX
#define _LIBIODEFV2_IODEFV2_HXX

#include <string>
#include <iostream>
#include "iodefv2-value.hxx"
#include "iodefv2-time.hxx"

namespace Iodef
{
        class IODEFV2;
}

namespace Iodef {
        class IODEFV2 {
            protected:
                iodefv2_object_t *_object;

            public:
                ~IODEFV2();
                IODEFV2();
                IODEFV2(const char *json);
                IODEFV2(const IODEFV2 &iodefv2);
                IODEFV2(iodefv2_object_t *message);

                void set(const char *path, const std::vector<Iodef::IODEFV2> &value);
                void set(const char *path, Iodef::IODEFV2 *value);
                void set(const char *path, const std::vector<Iodef::IODEFV2Value> &value);
                void set(const char *path, const std::string &value);
                void set(const char *path, Iodef::IODEFV2Value *value);
                void set(const char *path, Iodef::IODEFV2Value &value);
                void set(const char *path, Iodef::IODEFV2Time &value);
                void set(const char *path, const char *value);
                void set(const char *path, int32_t value);
                void set(const char *path, int64_t value);
                void set(const char *path, uint64_t value);
                void set(const char *path, float value);
                void set(const char *path, double value);
                Iodef::IODEFV2Value get(const char *path);


                IODEFV2 clone();
                iodefv2_class_id_t getId() const;
                const std::string toString() const;
                const std::string toJSON() const;
                const std::string toBinary() const;

                operator const std::string() const;
                operator iodefv2_object_t *() const;
                IODEFV2 &operator = (const IODEFV2 &iodefv2);
                int operator == (const IODEFV2 *iodefv2);
        };
};

#endif /* __LIBIODEFV2_IODEFV2_HXX */
