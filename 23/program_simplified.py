a = b = c = d = e = f = g = h = 0

b = 57
c = b
a = 1
if a != 0:
    b = 105700
    c = 122700

def is_prime(b):
    d = 2
    while d*d < b:
        if b % d == 0:
            return False
        d += 1
    return True

while b <= c:
    if not is_prime(b):
        h += 1
    b += 17

print(h)
