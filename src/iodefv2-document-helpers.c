/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
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

#include <stdlib.h>
#include <string.h>

#include "iodefv2.h"
#include "iodefv2-document-helpers.h"


/**
 * iodefv2_document_set_value:
 * @message: Pointer to an #iodefv2_document_t object.
 * @path: Path to be set within @message.
 * @value: Value to associate @message[@path] with.
 *
 * This function will set the @path member within @message to the
 * provided @value.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int iodefv2_document_set_value(iodefv2_document_t *message, const char *path, iodefv2_value_t *value)
{
        int ret;
        iodefv2_path_t *ip;

        ret = iodefv2_path_new_fast(&ip, path);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_path_set(ip, message, value);
        iodefv2_path_destroy(ip);

        return ret;
}



/**
 * iodefv2_document_get_value:
 * @message: Pointer to an #iodefv2_document_t object.
 * @path: Path to retrieve the value from within @message.
 * @value: Pointer where the result should be stored.
 *
 * Retrieve the value stored within @path of @message and store it
 * in the user provided @value.
 *
 * Returns: A positive value in case @path was successfully retrieved
 * 0 if the path is empty, or a negative value if an error occured.
 */
int iodefv2_document_get_value(iodefv2_document_t *message, const char *path, iodefv2_value_t **value)
{
        int ret;
        iodefv2_path_t *ip;

        ret = iodefv2_path_new_fast(&ip, path);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_path_get(ip, message, value);
        iodefv2_path_destroy(ip);

        return ret;
}



/**
 * iodefv2_document_set_string:
 * @message: Pointer to an #iodefv2_document_t object.
 * @path: Path to be set within @message.
 * @value: Value to associate @message[@path] with.
 *
 * This function will set the @path member within @message to the
 * provided @value, which will be converted to the corresponding
 * @path value type.
 *
 * Example:
 * iodefv2_document_set_string(message, "alert.classification.text", "MyText");
 * iodefv2_document_set_string(message, "alert.source(0).service.port", "1024");
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int iodefv2_document_set_string(iodefv2_document_t *message, const char *path, const char *value)
{
        int ret;
        iodefv2_value_t *iv;
        libiodefv2_string_t *str;

        ret = libiodefv2_string_new_dup(&str, value);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_value_new_string(&iv, str);
        if ( ret < 0 ) {
                libiodefv2_string_destroy(str);
                return ret;
        }

        ret = iodefv2_document_set_value(message, path, iv);
        iodefv2_value_destroy(iv);

        return ret;
}



/**
 * iodefv2_document_get_string:
 * @message: Pointer to an #iodefv2_document_t object.
 * @path: Path to retrieve the string from within @message.
 * @result: Pointer where the result should be stored.
 *
 * Retrieve the string stored within @path of @message and store it
 * in the user provided @result.
 *
 * The caller is responssible for freeing @result.
 *
 * Returns: A positive value in case @path was successfully retrieved
 * 0 if the path is empty, or a negative value if an error occured.
 */
int iodefv2_document_get_string(iodefv2_document_t *message, const char *path, char **result)
{
        int ret;
        iodefv2_value_t *iv;
        libiodefv2_string_t *str;

        ret = iodefv2_document_get_value(message, path, &iv);
        if ( ret <= 0 )
                return ret;

        if ( iodefv2_value_get_type(iv) != IODEFV2_VALUE_TYPE_STRING ) {
                ret = _iodefv2_value_cast(iv, IODEFV2_VALUE_TYPE_STRING, 0);
                if ( ret < 0 )
                        goto err;
        }

        if ( ! (str = iodefv2_value_get_string(iv)) ) {
                ret = -1;
                goto err;
        }

        if ( libiodefv2_string_is_empty(str) ) {
                *result = NULL;
                return 0;
        }

        *result = strdup(libiodefv2_string_get_string(str));
        ret = libiodefv2_string_get_len(str);

err:
        iodefv2_value_destroy(iv);
        return ret;
}



/**
 * iodefv2_document_set_number:
 * @message: Pointer to an #iodefv2_document_t object.
 * @path: Path to be set within @message.
 * @number: Value to associate @message[@path] with.
 *
 * This function will set the @path member within @message to the
 * provided @value, which will be converted to the @path value type.
 *
 * Example:
 * iodefv2_document_set_number(message, "alert.assessment.confidence.confidence", 0.123456);
 * iodefv2_document_set_number(message, "alert.source(0).service.port", 1024);
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int iodefv2_document_set_number(iodefv2_document_t *message, const char *path, double number)
{
        int ret;
        iodefv2_value_t *iv;

        ret = iodefv2_value_new_double(&iv, number);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_document_set_value(message, path, iv);
        iodefv2_value_destroy(iv);

        return ret;

}


/**
 * iodefv2_document_get_number:
 * @message: Pointer to an #iodefv2_document_t object.
 * @path: Path to retrieve the number from within @message.
 * @result: Pointer where the result should be stored.
 *
 * Retrieve the number stored within @path of @message and store it
 * in the user provided @result.
 *
 * Returns: A positive value in case @path was successfully retrieved
 * 0 if the path is empty, or a negative value if an error occured.
 */
int iodefv2_document_get_number(iodefv2_document_t *message, const char *path, double *result)
{
        int ret;
        iodefv2_value_t *iv;

        ret = iodefv2_document_get_value(message, path, &iv);
        if ( ret <= 0 )
                return ret;

        if ( iodefv2_value_get_type(iv) != IODEFV2_VALUE_TYPE_DOUBLE ) {
                ret = _iodefv2_value_cast(iv, IODEFV2_VALUE_TYPE_DOUBLE, 0);
                if ( ret < 0 )
                        goto err;
        }

        ret = 1;
        *result = iodefv2_value_get_double(iv);

err:
        iodefv2_value_destroy(iv);
        return ret;
}


/**
 * iodefv2_document_set_data:
 * @message: Pointer to an #iodefv2_document_t object.
 * @path: Path to be set within @message.
 * @data: Pointer to data to associate @message[@path] with.
 * @size: Size of the data pointed to by @data.
 *
 * This function will set the @path member within @message to the
 * provided @data of size @size.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int iodefv2_document_set_data(iodefv2_document_t *message, const char *path, const unsigned char *data, size_t size)
{
        int ret;
        iodefv2_data_t *id;
        iodefv2_value_t *iv;

        ret = iodefv2_data_new_byte_string_dup(&id, data, size);
        if ( ret < 0 )
                return ret;

        ret = iodefv2_value_new_data(&iv, id);
        if ( ret < 0 ) {
                iodefv2_data_destroy(id);
                return ret;
        }

        ret = iodefv2_document_set_value(message, path, iv);
        iodefv2_value_destroy(iv);

        return ret;
}


int iodefv2_document_get_data(iodefv2_document_t *message, const char *path, unsigned char **data, size_t *size)
{
        int ret;
        iodefv2_data_t *dp;
        iodefv2_value_t *iv;

        ret = iodefv2_document_get_value(message, path, &iv);
        if ( ret <= 0 )
                return ret;

        if ( iodefv2_value_get_type(iv) != IODEFV2_VALUE_TYPE_DATA || ! (dp = iodefv2_value_get_data(iv)) ) {
                ret = -1;
                goto err;
        }

        *size = iodefv2_data_get_len(dp);
        *data = malloc(*size);
        if ( ! *data ) {
                ret = -1;
                goto err;
        }

        memcpy(*data, iodefv2_data_get_data(dp), *size);

err:
        iodefv2_value_destroy(iv);
        return ret;
}
