#!/usr/bin/env lua

require("libiodefv2")

function my_cb(level, log)
	io.write("log: " .. log)
end
libiodefv2.LibIodefLog_setCallback(my_cb)


function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    elseif not v then
      print(formatting .. 'NUL')
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))
    else
      print(formatting .. tostring(v))
    end
  end
end


iodefv2 = libiodefv2.IODEFV2()

print("*** IODEFV2->Set() ***")
iodefv2:set("alert.classification.text", "My Message")
iodefv2:set("alert.source(0).node.address(0).address", "s0a0")
iodefv2:set("alert.source(0).node.address(1).address", "s0a1")
iodefv2:set("alert.source(1).node.address(0).address", "s1a0")
iodefv2:set("alert.source(1).node.address(1).address", "s1a1")
iodefv2:set("alert.source(1).node.address(2).address", nil)
iodefv2:set("alert.source(1).node.address(3).address", "s1a3")
print(iodefv2)

print("\n*** Value IODEFV2->Get() ***")
print(iodefv2:get("alert.classification.text"))

print("\n*** Listed Value IODEFV2->Get() ***")
tprint(iodefv2:get("alert.source(*).node.address(*).address"))

print("\n*** Object IODEFV2->Get() ***")
print(iodefv2:get("alert.source(0).node.address(0)"))

print("\n*** Listed Object IODEFV2->Get() ***")
tprint(iodefv2:get("alert.source(*).node.address(*)"))


fd = io.open("foo.bin","w")
iodefv2:write(fd)
fd:close()

print("\n*** IODEFV2->Read() ***")
fd2 = io.open("foo.bin","r")
iodefv22 = libiodefv2.IODEFV2()
while iodefv22:read(fd2) do
	print(iodefv22)
end
fd2:close()


print("\n*** Client ***")
c = libiodefv2.ClientEasy("libiodefv2-lml")
c:start()

c:sendIODEFV2(iodefv2)
