<%page args="struct"/>
struct ${struct.type_name[:-2]} {

% for field in struct.fields_list:
##% if struct.struct_listed and field.is_keyed:
% if field.is_key_listed:
${field.desc}
% endif
% endfor
% for cnt, field in enumerate(struct.fields_list):
% if cnt == 0:
% if struct.struct_listed and not struct.is_keyed:
         ${struct.name_lib.upper()}_LINKED_OBJECT;
% elif not (struct.struct_listed or struct.is_keyed):
         ${struct.name_lib.upper()}_OBJECT;
% endif
         REFCOUNT;
% if struct.type_name == struct.name_lib + "_additional_data_t":
         IGNORED(${field.prefix}_bool_t, _type_is_set);
% endif
% endif
% if not field.is_key_listed:
${field.desc}
% endif
% if cnt + 1 == len(struct.fields_list):
};
% endif
% endfor
