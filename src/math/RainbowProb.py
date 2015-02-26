import sys
from sage.all import *

N = 2**64

# def prob(t, m):
#     prod = 1.0
#     for x in xrange(0, 2**21):
#         prod = prod * (1.0 - (mj(m, x) / N))
#     print (1 - prod)

# def mj(m, x):
#     res = m
#     for i in range(1,x):
#         res = res
#     return res

def memory(t, m):
    return t * m

def time_for_offline(t, m):
    return 123

def time_for_online(t, m):
    return 123

t = 2**21
m = 2**21

print memory(t, m)
