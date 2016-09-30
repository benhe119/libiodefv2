# coding: utf-8
import os

import json
import yaml


class LoadFile(object):

    def __init__(self, path, name_lib):
        self.loadfunctions = {'.json': self.load_json, '.yml': self.load_yaml}
        self.path = path
        self.name_lib = name_lib

    def file_extension(self, file_name):
        return os.path.splitext(file_name)[-1]

    def string_content(self, file_name):
        with open(file_name, 'r') as file_descriptor:
            return file_descriptor.read()

    def load_json(self, content):
        return json.loads(content)

    def load_yaml(self, content):
        yaml.load(content)

    def get_dict_class_list(self):
        dict_list = []

        for file_name in os.listdir(self.path):
            if self.file_extension(file_name) in self.loadfunctions:
                dict_list.append(self.loadfunctions[self.file_extension(file_name)](self.string_content(os.path.join(self.path, file_name))))

        sort = sorted(dict_list, key=lambda dict_class: int(dict_class["id"]))

        if self.name_lib == "idmefv2":
            web_service = sort.pop()
            snmp_service = sort.pop()
            transaction = sort.pop()
            sort.insert(13, web_service)
            sort.insert(13, snmp_service)
            sort.insert(23, transaction)
        elif self.name_lib == "idmef":
            chek_sum = sort.pop()
            for classe in sort:
                if classe["name"] == "File":
                    file_index = sort.index(classe)
                if classe["name"] == "Reference":
                    reference_index = sort.index(classe)
            sort.insert(file_index, chek_sum)
            ref_struct = sort.pop(reference_index + 1)
            sort.insert(1, ref_struct)

        return sort
