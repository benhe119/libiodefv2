<%!
import sys
sys.path.append('../')
from field import Struct, EnumField, Union, DataType, Field, PreDeclared
%>
<%def name="struct_func(struct)">
${structConstructor(struct.short_type_name, struct.type_name)}
##${structRef(struct.short_type_name, struct.type_name)}
${structDestroy(struct.short_type_name, struct.type_name, struct.top_level)}
${struct_fields(struct)}
${structCopy(struct.short_type_name, struct.type_name)}
${structClone(struct.short_type_name, struct.type_name)}
${structCmp(struct.short_type_name)}
</%def>
<%def name="enums(enum)">
/*****
 * ${name_lib}_${enum.short_type_name}_to_numeric:
 * @name: pointer to an ${name_lib.upper()} string representation of a #${enum.type_name} value.
 *
 * Returns: the numeric equivalent of @name, or -1 if @name is not valid.
 */
${enum.type_name} ${name_lib.upper()}${enum.short_type_name}::${enum.short_type_name}_to_numeric(const char *name)
{
        return ${name_lib}_${enum.short_type_name}_to_numeric(name);
}

/**
 * ${name_lib}_${enum.short_type_name}_to_string:
 * @val: an enumeration value for #${enum.type_name}.
 *
 * Return the ${name_lib.upper()} string equivalent of @val provided #${enum.type_name} value.
 *
 * Returns: a pointer to the string describing @val, or NULL if @val is invalid.
 */
const char *${name_lib.upper()}${enum.short_type_name}::to_string(${enum.type_name} val)
{
        return ${name_lib}_${enum.short_type_name}_to_string(val);
}
</%def>
<%def name="footer()">
void ${name_lib.upper()}Message::set_pmsg(${prefix}_msg_t *msg)
{
        ${name_lib}_${top_class}_set_pmsg(_priv, msg);
}


${prefix}_msg_t *${name_lib.upper()}Message::get_pmsg()
{
        return ${name_lib}_${top_class}_get_pmsg(_priv);
}


/**
 * ${name_lib}_${top_class}_destroy:
 * @ptr: pointer to a #${name_lib}_${top_class}_t object.
 *
 * Destroy @ptr and all of it's children.
 * The objects are only destroyed if their reference count reach zero.
 */
void ${name_lib.upper()}Message::~${name_lib.upper()}Message()
{
        ${name_lib}_${top_class}_destroy(_priv);
}
</%def>
<%def name="header()">
/*****
*
* Based on ${name_lib}-tree-wrap.c.mako

* Author: Sebastien Tricaud <stricaud@inl.fr>
*
* This file is part of the ${prefix.capitalize()} library.
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

/*
 * This file was automatically generated by our generator, version ${generator_version}
 *
 * Do not make changes to this file unless you know what you are doing.
 * modify the template interface file instead.
 * ${name_lib.upper()} version : ${lib_version}
 * Template file: generate-tree-wrap.cxx.mako
 *
 */

#include "config.h"

#include "${name_lib}-tree-wrap.hxx"
#include "common.h"


static std::string *to_string(${prefix}_string_t *str)
{
        return new std::string(${prefix}_string_get_string(str));
}


