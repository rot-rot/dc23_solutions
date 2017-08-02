# -*- coding: cp1252 -*-
s = "/zx{x{v|w{v{w{v{z|z|xyx{v|u|z|yy}{«{©{~{Š{~|x"

v = [ord(x) for x in s]

for i in range(0,256):
  print "".join(chr((n + i) % 256) for n in v)


def freq(x):
  ret = {}
  for c in x:
    if c in ret:
      ret[c] += 1
    else:
      ret[c] = 1
  return ret

f = freq(s)

print len(f)

for i in f.keys:
  print f[i]
