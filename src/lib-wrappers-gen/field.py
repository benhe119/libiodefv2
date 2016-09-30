# coding: utf-8

import re
import os
from enum import Enum

from mako.template import Template


class Names(object):
    def __init__(self, name_lib, name, type=None):
        self.name_lib = name_lib.lower()
        self.name = name.replace(self.name_lib.upper() + "-", "")
        self.type = type

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def lower_name(self):
        """
            >>> print Names(name_lib='idmef', name='nameOfField', type=None).lower_name
            name_of_field
        """
        word = re.sub(r"([A-Z]+)([A-Z][a-z])", r'\1_\2', self.name)
        word = re.sub(r"([a-z\d])([A-Z])", r'\1_\2', word)
        word = word.replace("-", "_")
        return word.lower()

    @property
    def lower_type(self):
        """
            >>> print Names(name_lib='idmef', name='nameOfField', type=None).lower_name
            name_of_field
        """
        type = self.type if self.type != None else self.name
        word = re.sub(r"([A-Z]+)([A-Z][a-z])", r'\1_\2', type)
        word = re.sub(r"([a-z\d])([A-Z])", r'\1_\2', word)
        word = word.replace("-", "_")
        return word.lower()

    @property
    def human_name(self):
        word = self.name.replace('_', ' ') if self.type == None else self.type.replace('_', ' ')
        word = re.sub(r"(?i)([a-z\d]*)", lambda m: m.group(1).lower(), word)
        word = re.sub(r"^\w", lambda m: m.group(0).upper(), word)
        return word

    @property
    def upper_name(self):
        """
            >>> print Names(name_lib='idmef', name='nameOfField', type=None).upper_name
            NAME_OF_FIELD
        """
        return self.lower_name.upper()

    @property
    def type_name(self):
        """
            >>> print Names(name_lib='idmef', name='nameOfField', type=None).type_name
            idmef_name_of_field_t
        """
        return "_".join((self.name_lib, self.lower_type, "t"))

    @property
    def short_type_name(self):
        """
            >>> print Names(name_lib='idmef', name='nameOfField', type=None).short_type_name
            name_of_field
        """
        return self.lower_name

    @property
    def primitive_type_name(self):
        """
            >>> print Names(name_lib='idmef', name='nameOfField', type="uint32_t").primitive_type_name
            uint32_t
        """
        return self.type

    @property
    def primitive_short_type_name(self):
        """
            >>> Names(name_lib='idmef', name='nameOfField', type="uint32_t").primitive_short_type_name
            'uint32'
        """
        return "".join(self.primitive_type_name.rsplit("_t", 1))

    @property
    def primitive_value_type(self):
        """
            >>> print Names(name_lib='idmef',
            ...    name='nameOfField',
            ...    type="uint32_t").primitive_value_type
            uint32
        """
        if self.primitive_short_type_name.startswith(self.name_lib + "_"):
            return self.primitive_short_type_name.replace(self.name_lib + "_", "")
        elif self.primitive_short_type_name.startswith(self.prefix + "_"):
            return self.primitive_short_type_name.replace(self.prefix + "_", "")

        return self.primitive_short_type_name


class Multiplicity(tuple):
    @classmethod
    def get(cls, json_multiplicity):
        normalized_mult = {
            "1": ('1', '1'),
            "Required": ('1', '1'),
            "0..*": ('0', '*'),
            "1..*": ('1', '*'),
            "Optional": ('0', '1'),
            "0..1": ('0', '1')
        }.get(json_multiplicity, json_multiplicity)
        return cls(normalized_mult)

    @property
    def required(self):
        return self[0] == "1"

    @property
    def optional(self):
        return self[0] == "0"

    @property
    def listed(self):
        return self[1] == "*"

    @property
    def unique(self):
        return self[1] == "1"


class DataType(Enum):
    PRIMITIVE = 1
    STRUCT = 2
    PRIMITIVE_STRUCT = 3
    ENUM = 4
    UNION = 5


