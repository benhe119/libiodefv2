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

#include "iodefv2.h"
#include "iodefv2-object-prv.h"
#include "iodefv2-object.h"
#include "iodefv2-class.h"


typedef struct iodefv2_linked_object iodefv2_linked_object_t;



iodefv2_class_id_t iodefv2_object_get_class(iodefv2_object_t *obj)
{
        return obj->_iodefv2_object_id;
}



iodefv2_object_t *iodefv2_object_ref(iodefv2_object_t *obj)
{
        int ret;

        ret = iodefv2_class_ref(obj->_iodefv2_object_id, obj);
        libiodefv2_return_val_if_fail(ret == 0, NULL);

        return obj;
}



void iodefv2_object_destroy(iodefv2_object_t *obj)
{
        iodefv2_class_destroy(obj->_iodefv2_object_id, obj);
}



int iodefv2_object_compare(iodefv2_object_t *obj1, iodefv2_object_t *obj2)
{
        if ( obj1->_iodefv2_object_id != obj2->_iodefv2_object_id )
                return -1;

        return iodefv2_class_compare(obj1->_iodefv2_object_id, obj1, obj2);
}



int iodefv2_object_clone(iodefv2_object_t *obj, iodefv2_object_t **dst)
{
        return iodefv2_class_clone(obj->_iodefv2_object_id, obj, (void **) dst);
}



int iodefv2_object_copy(iodefv2_object_t *src, iodefv2_object_t *dst)
{
        return iodefv2_class_copy(src->_iodefv2_object_id, src, dst);
}



int iodefv2_object_print(iodefv2_object_t *obj, libiodefv2_io_t *fd)
{
        return iodefv2_class_print(obj->_iodefv2_object_id, obj, fd);
}

int iodefv2_object_print_json(iodefv2_object_t *obj, libiodefv2_io_t *fd)
{
        return iodefv2_class_print_json(obj->_iodefv2_object_id, obj, fd);
}

int iodefv2_object_print_binary(iodefv2_object_t *obj, libiodefv2_io_t *fd)
{
        return iodefv2_class_print_binary(obj->_iodefv2_object_id, obj, fd);
}

void iodefv2_object_add(libiodefv2_list_t *head, iodefv2_object_t *object)
{
        libiodefv2_return_if_fail(iodefv2_class_is_listed(object->_iodefv2_object_id));
        libiodefv2_list_add(head, &((iodefv2_linked_object_t *) object)->_list);
}


void iodefv2_object_add_tail(libiodefv2_list_t *head, iodefv2_object_t *object)
{
        libiodefv2_return_if_fail(iodefv2_class_is_listed(object->_iodefv2_object_id));
        libiodefv2_list_add_tail(head, &((iodefv2_linked_object_t *) object)->_list);
}


void iodefv2_object_del(iodefv2_object_t *object)
{
        libiodefv2_return_if_fail(iodefv2_class_is_listed(object->_iodefv2_object_id));
        libiodefv2_list_del(&((iodefv2_linked_object_t *) object)->_list);
}


void iodefv2_object_del_init(iodefv2_object_t *object)
{
        libiodefv2_return_if_fail(iodefv2_class_is_listed(object->_iodefv2_object_id));
        libiodefv2_list_del(&((iodefv2_linked_object_t *) object)->_list);
        libiodefv2_list_init(&((iodefv2_linked_object_t *) object)->_list);
}


void *iodefv2_object_get_list_entry(libiodefv2_list_t *elem)
{
        return libiodefv2_list_entry(elem, iodefv2_linked_object_t, _list);
}
