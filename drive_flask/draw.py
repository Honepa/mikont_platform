from json import load

log = load(open('log.json'))

keys = [
  "lft_target",
  "lft_avg",
  "lft_pwm",
  "VCC",
  "I",
]

for i in log:
  s = ""
  for k in keys:
    s += "%s " % i[k]
  open('to_draw', 'w').write("%s\n" % s)
