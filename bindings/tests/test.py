#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import iodefv2

iodefv2 = iodefv2.IODEFV2()

print("*** IODEFV2->Set() ***")
iodefv2.set("incident(0).event_data(0).description", "My description")
iodefv2.set("incident(0).event_data(0).flow(0).system(0).node.address(0).ext_category", "s0a0")
iodefv2.set("incident(0).event_data(0).flow(0).system(0).node.address(1).ext_category", "s0a1")
iodefv2.set("incident(0).event_data(1).flow(0).system(0).node.address(0).ext_category", "s1a0")
iodefv2.set("incident(0).event_data(1).flow(0).system(0).node.address(1).ext_category", "s1a1")
iodefv2.set("incident(0).event_data(1).flow(0).system(0).node.address(2).ext_category", None)
iodefv2.set("incident(0).event_data(1).flow(0).system(0).node.address(3).ext_category", "s1a3")
print(iodefv2)

print("\n*** Value IODEFV2->Get() ***")
print(iodefv2.get("incident(0).event_data(0).description"))

print ("\n*** Listed Value IODEFV2->Get() ***")
print (iodefv2.get("incident(0).event_data(*).flow(0).system(0).node.address(*).ext_category"))

print ("\n*** Object IODEFV2->Get() ***")
print (iodefv2.get("incident(0).event_data(0).flow(0).system(0).node.address(0)"))

print ("\n*** Listed Object IODEFV2->Get() ***")
print (iodefv2.get("incident(0).event_data(*).flow(0).system(0).node.address(*)"))


