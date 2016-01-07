# coding: utf-8
import math


def dis(v):
    return math.sqrt(v[0]*v[0] + v[1]*v[1])

# contant
# unit: mm
L = 9
Wb = 46
Dm = 100
Mc = (Dm/2, Dm/2)
Jb = (Wb/2-L/math.sqrt(2), Wb/2)
Rm = 7
sina = L/2/Rm
cosa = math.sqrt(1-sina*sina)
Lomc = dis(Mc)

# equations
# V(x) means vector
# 1. V(JmMc)*V(OMc) = Lomc*Rm*cosa
# 2. Yjm-Yjb = Xjm-Xjb

# 1. becomes
# (Dm/2-Xjm)*Dm/2 + (Dm/2-Yjm)*Dm/2 = Lomc*Rm*cosa

# solution
# Xjm = Dm/2-Lomc*Rm*cosa/Dm+Xjb/2-Yjb/2
# Yjm = Dm/2-Lomc*Rm*cosa/Dm-Xjb/2+Yjb/2

Jm = (
    Dm/2 - Lomc*Rm*cosa/Dm + Jb[0]/2 - Jb[1]/2,
    Dm/2 - Lomc*Rm*cosa/Dm - Jb[0]/2 + Jb[1]/2
)

print('JbU = ', Jb)
print('JmU = ', Jm)
print('a = ', math.asin(sina)*57.29577951308232)
