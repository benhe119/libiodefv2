import iodefv2
import datetime

msg = iodefv2.IODEFV2()


msg.set("incident(0).incident_id.name", "First_incident")
msg.set("formatid", "formatid")
msg.set("version", "2.0")

msg.set("incident(0).contact(0).contact_name", "Abdellah")
msg.set("incident(0).method(0).reference(0).reference_name", "reference unique")
msg.set("incident(0).report_time", str(datetime.datetime.now()))

print(msg)
print(msg.toJSON())

msg2 = iodefv2.IODEFV2(msg.toJSON())
print(msg2)

