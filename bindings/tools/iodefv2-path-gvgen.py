#!/usr/bin/python
#
# Graph IODEFV2 Messages
#


import signal
import time
import sys
sys.path.append('.')
sys.path.append('..')
sys.path.append('./.libs')
sys.path.append('../.libs')

import gvgen

try:
       import LibIodefEasy
except Exception,e:
       print "Import failed: ",e
       print "Try 'cd ./.libs && ln -s libiodefv2_python.so _LibIodefEasy.so'"
       sys.exit(1)



client = LibIodefEasy.Client("LibIodefGvGen")
client.Init()

client.PoolInit("localhost", 1)


sourceslist = {}
targetslist = {}
alertslist = {}

graph = gvgen.GvGen()

def sighandler(sig, frame):
        graph.dot()
        sys.exit(0)

signal.signal(signal.SIGTERM, sighandler)
signal.signal(signal.SIGINT, sighandler)



def handle_alert(iodefv2):

        source = iodefv2.Get("alert.source(0).node.address(0).address")
        if not sourceslist.has_key(source):
                sourceitem = graph.newItem(source)
                sourceslist[source] = sourceitem
        target = iodefv2.Get("alert.target(0).node.address(0).address")
        if not targetslist.has_key(target):
                targetitem = graph.newItem(target)
                targetslist[target] = targetitem
        alert  = iodefv2.Get("alert.classification.text")
        alert = alert.replace(' ','_')
        if not alertslist.has_key(alert):
                alertitem = graph.newItem(alert)
                alertslist[alert] = alertitem

                severity = iodefv2.Get("alert.assessment.impact.severity")
                if severity:
                        if severity == "high":
                                graph.propertyAppend(alertslist[alert],"color","red")
                                graph.propertyAppend(alertslist[alert],"style","filled")
                        if severity == "medium":
                                graph.propertyAppend(alertslist[alert],"color","orange")
                                graph.propertyAppend(alertslist[alert],"style","filled")
                        if severity == "low":
                                graph.propertyAppend(alertslist[alert],"color","green")
                                graph.propertyAppend(alertslist[alert],"style","filled")
                        if severity == "info":
                                graph.propertyAppend(alertslist[alert],"color","blue")
                                graph.propertyAppend(alertslist[alert],"style","filled")


        graph.newLink(sourceslist[source],alertslist[alert])
        graph.newLink(alertslist[alert],targetslist[target])


while 1:
       iodefv2 = client.ReadIODEFV2(1)
       if iodefv2:
              handle_alert(iodefv2)

       time.sleep(2)