class Type(Enum):

    STRING = {"c_type": "string", "data_type": DataType.PRIMITIVE_STRUCT}
    DATETIME = {"c_type": "time", "data_type": DataType.PRIMITIVE_STRUCT}
    DATA = {"c_type": "data", "data_type": DataType.PRIMITIVE_STRUCT}
    INTEGER = {"c_type": "uint32_t", "data_type": DataType.PRIMITIVE}
    UMICRO_INT = {"c_type": "uint16_t", "data_type": DataType.PRIMITIVE}
    UTINY_INT = {"c_type": "uint8_t", "data_type": DataType.PRIMITIVE}
    BYTE = {"c_type": "uint8_t", "data_type": DataType.PRIMITIVE}
    CHARACTER = {"c_type": "char", "data_type": DataType.PRIMITIVE}
    INT = {"c_type": "int32_t", "data_type": DataType.PRIMITIVE}
    REAL = {"c_type": "float", "data_type": DataType.PRIMITIVE}
    UBIG_INT = {"c_type": "uint64_t", "data_type": DataType.PRIMITIVE}
    STRUCT = {"c_type": None, "data_type": DataType.STRUCT}
    ENUM = {"c_type": "enum", "data_type": DataType.ENUM}
    UNION = {"c_type": "union", "data_type": DataType.UNION}

    @classmethod
    def get(cls, value):
        return {
            "DATETIME": cls.DATETIME,
            "BYTE[]": cls.DATA,
            "STRING": cls.STRING,
            "ID": cls.STRING,
            "IDREF": cls.STRING,
            "PHONE": cls.STRING,
            "EMAIL": cls.STRING,
            "POSTAL": cls.STRING,
            "PORTLIST": cls.STRING,
            "ML_STRING": cls.STRING,
            "URL": cls.STRING,
            "TIMEZONE": cls.STRING,
            "INTEGER": cls.INTEGER,
            "UMICRO_INT": cls.UMICRO_INT,
            "UTINY_INT": cls.UTINY_INT,
            "BYTE": cls.BYTE,
            "CHARACTER": cls.CHARACTER,
            "INT": cls.INT,
            "REAL": cls.REAL,
            "UBIG_INT": cls.UBIG_INT,
            "NONE": cls.STRUCT,
            "ADDITIONALDATA": cls.STRUCT,
            "SOFTWARE": cls.STRUCT,
            "ENUM": cls.ENUM,
            "UNION": cls.UNION
        }.get(value)


