import sys
from sage.all import *

N = 2**64
t = 2**10
m = 2**20
l = 4
rps = 0.99
rmsc = 0.0

def calc_rmsc():
    return 2*((1.0 - rps)**(-(1/(2*l))) - 1.0)

def solve_mt(m):
    x = var('x')
    a = solve(x * m == rmsc * N, x, solution_dict=True)
    #print (int(log(a[0][x])/log(2)))
    return 2**(int(log(a[0][x])/log(2)))

def memory():
    return m * l

def time_for_offline():
    return N

def time_for_online():
    return t * t

print ""
print "Proberbility required set to: %f with tables set: %i" % (rps, l)
rmsc = calc_rmsc()
for x in xrange(0, 10):
    print ""
    m = 2**(40 + x)
    t = solve_mt(m)
    print "t: 2^%d" % (int(log(t)/log(2)))
    print "m: 2^%d" % (int(log(m)/log(2)))
    print "Rmsc calculated: %f" % rmsc
    print "Memory used: 2^" + str(int(log(memory())/log(2)))
    print "Time used for offline phase: 2^" + str(int(log(time_for_offline())/log(2)))
    print "Time used for online phase: 2^" + str(int(log(time_for_online())/log(2)))