static ${prefix}_string_t *from_string(std::string *str)
{
        int ret;
        ${prefix}_string_t *pstr;

        ret = ${prefix}_string_new_dup_fast(&pstr, str->c_str(), str->length());
        if ( ret < 0 )
                throw LibIdemfv2Error(ret);

        return pstr;
}
</%def>
<%def name="structConstructor(short_type_name, type_name)">
/**
 * ${name_lib}_${short_type_name}_new:
 * @ret: Pointer where to store the created #${type_name} object.
 *
 * Create a new #${type_name} object.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
${name_lib.upper()}${short_type_name}::${name_lib.upper()}${short_type_name}()
{
        ${name_lib}_${short_type_name}_new(&_priv);
}
</%def>
<%def name="structCopy(short_type_name, type_name)">
/**
 * ${name_lib}_${short_type_name}_copy:
 * @src: Source of the copy.
 * @dst: Where to copy the object.
 *
 * Copy a new #${type_name} object from @src to @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int ${name_lib.upper()}${short_type_name}::copy(${name_lib.upper()}${short_type_name} *dst)
{
        return ${name_lib}_${short_type_name}_copy(_priv, dst->_priv);
}

</%def>
<%def name="structClone(short_type_name, type_name)">
/**
 * ${name_lib}_${short_type_name}_clone:
 * @src: Object to be cloned.
 * @dst: Address where to store the pointer to the cloned object.
 *
 * Create a copy of @src, and store it in @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
${name_lib.upper()}${short_type_name} *${name_lib.upper()}${short_type_name}::clone()
{
        int ret;
        ${type_name} *dst;
        ${name_lib.upper()}${short_type_name} *ptr;

        ret = ${name_lib}_${short_type_name}_clone(_priv, &dst);
        if ( ret < 0 )
                throw LibIdemfv2Error(ret);

        ptr = new ${name_lib.upper()}${short_type_name}();
        ${name_lib}_${short_type_name}_destroy(ptr->_priv);
        ptr->_priv = dst;

        return ptr;
}
</%def>
<%def name="structCmp(short_type_name)">
/**
 * ${name_lib}_${short_type_name}_compare:
 * @obj1: Object to compare with @obj2.
 * @obj2: Object to compare with @obj1.
 *
 * Compare @obj1 with @obj2.
 *
 * Returns: 0 on match, a negative value on comparison failure.
 */
int ${name_lib.upper()}${short_type_name}::compare(${name_lib.upper()}${short_type_name} *obj)
{
        return ${name_lib}_${short_type_name}_compare(_priv, obj->_priv);
}
</%def>
<%def name="structDestroy(short_type_name, type_name, toplevel)">
% if toplevel == 0:
/**
 * ${name_lib}_${short_type_name}_destroy:
 * @ptr: pointer to a #${type_name} object.
 *
 * Destroy @ptr and all of it's children.
 * The objects are only destroyed if their reference count reach zero.
 */
${name_lib.upper()}${short_type_name}::~${name_lib.upper()}${short_type_name}()
{
        ${name_lib}_${short_type_name}_destroy(_priv);
}
% endif
</%def>
<%def name="structFieldNormal(field, short_type_name, type_name, name_member=None)">

<%
    ptr = ""
    (name, field_is_op_int) = (name_member, False) if name_member else (field.name, field.is_op_int)
    if(field.ptr == 1):
        if field.type == DataType.STRUCT or field.type == DataType.PRIMITIVE_STRUCT:
            ptr = "*"
        else:
            refer = "*"
    if(field.ptr != 1):
        if field.type == DataType.STRUCT or field.type == DataType.PRIMITIVE_STRUCT or field_is_op_int:
            ptr = "*"
            refer = "&"
    if(field.type_name == prefix + "_string_t"):
        type="std::string"
        fromcast="from_string"
        tocast="to_string"
    if(field.type_name != prefix + "_string_t"):
        type = field.type_name ;
        fromcast = ""
        tocast = ""
%>
${type} ${ptr} ${name_lib.upper()}${short_type_name}::get_${name}()
{
        return ${tocast}(${name_lib}_${short_type_name}_get_${name}(_priv));
}
<%
if field.name == "class":
    field_name = "class_str"
elif field.name == "virtual":
    field_name = "virtual_sys"
elif field.name == "operator":
    field_name = "operator_indic"
else:
    field_name = field.name
%>
%if field_is_op_int:
void ${name_lib.upper()}${short_type_name}::set_${field_name}(${field.type_name} ${field_name})
{
        ${prefix}_return_if_fail(ptr);
        ptr->${field_name} = ${field_name};
        ptr->${field_name}_is_set = 1;
}