class Field(object):
    def __init__(self, name_lib="iodef", name_class="ClassName", type="typeOnJson", json_name="NameField", multiplicity=None, id=0, values=None, key_listed=False, enum_id=0, doc=None):
        self.name_lib = name_lib
        self.name_class = name_class
        self.type = type
        self.json_name = json_name
        self.multiplicity = multiplicity
        self.id = int(id)
        self.values = values if values else []
        self.key_listed = key_listed
        self.enum_id = enum_id
        self.doc = doc

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def data_type(self):
        #~ print str(self.type).upper()
        return Type.get(str(self.type).upper()).value.get("data_type")

    @property
    def c_type(self):
        pre_type = Type.get(str(self.type).upper()).value.get("c_type")
        return {
                    "string": self.prefix + "_string_t",
                    "data": self.name_lib + "_data_t",
                    "time": self.name_lib + "_time_t"
                }.get(pre_type, pre_type)

    @property
    def ptr(self):
        return self.data_type != DataType.PRIMITIVE and self.data_type != DataType.ENUM

    @property
    def names(self):
        if self.data_type == DataType.PRIMITIVE or DataType.PRIMITIVE_STRUCT:
            return Names(self.name_lib, self.json_name, self.c_type)

        return Names(self.name_lib, self.json_name, self.type)

    @property
    def required(self):
        return self.multiplicity.required

    @property
    def optional(self):
        return self.multiplicity.optional

    @property
    def unique(self):
        return self.multiplicity.unique

    @property
    def listed(self):
        return self.multiplicity is not None and self.multiplicity.listed and self.type == "None"

    @property
    def short_type_name(self):
        """
            :Example:

            >>> #
            >>> Field(name_lib="idmef", name_class="ClassName",
            ...    type=None, json_name="nameOfField").short_type_name
            'name_of_field'
        """
        if self.data_type == DataType.PRIMITIVE or self.data_type == DataType.PRIMITIVE_STRUCT:
            return self.names.primitive_short_type_name
        elif self.type == "ENUM":
            return Names(self.name_lib, self.name_class).lower_name + "_" + Names(self.name_lib, self.json_name).lower_name

        return self.names.short_type_name

        #~ return "".join(self.type_name.replace(self.name_lib + "_", "").rsplit("_t", 1))


    @property
    def type_name(self):
        """
            >>> Field(name_lib="idmef", name_class="ClassName", type=None, json_name="nameOfField").type_name
            'idmef_name_of_field_t'
        """
        if self.data_type == DataType.PRIMITIVE or self.data_type == DataType.PRIMITIVE_STRUCT:
            return self.names.primitive_type_name
        if self.data_type == DataType.ENUM:
            return "_".join((self.name_lib, Names(self.name_lib, self.name_class).lower_name, self.names.lower_name, "t"))

        return self.names.type_name

    @property
    def name(self):
        """
            >>> print Field(name_lib="idmef",
            ...    name_class="ClassName", type=None,
            ...    json_name="nameOfField").name
            name_of_field
        """
        if self.listed:
            return self.names.short_type_name + "_list"

        return self.names.lower_name

    @property
    def short_name(self):
        """
            >>> Field(name_lib="idmef",
            ...    name_class="ClassName", type=None,
            ...    json_name="nameOfField").short_name
            'name_of_field'
        """
        if self.listed:
            return self.name.replace("_list", "")

        return self.name

    @property
    def value_type(self):
        """
            >>> Field(name_lib="idmefv2", name_class="ClassName",
            ...    type="STRING", json_name="nameOfField").value_type
            'string'
        """
        return self.names.primitive_value_type

    @property
    def is_op_int(self):
        """
            >>> print Field(name_lib="idmef", name_class="AdditionalData",
            ...    type="STRING", json_name="nameOfField",
            ...    multiplicity=Multiplicity(('0', '1'))).is_op_int
            False
        """
        if self.name_lib == "iodef":
            return self.multiplicity.optional and self.data_type == DataType.PRIMITIVE

        return self.multiplicity.optional and (
            self.data_type == DataType.PRIMITIVE or (
                self.data_type == DataType.ENUM and all('0' != member["rank"] for member in self.values)))

    @property
    def is_key_listed_object(self):
        """
            >>> print Field(name_lib="idmef", name_class="AdditionalData",
            ...    type="STRING",json_name="nameOfField",
            ...    multiplicity=Multiplicity(('0', '1'))).is_key_listed_object
            0
        """
        return self.is_listed_object and self.key_listed

    @property
    def is_listed_object(self):
        """
            >>> Field(name_lib="idmef", name_class="AdditionalDta",
            ...    type="STRING", json_name="nameOfField",
            ...    multiplicity=Multiplicity(('0', '1'))).is_listed_object
            0
        """
        return 1 if self.listed and self.data_type == DataType.STRUCT else 0

    @property
    def is_key_listed(self):
        """
            >>> Field(name_lib="idmef", name_class="AdditionalDta", type="STRING",
            ...    json_name="nameOfField",
            ...    multiplicity=Multiplicity(('0', '1'))).is_key_listed
            0
        """
        return self.key_listed and self.type_name == self.prefix + "_string_t"

    def is_struct(self):
        """
            >>> Field(name_lib="idmef", name_class="AdditionalDta",
            ...    type="STRING", json_name="nameOfField",
            ...    multiplicity=Multiplicity(('0', '1'))).is_struct()
            False
        """
        return self.data_type == DataType.STRUCT

    def is_enum(self):
        """
            >>> Field(name_lib="idmef", name_class="AdditionalDta",
            ...    type="STRING", json_name="nameOfField",
            ...    multiplicity=Multiplicity(('0', '1'))).is_enum()
            False
        """
        return self.data_type == DataType.ENUM

    @classmethod
    def field_from_dict(cls, name_lib, name_class, dict_field):
        multiplicity = Multiplicity.get(str(dict_field.get("multiplicity")))
        return cls(name_lib, name_class, type=str(dict_field.get("type")),
                   json_name=str(dict_field.get("name")),
                   multiplicity=multiplicity, id=int(dict_field.get("id", 5)),
                   values=dict_field.get("values", None),
                   key_listed=dict_field.get("key_listed", False),
                   enum_id=int(dict_field.get("ident", -2)),
                   doc=dict_field.get("description", ""))

    def is_pre_declared(self, structs):
        return self.type_name not in structs.keys() and self.json_name != self.name_class

    @property
    def desc(self):
        template = Template(filename="lookup_path/templates/field-description.mako")
        return os.linesep.join([line for line in template.render(field=self).encode('utf8').splitlines() if line])


