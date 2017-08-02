
s = """KVABEGFBQEBA"""


for i in range(0,256):
  print i,"".join(chr((ord(x) + i) % 256) for x in s)

for c in s:
  print ord(c),bin(ord(c))
