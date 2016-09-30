#!/usr/bin/python

import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import LibIodefEasy
except:
	print "Import failed"
	print "Try 'cd ./.libs && ln -s libiodefv2_python.so _LibIodefEasy.so'"
	sys.exit(1)

def foo(id):
        print "callback: id = " + str(id)
	iodefv2 = LibIodefEasy._get_IODEFV2(id)
        iodefv2.PrintToStdout()
        #print bar.Get("alert.classification.text") # XXX not yet implemented
        return 0

LibIodefEasy.set_pymethod(foo)

LibIodefEasy.test_fct()