class Child(object):
    def __init__(self, name_lib, name_class, dict_child):
        self.name_lib = name_lib
        self.name_class = name_class
        self.type = None
        self.json_name = dict_child.get("name")
        self.id = int(dict_child.get("id", 5))
        self.ptr = True
        self.data_type = DataType.STRUCT
        self.multiplicity = Multiplicity.get("0..1")
        self.name = Names(self.name_lib, self.json_name).lower_name
        self.data_type = DataType.STRUCT

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def names(self):
        return Names(self.name_lib, self.json_name)

    @property
    def names_class(self):
        return Names(self.name_lib, self.name_class)

    @property
    def type_name(self):
        """
            >>> Child(name_lib="idmef", name_class="Service",
            ...    dict_child=dict(name='serviceWeb')).type_name
            'idmef_service_web_t'
        """
        if self.data_type == DataType.ENUM:
            "_".join(self.name_lib, self.names_class.lower_name, self.names.lower_name, "t")
        return self.names.type_name

    @property
    def short_type_name(self):
        """
            >>> Child(name_lib="idmef", name_class="Service",
            ...    dict_child={'name': 'serviceWeb'}).short_type_name
            'service_web'
        """
        return self.names.short_type_name

    @property
    def value(self):
        """
            >>> Child(name_lib="idmef",
            ...    name_class="Service",
            ...    dict_child={'name': 'serviceWeb'}).value
            'IDMEF_WEB_TYPE_SERVICE'
        """
        if "_" in self.names.short_type_name:
            tab = self.names.short_type_name.split("_")
            return "_".join((self.name_lib, tab[1], "type", tab[0])).upper()

        return "_".join((self.names_class.type_name[:-2], "type", self.names.short_type_name)).upper()

    @property
    def listed(self):
        return False

    def get_description(self):
        template = Template(filename="lookup_path/templates/union-member-description.mako")
        return os.linesep.join([line for line in template.render(member=self).encode('utf8').splitlines() if line])


class EnumMember(object):
    def __init__(self, name_lib, name_class, name_enum, dic_enum_field, text=None):
        self.name_lib = name_lib
        self.name_class = name_class
        self.name_enum = str(name_enum)
        self.keyword = str(dic_enum_field["keyword"])
        self.dtype = dic_enum_field.get("dtype", None)
        self.value = int(dic_enum_field["rank"])
        self.id = int(dic_enum_field["rank"])
        self.text = text

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def name(self):
        """
            >>> EnumMember(
            ...     name_lib="idmef",
            ...     name_class="Service",
            ...     name_enum="category",
            ...     dic_enum_field={'rank': '1', 'keyword':'nameOfMember'}
            ... ).name
            'IDMEF_SERVICE_CATEGORY_NAME_OF_MEMBER'
        """
        name = ("_".join((self.name_lib, Names(self.name_lib, self.name_class).lower_name,
                          Names(self.name_lib, self.name_enum).lower_name, Names(self.name_lib,
                                                                                 self.keyword).lower_name))).upper()
        return name

    @property
    def short_name(self):
        return Names(self.name_lib, self.keyword).lower_name

    def enum_member_desc(self):
        template = Template(filename="lookup_path/templates/enum-member-description.mako")
        return os.linesep.join([line for line in template.render(member=self).encode('utf8').splitlines() if line])


