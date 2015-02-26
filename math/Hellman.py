#!/becktor/bin/env sage -python

import sys
from sage.all import *
def hellman_prob(x):

    I=64
    print(I)
    N=2**I
    print(N)
    m=2**(I/x)
    print(m)
    t=2**(I/x)
    l=t
    print(l*t*m)
    P=float(1-e**(-(l*t*m)/N))
    return P



print (hellman_prob(int(sys.argv[1])))
