/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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
#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>

#define LIBIODEFV2_ERROR_SOURCE_DEFAULT LIBIODEFV2_ERROR_SOURCE_IODEFV2_CRITERIA
#include "iodefv2.h"
#include "iodefv2-criteria.h"


struct iodefv2_criterion {
        iodefv2_path_t *path;
        iodefv2_criterion_value_t *value;
        iodefv2_criterion_operator_t operator;
};


struct iodefv2_criteria {
        int refcount;
        libiodefv2_bool_t negated;
        iodefv2_criterion_t *criterion;
        struct iodefv2_criteria *or;
        struct iodefv2_criteria *and;
};



/**
 * iodefv2_criterion_operator_to_string:
 * @op: #iodefv2_criterion_operator_t type.
 *
 * Transforms @op to string.
 *
 * Returns: A pointer to an operator string or NULL.
 */
const char *iodefv2_criterion_operator_to_string(iodefv2_criterion_operator_t op)
{
        int i;
        const struct {
                iodefv2_criterion_operator_t operator;
                const char *name;
        } tbl[] = {
                { IODEFV2_CRITERION_OPERATOR_EQUAL,     "="            },
                { IODEFV2_CRITERION_OPERATOR_EQUAL_NOCASE, "=*"        },

                { IODEFV2_CRITERION_OPERATOR_NOT_EQUAL, "!="           },
                { IODEFV2_CRITERION_OPERATOR_NOT_EQUAL_NOCASE, "!=*"   },

                { IODEFV2_CRITERION_OPERATOR_LESSER, "<"               },
                { IODEFV2_CRITERION_OPERATOR_GREATER, ">"              },
                { IODEFV2_CRITERION_OPERATOR_LESSER_OR_EQUAL, "<="     },
                { IODEFV2_CRITERION_OPERATOR_GREATER_OR_EQUAL, ">="    },

                { IODEFV2_CRITERION_OPERATOR_REGEX, "~"                },
                { IODEFV2_CRITERION_OPERATOR_REGEX_NOCASE, "~*"        },
                { IODEFV2_CRITERION_OPERATOR_NOT_REGEX, "!~"           },
                { IODEFV2_CRITERION_OPERATOR_NOT_REGEX_NOCASE, "!~*"   },

                { IODEFV2_CRITERION_OPERATOR_SUBSTR, "<>"              },
                { IODEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE, "<>*"      },
                { IODEFV2_CRITERION_OPERATOR_NOT_SUBSTR, "!<>"         },
                { IODEFV2_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE, "!<>*" },

                { IODEFV2_CRITERION_OPERATOR_NOT_NULL, ""              },
                { IODEFV2_CRITERION_OPERATOR_NULL, "!"                 },
        };

        for ( i = 0; tbl[i].operator != 0; i++ )
                if ( op == tbl[i].operator )
                        return tbl[i].name;

        return NULL;
}



/**
 * iodefv2_criterion_new:
 * @criterion: Address where to store the created #iodefv2_criterion_t object.
 * @path: Pointer to an #iodefv2_path_t object.
 * @value: Pointer to an #iodefv2_criterion_value_t object.
 * @op: #iodefv2_criterion_operator_t to use for matching this criterion.
 *
 * Creates a new #iodefv2_criterion_t object and store it in @criterion.
 * Matching this criterion will result in comparing the object value
 * pointed by @path against the provided @value, using @op.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_criterion_new(iodefv2_criterion_t **criterion, iodefv2_path_t *path,
                        iodefv2_criterion_value_t *value, iodefv2_criterion_operator_t op)
{
        libiodefv2_return_val_if_fail(path != NULL, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(! (value == NULL && ! (op & IODEFV2_CRITERION_OPERATOR_NULL)), libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        *criterion = calloc(1, sizeof(**criterion));
        if ( ! *criterion )
                return libiodefv2_error_from_errno(errno);

        (*criterion)->path = path;
        (*criterion)->value = value;
        (*criterion)->operator = op;

        return 0;
}



/**
 * iodefv2_criterion_destroy:
 * @criterion: Pointer to a #iodefv2_criterion_t object.
 *
 * Destroys @criterion and its content.
 */
void iodefv2_criterion_destroy(iodefv2_criterion_t *criterion)
{
        libiodefv2_return_if_fail(criterion);

        iodefv2_path_destroy(criterion->path);

        if ( criterion->value ) /* can be NULL if operator is is_null or is_not_null */
                iodefv2_criterion_value_destroy(criterion->value);

        free(criterion);
}



