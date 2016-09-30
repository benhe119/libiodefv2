#!/usr/bin/env ruby 

require("LibIodef")

LibIodef::LibIodefLog::setCallback(lambda{|level,str|print "log: " + str})
iodefv2 = LibIodef::IODEFV2.new()

print "*** IODEFV2->Set() ***\n"
iodefv2.set("alert.classification.text", "My Message")
iodefv2.set("alert.source(0).node.address(0).address", "s0a1")
iodefv2.set("alert.source(0).node.address(1).address", "s0a2")
iodefv2.set("alert.source(1).node.address(0).address", "s1a1")
iodefv2.set("alert.source(1).node.address(1).address", "s1a2")
iodefv2.set("alert.source(1).node.address(2).address", nil)
iodefv2.set("alert.source(1).node.address(3).address", "s1a3")
print iodefv2

print "\n*** Value IODEFV2->Get() ***\n"
print iodefv2.get("alert.classification.text")

print "\n\n*** Listed Value IODEFV2->Get() ***\n"
print iodefv2.get("alert.source(*).node.address(*).address")

print "\n\n*** Object IODEFV2->Get() ***\n"
print iodefv2.get("alert.source(0).node.address(0)")

print "\n\n*** Listed Object IODEFV2->Get() ***\n"
print iodefv2.get("alert.source(*).node.address(*)")
print "\n\n"

fd = File.new("foo.bin", "w")
iodefv2 >> fd
#iodefv2.Write(fd)
fd.close()

print "\n*** IODEFV2->Read() ***\n"
fd2 = File.new("foo.bin", "r")
iodefv22 = LibIodef::IODEFV2.new()
while true do
	begin
		iodefv22 << fd2
		print iodefv22
	rescue EOFError
                print "Got EOF\n"
		break
	end
end
fd2.close()

fd2 = File.new("foo.bin", "r")
iodefv22 = LibIodef::IODEFV2.new()
while iodefv22.read(fd2) > 0 do
	print iodefv22
end
fd2.close()

print "\n*** Client ***\n"
c = LibIodef::ClientEasy.new("libiodefv2-lml")
c.start()

c << iodefv2
