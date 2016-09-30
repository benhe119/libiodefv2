#!/usr/bin/python

import sys
import LibIodefEasy

iodefv2 = LibIodefEasy.IODEFV2()
iodefv2.Set("alert.classification.text", "Bar")

client = LibIodefEasy.ClientEasy("MyTest")
client << iodefv2