/**
 * iodefv2_criterion_clone:
 * @criterion: Pointer to a #iodefv2_criterion_t object to clone.
 * @dst: Address where to store the cloned #iodefv2_criterion_t object.
 *
 * Clones @criterion and stores the cloned criterion within @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_criterion_clone(const iodefv2_criterion_t *criterion, iodefv2_criterion_t **dst)
{
        int ret;
        iodefv2_path_t *path;
        iodefv2_criterion_value_t *value = NULL;

        libiodefv2_return_val_if_fail(criterion, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_path_clone(criterion->path, &path);
        if ( ret < 0 )
                return ret;

        if ( criterion->value ) {
                ret = iodefv2_criterion_value_clone(criterion->value, &value);
                if ( ret < 0 ) {
                        iodefv2_path_destroy(path);
                        return ret;
                }
        }

        ret = iodefv2_criterion_new(dst, path, value, criterion->operator);
        if ( ret < 0 ) {
                iodefv2_path_destroy(path);
                iodefv2_criterion_value_destroy(value);
                return ret;
        }

        return 0;
}



/**
 * iodefv2_criterion_print:
 * @criterion: Pointer to a #iodefv2_criterion_t object.
 * @fd: Pointer to a #libiodefv2_io_t object.
 *
 * Dump @criterion to @fd in the form of:
 * [path] [operator] [value]
 *
 * Or if there is no value associated with the criterion:
 * [operator] [path]
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_criterion_print(const iodefv2_criterion_t *criterion, libiodefv2_io_t *fd)
{
        int ret;
        libiodefv2_string_t *out;

        libiodefv2_return_val_if_fail(criterion, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(fd, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = libiodefv2_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_criterion_to_string(criterion, out);
        if ( ret < 0 ) {
                libiodefv2_string_destroy(out);
                return ret;
        }

        ret = libiodefv2_io_write(fd, libiodefv2_string_get_string(out), libiodefv2_string_get_len(out));
        libiodefv2_string_destroy(out);

        return ret;
}



/**
 * iodefv2_criterion_to_string:
 * @criterion: Pointer to a #iodefv2_criterion_t object.
 * @out: Pointer to a #libiodefv2_string_t object.
 *
 * Dump @criterion as a string to the @out buffer in the form of:
 * [path] [operator] [value]
 *
 * Or if there is no value associated with the criterion:
 * [operator] [path]
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_criterion_to_string(const iodefv2_criterion_t *criterion, libiodefv2_string_t *out)
{
        const char *name, *operator;

        libiodefv2_return_val_if_fail(criterion, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(out, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        operator = iodefv2_criterion_operator_to_string(criterion->operator);
        if ( ! operator )
                return -1;

        name = iodefv2_path_get_name(criterion->path, -1);

        if ( ! criterion->value )
                return libiodefv2_string_sprintf(out, "%s%s%s", operator, (*operator) ? " " : "", name);

        libiodefv2_string_sprintf(out, "%s %s ", name, operator);

        return iodefv2_criterion_value_to_string(criterion->value, out);
}



/**
 * iodefv2_criterion_get_path:
 * @criterion: Pointer to a #iodefv2_criterion_t object.
 *
 * Used to access the #iodefv2_path_t object associated with @criterion.
 *
 * Returns: the #iodefv2_path_t object associated with @criterion.
 */
iodefv2_path_t *iodefv2_criterion_get_path(const iodefv2_criterion_t *criterion)
{
        libiodefv2_return_val_if_fail(criterion, NULL);
        return criterion->path;
}



/**
 * iodefv2_criterion_get_value:
 * @criterion: Pointer to a #iodefv2_criterion_t object.
 *
 * Used to access the #iodefv2_criterion_value_t associated with @criterion.
 * There might be no value specifically if the provided #iodefv2_criterion_operator_t
 * was IODEFV2_CRITERION_OPERATOR_NULL or IODEFV2_CRITERION_OPERATOR_NOT_NULL.
 *
 * Returns: the #iodefv2_criterion_value_t object associated with @criterion.
 */
iodefv2_criterion_value_t *iodefv2_criterion_get_value(const iodefv2_criterion_t *criterion)
{
        libiodefv2_return_val_if_fail(criterion, NULL);
        return criterion->value;
}




/**
 * iodefv2_criterion_get_operator:
 * @criterion: Pointer to a #iodefv2_criterion_t object.
 *
 * Used to access the #iodefv2_criterion_operator_t enumeration associated with @criterion.
 *
 * Returns: the #iodefv2_criterion_operator_t associated with @criterion.
 */
