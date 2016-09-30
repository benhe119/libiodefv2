/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEFV2_IODEFV2_OBJECT_PRV_H
#define _LIBIODEFV2_IODEFV2_OBJECT_PRV_H

#define IODEFV2_OBJECT unsigned int _iodefv2_object_id
#define IODEFV2_LINKED_OBJECT IODEFV2_OBJECT; libiodefv2_list_t _list

struct iodefv2_object {
        IODEFV2_OBJECT;
};

struct iodefv2_linked_object {
        IODEFV2_OBJECT;
        libiodefv2_list_t _list;
};

#define iodefv2_linked_object_get_object(object) \
        (void *) libiodefv2_list_entry(object, struct iodefv2_linked_object, _list)

#endif /* _LIBIODEFV2_IODEFV2_OBJECT_PRV_H */
