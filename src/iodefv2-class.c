/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodefv2-ids.com>
* Author: Krzysztof Zaraska
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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>

#include "libmissing.h"
#include "libiodefv2-list.h"
#include "libiodefv2-log.h"
#include "libiodefv2-inttypes.h"
#include "libiodefv2-string.h"

#define LIBIODEFV2_ERROR_SOURCE_DEFAULT LIBIODEFV2_ERROR_SOURCE_IODEFV2_TYPE
#include "libiodefv2-error.h"

#include "iodefv2-time.h"
#include "iodefv2-data.h"
#include "iodefv2-value.h"
#include "iodefv2-class.h"

#include "iodefv2-tree-wrap.h"
#include "iodefv2-document-print.h"
#include "iodefv2-document-print-json.h"
#include "iodefv2-document-write.h"
#include "iodefv2-tree-data.h"
#include "iodefv2-path.h"



static inline int is_class_valid(iodefv2_class_id_t class)
{
        if ( class < 0 || (size_t) class >= sizeof(object_data) / sizeof(*object_data) )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_CLASS_UNKNOWN, "Unknown IODEFV2 class '%d'", (int) class);

        return 0;
}

static inline int is_child_valid(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        if ( child < 0 || (size_t) child >= object_data[class].children_list_elem )
                return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_CLASS_UNKNOWN_CHILD, "Unknown IODEFV2 child '%d' for class '%s'",
                                             (int) child, object_data[class].name);

        return 0;
}




iodefv2_class_child_id_t iodefv2_class_find_child(iodefv2_class_id_t class, const char *name)
{
        int ret;
        size_t i;
        const children_list_t *list;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        list = object_data[class].children_list;

        if ( list ) {
                for ( i = 0; i < object_data[class].children_list_elem; i++ )
                        if ( strcasecmp(list[i].name, name) == 0)
                                return i;
        }

        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_CLASS_UNKNOWN_CHILD, "Unknown IODEFV2 child '%s' for class '%s'", name, iodefv2_class_get_name(class));
}




libiodefv2_bool_t iodefv2_class_is_child_union_member(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return (object_data[class].children_list[child].union_id > 0) ? TRUE : FALSE;
}



libiodefv2_bool_t iodefv2_class_is_child_list(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].list;
}



libiodefv2_bool_t iodefv2_class_is_child_keyed_list(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].keyed_list;
}



iodefv2_value_type_id_t iodefv2_class_get_child_value_type(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].type;
}



size_t iodefv2_class_get_child_count(iodefv2_class_id_t class)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list_elem;
}



int iodefv2_class_get_child_union_id(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;
        const children_list_t *c;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        c = &object_data[class].children_list[child];
        if ( ! c->union_id )
                return libiodefv2_error(LIBIODEFV2_ERROR_IODEFV2_CLASS_CHILD_NOT_UNION);

        return c->union_id;
}



iodefv2_class_id_t iodefv2_class_get_child_class(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;
        const children_list_t *c;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        c = &object_data[class].children_list[child];
        if ( c->type != IODEFV2_VALUE_TYPE_CLASS && c->type != IODEFV2_VALUE_TYPE_ENUM )
                return libiodefv2_error(LIBIODEFV2_ERROR_IODEFV2_CLASS_CHILD_NOT_CLASS);

        return c->class;
}



const char *iodefv2_class_get_child_name(iodefv2_class_id_t class, iodefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return NULL;

        return object_data[class].children_list[child].name;
}




iodefv2_class_id_t iodefv2_class_find(const char *name)
{
        iodefv2_class_id_t i;

        for ( i = 0; object_data[i].name != NULL; i++ )
                if ( strcasecmp(object_data[i].name, name) == 0 )
                        return i;

        return libiodefv2_error_verbose(LIBIODEFV2_ERROR_IODEFV2_CLASS_UNKNOWN_NAME, "Unknown IODEFV2 class '%s'", name);
}


int iodefv2_class_enum_to_numeric(iodefv2_class_id_t class, const char *val)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        if ( ! object_data[class].to_numeric )
                return -1;

            return object_data[class].to_numeric(val);
}


const char *iodefv2_class_enum_to_string(iodefv2_class_id_t class, int val)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return NULL;

        if ( ! object_data[class].to_string )
                return NULL;

        return object_data[class].to_string(val);
}


int iodefv2_class_get_child(void *ptr, iodefv2_class_id_t class, iodefv2_class_child_id_t child, void **childptr)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].get_child(ptr, child, childptr);
}




int iodefv2_class_new_child(void *ptr, iodefv2_class_id_t class, iodefv2_class_child_id_t child, int n, void **childptr)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].new_child(ptr, child, n, childptr);
}



int iodefv2_class_destroy_child(void *ptr, iodefv2_class_id_t class, iodefv2_class_child_id_t child, int n)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].destroy_child(ptr, child, n);
}


int iodefv2_class_copy(iodefv2_class_id_t class, const void *src, void *dst)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].copy(src, dst);
}



int iodefv2_class_clone(iodefv2_class_id_t class, const void *src, void **dst)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].clone(src, dst);
}



int iodefv2_class_compare(iodefv2_class_id_t class, const void *c1, const void *c2)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].compare(c1, c2);
}



int iodefv2_class_destroy(iodefv2_class_id_t class, void *obj)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        object_data[class].destroy(obj);

        return 0;
}



int iodefv2_class_ref(iodefv2_class_id_t class, void *obj)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        object_data[class].ref(obj);

        return 0;
}



int iodefv2_class_print(iodefv2_class_id_t class, void *obj, libiodefv2_io_t *fd)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print(obj, fd);
}


int iodefv2_class_print_json(iodefv2_class_id_t class, void *obj, libiodefv2_io_t *fd)
{
        int ret;
        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print_json(obj, fd);
}


int iodefv2_class_print_binary(iodefv2_class_id_t class, void *obj, libiodefv2_io_t *fd)
{
        int ret;
        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print_binary(obj, fd);
}


const char *iodefv2_class_get_name(iodefv2_class_id_t class)
{
        if ( is_class_valid(class) < 0 )
                return NULL;

        return object_data[class].name;
}



libiodefv2_bool_t iodefv2_class_is_listed(iodefv2_class_id_t class)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].is_listed;
}