void ${name_lib.upper()}${short_type_name}::unset_${field.name}()
{
        ${name_lib}_${short_type_name}_unset_${field.name}(_priv);
}
%elif  field.type == DataType.STRUCT or field.type == DataType.PRIMITIVE_STRUCT:
% if (field.ptr !=0):
<% destroy_func = field.short_type_name+"_destroy" %>
%if  field.type != DataType.PRIMITIVE_STRUCT:
<% destroy_func = name_lib + "_"+destroy_func %>
% endif

void ${name_lib.upper()}${short_type_name}::set_${field_name}(${field.type_name} *${field_name})
{
        ${name_lib}_${short_type_name}_set_${field.name}(this, ${field_name});
}
% else:
<%
    destroy_internal_func = field.short_type_name + "_destroy_internal"
    if field.type != DataType.PRIMITIVE and field.type != DataType.PRIMITIVE_STRUCT:
        destroy_internal_func = name_lib + "_"+destroy_internal_func
%>
void ${name_lib.upper()}${short_type_name}::set_${field.name}(${field.type_name} *$field_name)
{
        ${name_lib}_${short_type_name}_set_${field.name}(this, ${field_name});
}
% endif
% else:
% if field.ptr == 1:
void ${name_lib.upper()}${short_type_name}::set_${field.name}(${field.type_name} *${field_name})
{
        ${name_lib}_${short_type_name}_set_${field.name}(this, ${field_name});
}
% else:
void ${name_lib.upper()}${short_type_name}::set_${field.name}(${field.type_name} ${field_name})
{
        ${name_lib}_${short_type_name}_set_${field.name}(this, ${field_name});
}
%endif
%endif
</%def>
<%def name="struct_field_union(field, short_type_name, type_name)">
/**
 * ${name_lib}_${short_type_name}_get_${field.var}:
 * @ptr: pointer to a #${type_name} object.
 *
 * Access the ${field.var} children of @ptr.
 *
 * Returns: a pointer to the #${field.type_name} children, or NULL if it is not set.
 */
${field.type_name} ${name_lib.upper()}${short_type_name}::get_${field.var}()
{
        return ${name_lib}_${short_type_name}_get_${field.var}(_priv);
}
% for member in field.list_member:
/**
 * ${name_lib}_${short_type_name}_get_${member.name}:
 * @ptr: pointer to a #${type_name} object.
 *
 * Access the ${member.name} children of @ptr.
 *
 * Returns: a pointer to the #${member.type_name} children, or NULL if it is not set.
 */
${member.type_name} *${name_lib.upper()}${short_type_name}::get_${member.name}()
{
        return ${name_lib}_${short_type_name}_get_${member.name}(_priv);
}
/**
 * ${name_lib}_${short_type_name}_set_${member.name}:
 * @ptr: pointer to a #${type_name} object.
 * @${member.name}: pointer to a #${member.type_name} object.
 *
 * Set @${member.name} object as a children of @ptr.
 * if @ptr already contain a @${member.name} object, then it is destroyed,
 * and updated to point to the provided @${member.name} object.
 */
