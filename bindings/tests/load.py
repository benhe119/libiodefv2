#!/usr/bin/python

import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import LibIodefEasy
except Exception,e:
	print "Import failed: ",e
	print "Try 'cd ./.libs && ln -s libiodefv2_python.so _LibIodefEasy.so'"
	sys.exit(1)

iodefv2 = LibIodefEasy.IODEFV2()
iodefv2.ReadFromFile("foo.bin")
iodefv2.PrintToStdout()
