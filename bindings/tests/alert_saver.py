#!/usr/bin/python

import os
import sys
import LibIodefEasy


##################
# this program will save all alerts to the "alerts" directory.
#
# known bu^Wfeatures: directory must exist, and old alerts are overwritten if present
##################

i = 0
dest_dir = "alerts"
if len(sys.argv) > 1:
	dest_dir = sys.argv[1]


if os.path.exists(dest_dir) == 0:
	print "dir ",dest_dir," does not exist"
	sys.exit(1)


client = LibIodefEasy.ClientEasy("PolluxTest", LibIodefEasy.Client.IODEFV2_READ)
client.SetFlags(client.GetFlags() & ~LibIodefEasy.Client.HEARTBEAT)
#client.Init()
client.Start()

#ret = client.PoolInit("192.168.33.215", 1)
#print "PoolInit =>",ret


def handle_alert(iodefv2):
	global i
	try:
		print iodefv2
		i += 1
		f = open( "%s/%d.iodefv2" % (dest_dir,i), "w" )
		iodefv2 >> f
		f.close()
		print i
	except RuntimeError,_e:
		print "An exception occured: ",_e


import time

while 1:
	sys.stdout.write(".")
	sys.stdout.flush()
	iodefv2 = LibIodefEasy.IODEFV2()
	client >> iodefv2
	if iodefv2:
		handle_alert(iodefv2)
	time.sleep(2)