void ${name_lib.upper()}${short_type_name}::set_${member.name}(${member.type_name} *${member.name})
{
        ${name_lib}_${short_type_name}_set_${member.name}(_priv, ${member.name});
}
/**
 * ${name_lib}_${short_type_name}_new_${member.name}:
 * @ptr: pointer to a #${type_name} object.
 * @ret: pointer where to store the created #${member.type_name} object.
 *
 * Create a new ${member.type_name} object, children of #${type_name}.
 * If @ptr already contain a #${member.type_name} object, then it is destroyed.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
${name_lib.upper()}${member.short_type_name} *${name_lib.upper()}${short_type_name}::new_${member.name}()
{
        int ret;
        ${member.type_name} *ptr;
        ${name_lib.upper()}${member.short_type_name} *obj;

        ret = ${name_lib}_${short_type_name}_new_${member.name}(_priv, &ptr);
        if ( ret < 0 )
                throw LibIdemfv2Error(ret);

        obj = new ${name_lib.upper()}${member.short_type_name};
        ${name_lib}_${member.short_type_name}_destroy(obj->_priv);
        obj->_priv = ptr;
}
% endfor
</%def>
<%def name="struct_field_list(field, short_type_name, type_name)">
<%
    new_field_function = field.short_type_name+"_new(ret)"
    if field.type != DataType.PRIMITIVE and field.type != DataType.PRIMITIVE_STRUCT:
        new_field_function = name_lib + "_" + new_field_function
%>
/**
 * ${name_lib}_${short_type_name}_get_next_${field.short_name}:
 * @${short_type_name}: pointer to a #${type_name} object.
 * @${field.short_type_name}_cur: pointer to a #${field.type_name} object.
 *
 * Get the next #${field.type_name} object listed in @ptr.
 * When iterating over the ${field.type_name} object listed in @ptr,
 * @object should be set to the latest returned #${field.type_name} object.
 *
 * Returns: the next #${field.type_name} in the list.
 */
${field.type_name} *${name_lib.upper()}${short_type_name}::get_next_${field.short_name}(${field.type_name} *${field.short_type_name}_cur)
{
        return ${name_lib}_${short_type_name}_get_next_${field.short_name}(_priv, ${field.short_type_name}_cur);
}


/**
 * ${name_lib}_${short_type_name}_set_${field.short_name}:
 * @ptr: pointer to a #${type_name} object.
 * @object: pointer to a #${field.type_name} object.
 * @pos: Position in the list.
 *
 * Add @object to position @pos of @ptr list of #$field->{type_name} object.
 *
 * If @pos is #${name_lib.upper()}_LIST_APPEND, @object will be inserted at the tail of the list.
 * If @pos is #${name_lib.upper()}_LIST_PREPEND, @object will be inserted at the head of the list.
 */
void ${name_lib.upper()}${short_type_name}set_${field.short_name}(${field.type_name} *object, int pos)
{
        ${name_lib}_${short_type_name}_set_${field.short_name}(_priv, object, pos);
}


/**
 * ${name_lib}_${short_type_name}_new_${field.short_name}:
 * @ptr: pointer to a #${type_name} object.
 * @ret: pointer to an address where to store the created #${field.type_name} object.
 * @pos: position in the list.
 *
 * Create a new #${field.type_name} children of @ptr, and add it to position @pos of
 * \@ptr list of #${field.type_name} object. The created #${field.type_name} object is
 * stored in @ret.
 *
 * If @pos is #${name_lib.upper()}_LIST_APPEND, @object will be inserted at the tail of the list.
 * If @pos is #${name_lib.upper()}_LIST_PREPEND, @object will be inserted at the head of the list.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int ${name_lib.upper()}${short_type_name}::new_${field.short_name}(${field.type_name} **ret, int pos)
{
        return ${name_lib}_${short_type_name}_new_${field.short_name}(_priv, ret, pos);
}
</%def>
<%def name="struct_fields(struct)">
% for field in struct.fields_list:
##% if field.listed:
##${struct_field_list(field, struct.short_type_name, struct.type_name)}s
% if isinstance(field, Union):
${struct_field_union(field, struct.short_type_name, struct.type_name)}
% elif field.data_type != DataType.UNION and not field.listed:
${structFieldNormal(field, struct.short_type_name, struct.type_name)}
% endif
% endfor
</%def>
<%def name="generateFile(obj_list)">
${header()}
% for obj in obj_list:
    % if isinstance(obj, Struct):
    ${struct_func(obj)}
## % elif isinstance(obj, EnumField):
## ${enums(obj)}
    % endif
% endfor
${footer()}
</%def>
${generateFile(lib_classes.obj_list)}
