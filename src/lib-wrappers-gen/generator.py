#!/usr/bin/env python
# coding: utf-8

from __future__ import print_function
import yaml

from mako.template import Template
from mako.lookup import TemplateLookup

from file_loader import LoadFile
from field import LibData


GENERATOR_VERSION = 1
LIB_VERSION = {
    "idmef": "RFC4765",
    "idmefv2": "Not released yet",
    "iodef": "RFC5070",
    "iodefv2": "RFC5070-bis-26",
}


class Generator(object):

    def __init__(self, name_lib, template_name, dst, lookup_path):
        self.name_lib = name_lib
        self.template_name = template_name
        self.dst = dst
        self.lookup_path = lookup_path

    @property
    def jsons_path(self):
        return "idmef_json" if self.name_lib == "idmef" else\
               "idmefv2_json" if self.name_lib == "idmefv2" else "iodefv2_json"

    @property
    def top_class(self):
        return "document" if self.name_lib == "iodefv2" else "message"

    def get_render(self, **kwargs):
        with open(self.dst, 'w') as dst_fd:
            print("Generating {0} from {1} ...".format(self.dst, self.template_name), end="")

            mylookup = TemplateLookup(directories=self.lookup_path)
            template = Template(filename=self.template_name, lookup=mylookup)
            dst_fd.write(template.render(**kwargs).encode('utf8'))

            print("OK!")

if __name__ == '__main__':
    with open('conf.yml', 'r') as conf_fd:
        src_dst_list = yaml.load(conf_fd)

    previous_schema = ""

    for src_dst in src_dst_list:
        generator = Generator(src_dst["schema"], src_dst["src"], src_dst["dst"], "lookup_path")

        if previous_schema != src_dst["schema"]:
            lib_classes = LibData(src_dst["schema"])
            dict_class_list = LoadFile(generator.jsons_path, lib_classes.name).get_dict_class_list()
            lib_classes.process(dict_class_list)
            previous_schema = src_dst["schema"]

        generator.get_render(lib_classes=lib_classes, name_lib=lib_classes.name,
                             prefix=lib_classes.prefix, top_class=generator.top_class,
                             generator_version=GENERATOR_VERSION,
                             lib_version=LIB_VERSION[src_dst["schema"]])
