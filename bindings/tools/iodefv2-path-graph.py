#!/usr/bin/python
#
# Graph IODEFV2 Messages
#


import time
import sys
sys.path.append('.')
sys.path.append('./.libs')

import gvgen

try:
       import LibIodefEasy
except Exception,e:
       print "Import failed: ",e
       print "Try 'cd ./.libs && ln -s libiodefv2_python.so _LibIodefEasy.so'"
       sys.exit(1)


client = LibIodefEasy.Client("PoolingTest")
client.Init()

client.PoolInit("192.168.33.215", 1)

def handle_alert(iodefv2):
       graph = gvgen.GvGen()
       alert_g = graph.newItem("IODEFV2 Alert")

       value = iodefv2.Get("alert.classification.text")
       if value:
              act = graph.newItem("alert.classification.text", None, distinct=1)
              actc = graph.newItem(value, act, distinct=1)
              graph.newLink(alert_g, actc) 

       value = iodefv2.Get("alert.assessment.impact.description")
       if value:
              aaid = graph.newItem("alert.assessment.impact.description", None, distinct=1)
              aaidc = graph.newItem(value, aaid, distinct=1)
              graph.newLink(alert_g, aaidc)

       value = iodefv2.Get("alert.assessment.impact.completion")
       if value:
              aaic = graph.newItem("alert.assessment.impact.completion", None, distinct=1)
              aaicc = graph.newItem(value, aaic, distinct=1)
              graph.newLink(alert_g, aaicc)

       graph.dot()


while 1:
       iodefv2 = client.ReadIODEFV2(1)
       if iodefv2:
              handle_alert(iodefv2)

       time.sleep(2)

