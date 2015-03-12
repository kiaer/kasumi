import sys
from sage.all import *

def spHellman(I,m,t,N):
    M=2**(I/m)
    T=2**(I/t)
    l=T
    Hmsc=1
    V=(l*M*T)/N
    P=1-e**(-V)
    return P

I = 64.0
m = [1.25, 1.5, 1.6667, 2, 3, 4    , 5  , 6  , 10    ,3]
t = [10  , 6  , 5     , 4, 3, 2.666, 2.5, 2.4, 2.2222,2.9]
N = 2**I
Hmsc=1
Hpc=1

#Hcr=(sqrt(2)/sqrt(Hmsc))*((e**(sqrt(2*Hmsc))-1)/(e**(sqrt(2*Hmsc))+1))
#Time=(1/Hmsc+1/6)*(Hps/Hcr)*(64**(2/3))**2
#CostOfAlarms=(Hmsc/6)*2**(2/t[x])

mx=[]

#def eq(y):
x=var('x')
sol = sage_solve(2**(64/2)*(2**(64/x))**2==18446744073709551616,x);sol

[ n(s[x]) for s in sol]




for x in range(0,len(m)):
    print("Memory: %f  \n time: %f \n probability is %f\n"%((2**64/(m[x]))*(2**(64/t[x])),(2**(64/t[x]))**2,spHellman(I,m[x],t[x],N)))

#mx[0]=eq(2)
#print(mx)

#probabilirty
#optimized for memory
#optimized for time
#optimized for
