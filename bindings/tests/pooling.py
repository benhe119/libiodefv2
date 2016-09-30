#!/usr/bin/python

import LibIodefEasy

client = LibIodefEasy.ClientEasy("PoolingTest", LibIodefEasy.Client.IODEFV2_READ)
client.Start()

while True:
    iodefv2 = LibIodefEasy.IODEFV2()

    ret = client.RecvIODEFV2(iodefv2)
    if ret:
	print iodefv2
