a = b = c = d = e = f = g = h = 0

b = 57
c = b
if a != 0:
    b = 105700
    c = 122700

f = 1 #M3
d = 2

e = 2 # M2
g = d * e # M1
g -= b
if g == 0:
    f = 0
e += 1
if e != b: goto M1
d += 1
if d != b: goto M2

if f == 0:
    h += 1
g = b
g -= c
if g != 0:
    b += 17
    goto M3
