<%!
import sys
sys.path.append('../')
from field import Struct, Union, DataType, Multiplicity, Field, EnumField, PreDeclared
%>
<%def name="header()">
/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@prelude-ids.com>
* Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
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
 * Template file: generate-tree-wrap.h.mako
 *
 */

#ifndef _${prefix.upper()}_${name_lib.upper()}_TREE_WRAP_H
#define _${prefix.upper()}_${name_lib.upper()}_TREE_WRAP_H

#include <limits.h>
#include "${name_lib}-class.h"
#include "${name_lib}-value.h"
#include "${prefix}-inttypes.h"
#include "${prefix}-string.h"

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef WIN32
# undef interface
#endif

#define ${name_lib.upper()}_LIST_APPEND  INT_MAX
#define ${name_lib.upper()}_LIST_PREPEND (INT_MAX - 1)
</%def>
<%def name="footer(type_list)">
##void ${name_lib}_message_set_pmsg(${name_lib}_message_t *message, ${prefix}_msg_t *msg);

##${prefix}_msg_t *${name_lib}_message_get_pmsg(${name_lib}_message_t *message);

int _${name_lib}_additional_data_type_is_set(${name_lib}_additional_data_t *ad);

#ifdef __cplusplus
  }
#endif

% for (short_type_name, ident) in type_list:
#define ${short_type_name} ${ident}
% endfor
#endif /*_${prefix.upper()}_${name_lib.upper()}_TREE_WRAP */
</%def>
<%def name="structDefinition(predeclaredsKeys, type_name, short_type_name, struct)">
<%include file="struct-description.h.mako" args="struct=struct"/>
% if typ not in predeclaredsKeys:
typedef struct ${name_lib}_${short_type_name} ${type_name};
% endif
</%def>
<%def name="structConstructor(short_type_name, type_name)">
int ${name_lib}_${short_type_name}_new(${type_name} **ret);
int ${name_lib}_${short_type_name}_copy(const ${type_name} *src, ${type_name} *dst);
int ${name_lib}_${short_type_name}_clone(${type_name} *src, ${type_name} **dst);
int ${name_lib}_${short_type_name}_compare(const ${type_name} *obj1, const ${type_name} *obj2);
</%def>
<%def name="structRef(short_type_name, type_name)">
${type_name} *${name_lib}_${short_type_name}_ref(${type_name} *${short_type_name});
</%def>
<%def name="structDestroyChild(short_type_name)">
int _${name_lib}_${short_type_name}_destroy_child(void *p, ${name_lib}_class_child_id_t child, int n);
</%def>
<%def name="structGetChild(short_type_name)">
int _${name_lib}_${short_type_name}_get_child(void *p, ${name_lib}_class_child_id_t child, void **childptr);
</%def>
<%def name="structNewChild(short_type_name)">
int _${name_lib}_${short_type_name}_new_child(void *p, ${name_lib}_class_child_id_t child, int n, void **ret);
</%def>
<%def name="structDestroy(short_type_name, type_name)">
void ${name_lib}_${short_type_name}_destroy(${type_name} *ptr);
</%def>
<%def name="structFieldNormal(field_type, short_type_name, type_name,fTypeName, field_name, name, is_op_int)">
<%
ptr = "" ; refer = "";
if name == None :
    name=field_name
if field_type == DataType.STRUCT or field_type == DataType.PRIMITIVE_STRUCT:
    ptr = "*"; refer="*";
%>
%if is_op_int:
<% refer = "*" %>void ${name_lib}_${short_type_name}_unset_${name}(${type_name} *ptr);
%endif
${fTypeName} ${refer}${name_lib}_${short_type_name}_get_${name}(${type_name} *ptr);
<%
if name == "class":
    fieldName = "class_str"
elif name == "virtual":
    fieldName = "virtual_sys"
elif name == "operator":
    fieldName = "operator_indic"
else:
    fieldName = name
%>
void ${name_lib}_${short_type_name}_set_${name}(${type_name} *ptr, ${fTypeName} ${ptr}${fieldName});
int ${name_lib}_${short_type_name}_new_${name}(${type_name} *ptr, ${fTypeName} **ret);
</%def>
<%def name="structFieldUnion(short_type_name, type_name,fTypeName, fName, fMemberList, var)">
${fTypeName} ${name_lib}_${short_type_name}_get_${var}(${type_name} *ptr);
% for member in fMemberList:
${structFieldNormal(member.data_type, short_type_name, type_name, member.type_name, member.short_type_name, member.short_type_name, is_op_int=False)}
% endfor
</%def>
<%def name="structFieldList(fTypeName, short_type_name, fShortName, type_name, fShortTypeName)">
${fTypeName} *${name_lib}_${short_type_name}_get_next_${fShortName}(${type_name} *${short_type_name}, ${fTypeName} *${fShortTypeName}_cur);
void ${name_lib}_${short_type_name}_set_${fShortName}(${type_name} *ptr, ${fTypeName} *object, int pos);
int ${name_lib}_${short_type_name}_new_${fShortName}(${type_name} *ptr, ${fTypeName} **ret, int pos);
</%def>
<%def name="structFields(fieldList, short_type_name, type_name)">
%for field in fieldList:
%if field.listed:
${structFieldList(field.type_name, short_type_name, field.short_name, type_name, field.short_type_name)}
%elif field.data_type == DataType.UNION:
${structFieldUnion(short_type_name, type_name, field.type_name, field.name, field.list_member, field.var)}
%else:
${structFieldNormal(field.data_type, short_type_name, type_name, field.type_name, field.name, field.name, field.is_op_int)}
%endif
%endfor
</%def>
<%def name="struct(short_type_name, type_name, fields_list, predeclaredsKeys, struct)">
${structDefinition(predeclaredsKeys, type_name, short_type_name, struct)}
${structConstructor(short_type_name, type_name)}
${structRef(short_type_name, type_name)}
#ifndef SWIG
${structGetChild(short_type_name)}
${structNewChild(short_type_name)}
${structDestroyChild(short_type_name)}
#endif
${structDestroy(short_type_name, type_name)}
${structFields(fields_list, short_type_name, type_name)}
</%def>
<%def name="enum(eTypeName, eShortTypeName, description)">
${description}
${eTypeName} ${name_lib}_${eShortTypeName}_to_numeric(const char *name);
const char *${name_lib}_${eShortTypeName}_to_string(${eTypeName} val);
</%def>
<%def name="preDeclared(type_name, short_type_name, type)">
%if type == type.STRUCT:
typedef struct ${name_lib}_${short_type_name} ${type_name};
% else:
typedef enum ${name_lib}_${short_type_name} ${type_name};
%endif
</%def>
<%def name="generateFile(lib_classes)">
${header()}
% for obj in lib_classes.obj_list:
    % if isinstance(obj, Struct):
        ${struct(obj.short_type_name, obj.type_name, obj.fields_list, lib_classes.predeclared.keys(), obj)}
    % elif isinstance(obj, EnumField):
        ${enum(obj.type_name, obj.short_type_name, obj.description)}
    % elif isinstance(obj, PreDeclared):
        ${preDeclared(obj.type_name, obj.short_type_name, obj.data_type)}
    %endif
%endfor
${footer(lib_classes.type_list)}
</%def>
${generateFile(lib_classes)}