class Union(Field):
    union_names = {"Service": "specific","IDMEFV2-Message": "message",
                   "IDMEF-Message": "message", "Alert": "detail"}

    def __init__(self, name_lib, name_class, list_member=None, description="", id_class=0):
        self.name_lib = name_lib
        self.name_class = name_class
        self.type = "UNION"
        self.json_name = ""
        self.list_member = list_member
        self.description = description
        self.key_listed = False
        self.id_class = id_class

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def enum(self):
        return EnumField.enum_from_union(self.name_lib, self.name_class, self.list_member, self.id_class-1, self.id_class)

    @property
    def multiplicity(self):
        return Multiplicity.get(('0', '1'))

    @property
    def name(self):
        """
            >>> Union(name_lib="idmef", name_class="Service").name
            'specific'
        """
        return Union.union_names.get(self.name_class, "type")

    @property
    def type_name(self):
        """
            >>> Union(name_lib="idmef", name_class="Service").type_name
            'idmef_service_type_t'
        """
        return "_".join((Names(self.name_lib, self.name_class).type_name[:-2], "type", "t"))

    @property
    def short_type_name(self):
        """
            >>> Union(name_lib="idmef", name_class="Service").short_type_name
            'service_type'
        """
        return (self.type_name.replace(self.name_lib + "_", ""))[:-2]

    @property
    def var(self):
        return "type"

    @property
    def get_field_desc(self):
        template = Template(filename="lookup_path/templates/union-description.mako")
        return os.linesep.join([line for line in template.render(union=self).encode('utf8').splitlines() if line])


class EnumField(Field):
    def __init__(self, name_lib, name_class, type, json_name, multiplicity, id, dict_member_list, enum_id, id_class):
        Field.__init__(self, name_lib, name_class, type, json_name, multiplicity, id)
        self.dict_member_list = dict_member_list
        self.enum_id = enum_id
        self.id_class = id_class

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def data_type(self):
        return DataType.ENUM

    @classmethod
    def enum_from_union(cls, name_lib, name_class, union_member_list, enum_id, id_class):
        (i, dict_member_list) = (1, [])
        dict_member_list.append(dict(keyword="error", rank="-1"))
        if name_class != "IDMEF-Message":
            dict_member_list.append(dict(keyword="default", rank="0"))
        for union_member in union_member_list:
            dict_member_list.append(dict(keyword=union_member.json_name.replace(name_class, ""), rank=str(i)))
            i += 1

        return cls(name_lib, name_class, DataType.ENUM, "type", "required", 0, dict_member_list, enum_id, id_class)

    @property
    def ident(self):
        ident = self.enum_id if self.enum_id != -2 else self.id_class - 1
        return ident

    @property
    def names_enum(self):
        return Names(self.name_lib, self.json_name)

    @property
    def names_class(self):
        return Names(self.name_lib, self.name_class)

    @property
    def type_name(self):
        """
            >>> EnumField(name_lib="idmef", name_class="AdditionalData", type="ENUM",
            ...    json_name="category", multiplicity=Multiplicity.get(('0','1')),
            ...    id=0, dict_member_list=[], enum_id=1, id_class=0).type_name
            'idmef_additional_data_category_t'
        """
        return "_".join((self.name_lib, self.names_class.lower_name, self.names_enum.lower_name, "t"))

    @property
    def short_type_name(self):
        """
            >>> EnumField(name_lib="idmef", name_class="AdditionalData", type="ENUM",
            ...    json_name="category", multiplicity=Multiplicity.get(('0','1')),
            ...    id=0, dict_member_list=[], enum_id=1, id_class=0).short_type_name
            'additional_data_category'
        """
        return self.type_name.replace(self.name_lib + "_", "")[:-2]

    @property
    def value_type(self):
        """
            >>> EnumField(name_lib="idmef", name_class="AdditionalData", type="ENUM",
            ...    json_name="category", multiplicity=Multiplicity.get(('0','1')),
            ...    id=0, dict_member_list=[], enum_id=1, id_class=0).value_type
            'additional_data_category'
        """
        return self.short_type_name

    @property
    def description(self):
        template = Template(filename="lookup_path/templates/enum-description.mako")
        return os.linesep.join([line for line in template.render(enum=self).encode('utf8').splitlines() if line])

    @property
    def enum_members(self):
        member_list = []
        for dict_member in sorted(self.dict_member_list, key=lambda dict_member: int(dict_member["rank"])):
            member_list.append(EnumMember(
                self.name_lib, self.name_class, self.json_name, dict_member))
        return member_list

    @property
    def vtype_members(self):
        map_vtype = {"integer": [], "real": [], "date-time": [], "string": [], "byte[]": [] }
        for member in self.enum_members:
            if member.dtype is not None:
                map_vtype[member.dtype].append(member.name)

        return map_vtype


