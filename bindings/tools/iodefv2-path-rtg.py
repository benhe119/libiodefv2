#!/usr/bin/python
#
# Graph IODEFV2 Messages
#

import time
import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import rtg_cli 
except:
	print "Cannot import rtg_cli"

try:
       import LibIodefEasy
except:
       print "Import failed"
       print "Try 'cd ./.libs && ln -s libiodefv2_python.so _LibIodefEasy.so'"
       sys.exit(1)

#
# GD Constants
#

client = LibIodefEasy.Client("LibIodefRTG")
client.Init()

client.PoolInit("192.168.33.215", 1)

#
# 10000 could be considered as the maximum, since
# it would cover already a big classification.text
#
def unique_alert_number(ClassificationText):
       number = 0

       for c in ClassificationText:
              number += ord(c)

       return number


def handle_alert(iodefv2, rtg):      

	classificationtext = iodefv2.Get("alert.classification.text")
	classificationtext = classificationtext.replace(' ','_')


	severity = iodefv2.Get("alert.assessment.impact.severity")
	if severity:
		if severity == "high":
			if classificationtext:
				edge_args = classificationtext + " " + "attacks"
				try:
					rtg.do_edge(edge_args)
				except:
					print "Cannot add the edge (already added?)"
				update_args = classificationtext + " color=(1,0,0)"  
				rtg.do_update(update_args)

		if severity == "medium":
			if classificationtext:
				edge_args = classificationtext + " " + "attacks"
				try:
					rtg.do_edge(edge_args)
				except:
					print "Cannot add the edge (already added?)"
				update_args = classificationtext + " color=(1,0.5,0)"
				rtg.do_update(update_args)

		if severity == "low":
			if classificationtext:
				edge_args = classificationtext + " " + "attacks"
				try:
					rtg.do_edge(edge_args)
				except:
					print "Cannot add the edge (already added?)"
				update_args = classificationtext + " color=(0,1,0)"
				rtg.do_update(update_args)

		if severity == "info":
			if classificationtext:
				edge_args = classificationtext + " " + "attacks"
				try:
					rtg.do_edge(edge_args)
				except:
					print "Cannot add the edge (already added?)"
				update_args = classificationtext + " color=(0,0,1)"
				rtg.do_update(update_args)

if __name__ == "__main__":

	rtg = rtg_cli.Interp()	

	while 1:
		iodefv2 = client.ReadIODEFV2(1)
		if iodefv2:
			handle_alert(iodefv2, rtg)

		time.sleep(1)

