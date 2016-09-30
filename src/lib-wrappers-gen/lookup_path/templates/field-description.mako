<%!
import sys
sys.path.append('../')
from field import Union, DataType
%>
<% ptr = "*" if field.ptr else "" %>
% if field.is_op_int:
         OPTIONAL_INT(${field.type_name}, ${ptr}${field.name});
% elif field.required and field.unique and (field.data_type == DataType.STRUCT or field.data_type == DataType.PRIMITIVE_STRUCT):
        REQUIRED(${field.type_name}, ${ptr}${field.name});
% elif field.listed and field.key_listed:
        KEYLISTED_OBJECT(${field.name}, ${field.type_name});
% elif field.listed:
        LISTED_OBJECT(${field.name}, ${field.type_name});
% elif field.is_key_listed:
        IS_KEY_LISTED(${field.name});
% elif isinstance(field, Union):
${field.get_field_desc}
% else:
         ${field.type_name} ${ptr}${field.name};
% endif