class PreDeclared(object):
    def __init__(self, name_lib, name, id):
        self.name_lib = name_lib
        self.name = name
        self.ident = int(id)
        self.top_level = 0
        self.refcount = 1
        self.data_type = DataType.STRUCT

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def names(self):
        return Names(self.name_lib, self.name)

    @property
    def type_name(self):
        """
            >>> PreDeclared(name_lib="idmef", name="linkage", id=23).type_name
            'idmef_linkage_t'
        """
        return self.names.type_name

    @property
    def short_type_name(self):
        """
            >>> PreDeclared(name_lib="idmef", name="linkage", id=23).short_type_name
            'linkage'
        """
        return self.names.short_type_name

    @property
    def description(self):
        """
            >>> PreDeclared(name_lib="idmef", name="linkage", id=23).description
            'PRE_DECLARE(idmef_linkage_t, struct);\\n'
        """
        return "PRE_DECLARE(%s, struct);\n" % (self.type_name)


class Struct(object):
    def __init__(self, name_lib="iodef", name="class", doc="", attributes=None, children=None, aggregates=None, ident=0):
        self.name_lib = name_lib
        self.name = name
        self.doc = doc
        if attributes is None:
            attributes = []
        self.attributes = attributes
        if children is None:
            children = []
        self.children = children
        if aggregates is None:
            aggregates = []
        self.aggregates = aggregates
        self.struct_listed = 0
        self.is_keyed = 0
        self.ident = ident
        self.refcount = True
        self.top_level = 0

    @property
    def prefix(self):
        return "lib" + self.name_lib

    @property
    def fields(self):
        if self.name == "Checksum":
            return self.aggregates + self.attributes

        return self.attributes + self.aggregates

    @property
    def names(self):
        return Names(self.name_lib, self.name)

    @property
    def type_name(self):
        """
            >>> Struct("idmef", "className").type_name
            'idmef_class_name_t'
        """
        return self.names.type_name

    @property
    def short_type_name(self):
        """
            >>> Struct(name_lib="idmef", name="className").short_type_name
            'class_name'
        """
        return self.names.short_type_name

    def listed(self, lib_data):
        class_dict = lib_data.linked_structs()
        return 1 if class_dict.get(self.type_name, 0) > 0 else 0

    def keyed(self, lib_data):
        class_dict = lib_data.linked_structs()
        return 1 if class_dict.get(self.type_name, 0) > 1 else 0

    @classmethod
    def _get_fields(cls, dict_field_list, name_lib, name_class):
        attributes = []
        for dict_field in sorted(dict_field_list, key=lambda dict_field: int(dict_field.get("id", 0))):
            new_field = Field.field_from_dict(name_lib, name_class, dict_field)
            attributes.append(new_field)

        return attributes

    @classmethod
    def _get_children(cls, dict_child_list, name_lib, name_class):
        member_list = []
        for dict_child in sorted(dict_child_list, key=lambda dict_child: int(dict_child.get("id", 1))):
            member_union = Child(name_lib, name_class, dict_child)
            member_list.append(member_union)

        return member_list

    @classmethod
    def class_from_dict(cls, name_lib, dict_class):
        name_lib = name_lib
        name = str(dict_class["name"])
        doc = str(dict_class["description"])
        attributes = cls._get_fields(
            dict_class["attributes"].values(), name_lib, name)
        aggregates = cls._get_fields(
            dict_class["aggregates"].values(), name_lib, name)
        children = None
        if 'childs' in dict_class.keys():
            children = cls._get_children(
                dict_class["childs"].values(), name_lib, name)
        ident = int(dict_class["id"])
        return cls(name_lib, name, doc, attributes, children, aggregates, ident)

    @property
    def fields_list(self):
        if self.name_lib == "iodef":
            fields_list = sorted(self.fields, key=lambda field: field.id)
        else:
            fields_list = self.fields
        if len(self.children) > 0:
            fields_list.append(Union(self.name_lib, self.name, self.children, self.ident-1, self.ident))

        return fields_list

    @property
    def enum_list(self):
        enums = []
        for field in self.fields_list:
            if (not isinstance(field, Union)) and field.is_enum():
                enums.append(EnumField(field.name_lib, field.name_class, field.type, field.json_name,
                                       field.multiplicity, field.id, field.values, field.enum_id, self.ident))
            if isinstance(field, Union):
                enums.append(field.enum)

        return enums

    def is_pre_dec_struct(self, structs, predeclared_list, obj_list, predeclared):
        for field in self.fields_list:
            if isinstance(field, Union):
                continue
            if field.is_struct():
                if(field.is_pre_declared(structs)):
                    pre_dec = PreDeclared(
                        field.name_lib, field.json_name, self.ident - 1)
                    predeclared_list.append(pre_dec)
                    predeclared[field.type_name] = field
                    obj_list.insert(-2, pre_dec)

        return predeclared_list


