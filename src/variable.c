/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodefv2-ids.com>
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

#include "libmissing.h"
#include "libiodefv2-list.h"
#include "libiodefv2-error.h"
#include "variable.h"


typedef struct {
        libiodefv2_list_t list;
        char *variable;
        char *value;
} variable_t;



static LIBIODEFV2_LIST(variable_list);



static variable_t *search_entry(const char *variable)
{
        int ret;
        libiodefv2_list_t *tmp;
        variable_t *item = NULL;

        libiodefv2_list_for_each(&variable_list, tmp) {
                item = libiodefv2_list_entry(tmp, variable_t, list);

                ret = strcasecmp(item->variable, variable);

                if ( ret == 0 )
                        return item;
        }

        return NULL;
}




static int create_entry(const char *variable, const char *value)
{
        variable_t *item;

        item = malloc(sizeof(*item));
        if ( ! item )
                return libiodefv2_error_from_errno(errno);

        item->variable = strdup(variable);
        if ( ! item->variable ) {
                free(item);
                return libiodefv2_error_from_errno(errno);
        }

        if ( ! value )
                item->value = NULL;
        else {
                item->value = strdup(value);
                if ( ! item->value ) {
                        free(item->variable);
                        free(item);
                        return libiodefv2_error_from_errno(errno);
                }
        }

        libiodefv2_list_add_tail(&variable_list, &item->list);

        return 0;
}


static void destroy_variable(variable_t *item)
{
        libiodefv2_list_del(&item->list);

        free(item->variable);

        if ( item->value )
                free(item->value);

        free(item);
}


/**
 * variable_get:
 * @variable: Variable to get the value from.
 *
 * Get value for the specified variable.
 *
 * Returns: Value of the variable, or NULL if the variable is not set.
 */
char *variable_get(const char *variable)
{
        variable_t *item;

        item = search_entry(variable);

        return ( item ) ? item->value : NULL;
}




/**
 * variable_set:
 * @variable: The variable in question.
 * @value: Value to assign to the variable.
 *
 * Set the specified variable to the given value.
 * The variable is created if it doesn't exit.
 *
 * Returns: 0 on success, -1 on error.
 */
int variable_set(const char *variable, const char *value)
{
        int ret = -1;
        variable_t *item;

        item = search_entry(variable);
        if ( ! item )
                ret = create_entry(variable, value);
        else {
                if ( item->value )
                        free(item->value);

                item->value = (value) ? strdup(value) : NULL;
        }

        return ( ret == 0 || item ) ? 0 : -1;
}




/**
 * variable_unset:
 * @variable: The variable in question.
 *
 * Delete the specified variable from the variable lists.
 *
 * Returns: 0 on success, -1 if variable could not be found.
 */
int variable_unset(const char *variable)
{
        variable_t *item;

        item = search_entry(variable);
        if ( ! item )
                return -1;

        destroy_variable(item);

        return 0;
}



void variable_unset_all(void)
{
        variable_t *item;
        libiodefv2_list_t *tmp, *bkp;

        libiodefv2_list_for_each_safe(&variable_list, tmp, bkp) {
                item = libiodefv2_list_entry(tmp, variable_t, list);
                destroy_variable(item);
        }
}