iodefv2_criterion_operator_t iodefv2_criterion_get_operator(const iodefv2_criterion_t *criterion)
{
        libiodefv2_return_val_if_fail(criterion, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        return criterion->operator;
}



/**
 * iodefv2_criterion_match:
 * @criterion: Pointer to a #iodefv2_criterion_t object.
 * @object: Pointer to a #iodefv2_object_t object to match against @criterion.
 *
 * Matches @message against the provided @criterion. This implies retrieving the
 * value associated with @criterion path, and matching it with the @iodefv2_criterion_value_t
 * object within @criterion.
 *
 * Returns: 1 for a match, 0 for no match, or a negative value if an error occured.
 */
int iodefv2_criterion_match(const iodefv2_criterion_t *criterion, void *object)
{
        int ret;
        iodefv2_value_t *value = NULL;

        libiodefv2_return_val_if_fail(criterion, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(object, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_path_get(criterion->path, object, &value);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_criterion_value_match(criterion->value, value, criterion->operator);
        if ( value )
                iodefv2_value_destroy(value);

        if ( ret < 0 )
                return ret;

        return (ret > 0) ? 1 : 0;
}



/**
 * iodefv2_criteria_new:
 * @criteria: Address where to store the created #iodefv2_criteria_t object.
 *
 * Creates a new #iodefv2_criteria_t object and store it into @criteria.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_criteria_new(iodefv2_criteria_t **criteria)
{
        *criteria = calloc(1, sizeof(**criteria));
        if ( ! *criteria )
                return libiodefv2_error_from_errno(errno);

        (*criteria)->or = NULL;
        (*criteria)->and = NULL;
        (*criteria)->refcount = 1;

        return 0;
}



/**
 * iodefv2_criteria_destroy:
 * @criteria: Pointer to a #iodefv2_criteria_t object.
 *
 * Destroys @criteria and its content.
 */
void iodefv2_criteria_destroy(iodefv2_criteria_t *criteria)
{
        libiodefv2_return_if_fail(criteria);

        if ( --criteria->refcount )
                return;

        if ( criteria->criterion )
                iodefv2_criterion_destroy(criteria->criterion);

        if ( criteria->or )
                iodefv2_criteria_destroy(criteria->or);

        if ( criteria->and )
                iodefv2_criteria_destroy(criteria->and);

        free(criteria);
}



/**
 * iodefv2_criteria_ref:
 * @criteria: Pointer to a #iodefv2_criteria_t object to reference.
 *
 * Increases @criteria reference count.
 *
 * iodefv2_criteria_destroy() will decrease the refcount until it reaches
 * 0, at which point @criteria will be destroyed.
 *
 * Returns: @criteria.
 */
iodefv2_criteria_t *iodefv2_criteria_ref(iodefv2_criteria_t *criteria)
{
        libiodefv2_return_val_if_fail(criteria, NULL);

        criteria->refcount++;
        return criteria;
}


/**
 * iodefv2_criteria_clone:
 * @src: Pointer to a #iodefv2_criteria_t object to clone.
 * @dst: Address where to store the cloned #iodefv2_criteria_t object.
 *
 * Clones @src and stores the cloned criteria within @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodefv2_criteria_clone(iodefv2_criteria_t *src, iodefv2_criteria_t **dst)
{
        int ret;
        iodefv2_criteria_t *new;

        libiodefv2_return_val_if_fail(src, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_criteria_new(dst);
        if ( ret < 0 )
                return ret;

        new = *dst;
        new->negated = src->negated;

        if ( src->or ) {
                ret = iodefv2_criteria_clone(src->or, &new->or);
                if ( ret < 0 ) {
                        iodefv2_criteria_destroy(new);
                        return ret;
                }
        }

        if ( src->and ) {
                ret = iodefv2_criteria_clone(src->and, &new->and);
                if ( ret < 0 ) {
                        iodefv2_criteria_destroy(new);
                        return ret;
                }
        }

        ret = iodefv2_criterion_clone(src->criterion, &new->criterion);
        if ( ret < 0 ) {
                iodefv2_criteria_destroy(new);
                return ret;
        }

        return 0;
}



iodefv2_criteria_t *iodefv2_criteria_get_or(const iodefv2_criteria_t *criteria)
{
        libiodefv2_return_val_if_fail(criteria, NULL);
        return criteria->or;
}



iodefv2_criteria_t *iodefv2_criteria_get_and(const iodefv2_criteria_t *criteria)
{
        libiodefv2_return_val_if_fail(criteria, NULL);
        return criteria->and;
}



int iodefv2_criteria_print(const iodefv2_criteria_t *criteria, libiodefv2_io_t *fd)
{
        int ret;
        libiodefv2_string_t *out;

        libiodefv2_return_val_if_fail(criteria, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(fd, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = libiodefv2_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_criteria_to_string(criteria, out);
        if ( ret < 0 )
                return ret;

        ret = libiodefv2_io_write(fd, libiodefv2_string_get_string(out), libiodefv2_string_get_len(out));
        libiodefv2_string_destroy(out);

        return ret;
}



int iodefv2_criteria_to_string(const iodefv2_criteria_t *criteria, libiodefv2_string_t *out)
{
        libiodefv2_return_val_if_fail(criteria, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(out, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        if ( criteria->or )
                libiodefv2_string_sprintf(out, "((");

        iodefv2_criterion_to_string(criteria->criterion, out);

        if ( criteria->and ) {
                libiodefv2_string_sprintf(out, " && ");
                iodefv2_criteria_to_string(criteria->and, out);
        }

        if ( criteria->or ) {
                libiodefv2_string_sprintf(out, ") || (");
                iodefv2_criteria_to_string(criteria->or, out);
                libiodefv2_string_sprintf(out, "))");
        }

        return 0;
}



libiodefv2_bool_t iodefv2_criteria_is_criterion(const iodefv2_criteria_t *criteria)
{
        libiodefv2_return_val_if_fail(criteria, FALSE);
        return (criteria->criterion != NULL) ? TRUE : FALSE;
}



iodefv2_criterion_t *iodefv2_criteria_get_criterion(const iodefv2_criteria_t *criteria)
{
        libiodefv2_return_val_if_fail(criteria, NULL);
        return criteria->criterion;
}



void iodefv2_criteria_or_criteria(iodefv2_criteria_t *criteria, iodefv2_criteria_t *criteria2)
{
        libiodefv2_return_if_fail(criteria);
        libiodefv2_return_if_fail(criteria2);

        while ( criteria->or )
                criteria = criteria->or;

        criteria->or = criteria2;
}



int iodefv2_criteria_and_criteria(iodefv2_criteria_t *criteria, iodefv2_criteria_t *criteria2)
{
        int ret;
        iodefv2_criteria_t *new, *last = NULL;

        libiodefv2_return_val_if_fail(criteria, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(criteria2, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        while ( criteria ) {
                last = criteria;

                if ( criteria->or ) {
                        ret = iodefv2_criteria_clone(criteria2, &new);
                        if ( ret < 0 )
                                return ret;

                        ret = iodefv2_criteria_and_criteria(criteria->or, new);
                        if ( ret < 0 )
                                return ret;
                }

                criteria = criteria->and;
        }

        last->and = criteria2;

        return 0;
}


void iodefv2_criteria_set_negation(iodefv2_criteria_t *criteria, libiodefv2_bool_t negate)
{
        libiodefv2_return_if_fail(criteria);
        criteria->negated = negate;
}


libiodefv2_bool_t iodefv2_criteria_get_negation(const iodefv2_criteria_t *criteria)
{
        libiodefv2_return_val_if_fail(criteria, FALSE);
        return criteria->negated;
}


void iodefv2_criteria_set_criterion(iodefv2_criteria_t *criteria, iodefv2_criterion_t *criterion)
{
        libiodefv2_return_if_fail(criteria);
        libiodefv2_return_if_fail(criterion);

        criteria->criterion = criterion;
}




/**
 * iodefv2_criteria_match:
 * @criteria: Pointer to a #iodefv2_criteria_t object.
 * @object: Pointer to a #iodefv2_object_t object.
 *
 * Matches @object against the provided criteria.
 *
 * Returns: 1 if criteria match, 0 if it did not, a negative value if an error occured.
 */
int iodefv2_criteria_match(const iodefv2_criteria_t *criteria, void *object)
{
        int ret;

        libiodefv2_return_val_if_fail(criteria, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));
        libiodefv2_return_val_if_fail(object, libiodefv2_error(LIBIODEFV2_ERROR_ASSERTION));

        ret = iodefv2_criterion_match(criteria->criterion, object);
        if ( ret < 0 )
                return ret;

        if ( ret == 1 && criteria->and )
                ret = iodefv2_criteria_match(criteria->and, object);

        if ( ret == 0 && criteria->or )
                ret = iodefv2_criteria_match(criteria->or, object);

        if ( ret < 0 )
                return ret;

        return (criteria->negated) ? !ret : ret;
}
