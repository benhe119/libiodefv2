
         UNION(${union.type_name}, type) {
% for member in union.list_member:
                    ${member.get_description()}
% endfor
        } ${union.name};
