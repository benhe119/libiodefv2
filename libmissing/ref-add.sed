/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ libiodefv2 / libiodefv2 /
  tb
  s/ $/ libiodefv2 /
  :b
  s/^/# Packages using this file:/
}
