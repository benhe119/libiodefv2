#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "iodefv2.h"
#include "iodefv2-value.h"


static void cast_data(iodefv2_value_t *value)
{
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT32, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT64, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT64, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_FLOAT, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DOUBLE, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_TIME, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_ENUM, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_LIST, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_CLASS, -1) < 0);

        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_STRING, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_STRING);
}

static void cast_int8(void)
{
        iodefv2_value_t *value;

        assert(iodefv2_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT64, -1) < 0);

        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT16, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT16);
        assert(iodefv2_value_get_int16(value) == INT8_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT32, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT32);
        assert(iodefv2_value_get_int32(value) == INT8_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT64, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT64);
        assert(iodefv2_value_get_int64(value) == INT8_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_FLOAT, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_FLOAT);
        assert(iodefv2_value_get_float(value) == INT8_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DOUBLE, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_DOUBLE);
        assert(iodefv2_value_get_double(value) == INT8_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DATA, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_DATA);
        assert((int) iodefv2_data_get_uint32(iodefv2_value_get_data(value)) == INT8_MIN);
        iodefv2_value_destroy(value);
}

static void cast_int16(void)
{
        iodefv2_value_t *value;

        assert(iodefv2_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT64, -1) < 0);

        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT32, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT32);
        assert(iodefv2_value_get_int32(value) == INT16_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT64, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT64);
        assert(iodefv2_value_get_int64(value) == INT16_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_FLOAT, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_FLOAT);
        assert(iodefv2_value_get_float(value) == INT16_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DOUBLE, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_DOUBLE);
        assert(iodefv2_value_get_double(value) == INT16_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DATA, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_DATA);
        assert((int) iodefv2_data_get_uint32(iodefv2_value_get_data(value)) == INT16_MIN);
        iodefv2_value_destroy(value);
}


static void cast_int32(void)
{
        iodefv2_value_t *value;

        assert(iodefv2_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT64, -1) < 0);

        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT64, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_INT64);
        assert(iodefv2_value_get_int64(value) == INT32_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_FLOAT, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_FLOAT);
        assert(iodefv2_value_get_float(value) == INT32_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DOUBLE, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_DOUBLE);
        assert(iodefv2_value_get_double(value) == INT32_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DATA, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_DATA);
        assert((int) iodefv2_data_get_uint32(iodefv2_value_get_data(value)) == INT32_MIN);
        iodefv2_value_destroy(value);

        assert(iodefv2_value_new_int32(&value, INT32_MAX) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_TIME, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_TIME);
        assert(iodefv2_time_get_sec(iodefv2_value_get_time(value)) == INT32_MAX);
        iodefv2_value_destroy(value);
}


static void cast_string(void)
{
        iodefv2_data_t *data;
        iodefv2_value_t *value;
        libiodefv2_string_t *str;

        assert(libiodefv2_string_new_ref(&str, "abcdefgh") == 0);
        assert(iodefv2_value_new_string(&value, str) == 0);
        iodefv2_value_dont_have_own_data(value);

        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT8, -1) < 0);
//        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT32, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_INT64, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_UINT64, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_FLOAT, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DOUBLE, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_TIME, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_ENUM, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_LIST, -1) < 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_CLASS, -1) < 0);

        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_DATA, -1) == 0);
        assert(iodefv2_value_get_type(value) == IODEFV2_VALUE_TYPE_DATA);
        assert(data = iodefv2_value_get_data(value));
        assert(iodefv2_data_get_len(data) == (libiodefv2_string_get_len(str) + 1));
        assert(memcmp(libiodefv2_string_get_string(str), iodefv2_data_get_data(data), iodefv2_data_get_len(data)) == 0);
        libiodefv2_string_destroy(str);

        cast_data(value);
        iodefv2_value_destroy(value);

        assert(libiodefv2_string_new_ref(&str, "2008-01-01 20:42:31") == 0);
        assert(iodefv2_value_new_string(&value, str) == 0);
        assert(_iodefv2_value_cast(value, IODEFV2_VALUE_TYPE_TIME, -1) == 0);
        iodefv2_value_destroy(value);
}


int main(void)
{
        cast_int8();
        cast_int16();
        cast_int32();
        cast_string();
        printf("successful tests \n");
        exit(0);
}