class LibData(object):
    def __init__(self, name):
        self.name = name
        self.obj_list = []
        self.structs = {}
        self.struct_list = []
        self.enum_list = []
        self.predeclared_list = []
        self.predeclared = {}

    @property
    def prefix(self):
        return ("lib" + self.name)

    @property
    def type_list(self):
        type_l = []
        for obj in self.obj_list:
            if not isinstance(obj, PreDeclared):
                type_l.append(
                    ((self.name + "_CLASS_ID_" + obj.short_type_name).upper(), obj.ident))

        return type_l

    def linked_structs(self):
        field_dict = {}
        for struct in self.struct_list:
            for field in struct.fields_list:
                if field.type_name not in field_dict.keys() and field.is_listed_object:
                    field_dict[field.type_name] = 1
                    if field.is_key_listed_object:
                        field_dict[field.type_name] = 2

        return field_dict

    def unique_id(self):
        used_id = {}
        objs_by_id = {}
        for obj in self.obj_list:
            if isinstance(obj, PreDeclared):
                continue
            used_id[obj.ident] = False
        for obj in self.obj_list:
            if isinstance(obj, PreDeclared):
                continue
            if not used_id[obj.ident]:
                used_id[obj.ident] = True
                objs_by_id[obj.ident] = obj.type_name
            else:
                print "%s and %s have the same ident.\nCheck the corresponding files on %s_json directory" % (obj.type_name, objs_by_id[obj.ident], self.name)
                return False

        return True

    def check_id(self):
        for obj in self.obj_list:
            print "object %s has ident %s" % (obj.type_name, obj.ident)
        print "length is %d" % (len(self.obj_list))

    def process(self, dict_class_list):
        for dict_class in dict_class_list:
            struct = Struct().class_from_dict(self.name, dict_class)
            struct.is_pre_dec_struct(self.structs, self.predeclared_list, self.obj_list, self.predeclared)
            self.struct_list.append(struct)
            if struct.type_name not in self.structs.keys():
                self.structs[struct.type_name] = struct
                self.obj_list = self.obj_list + struct.enum_list
                self.obj_list.append(struct)
        assert(self.unique_id())
        self.struct_list[-1].top_level = 1
        self.structs[self.struct_list[-1].type_name].top_level = 1
        for obj in self.obj_list:
            if isinstance(obj, Struct):
                obj.struct_listed = obj.listed(self)
                obj.is_keyed = obj.keyed(self)
                if obj.type_name == self.struct_list[-1].type_name:
                    obj.top_level = 1
        for pre_dec in self.predeclared_list:
            print "predeclared is %s" % (pre_dec.type_name)
        return self

if __name__ == "__main__":
    import doctest
    doctest.testmod()
