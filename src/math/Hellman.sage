import sys
from sage.all import *
I = 64.0
N = 2**I
mx=[1,1,1,1]
def eq1(y,h):
    x=var('x')
    sol = solve(2**(64/y)*((2**(64/x))**2)==h*N,x,solution_dict=True);sol
    return [ abs(n(s[x])) for s in sol]
def T(hmsc,hps,Hcr,y):
    return n((1/hmsc+1/6)*(hps/Hcr)*(2**(64/y))**2)

def M(n,hs,x,y):
    return (n*hs)/(2**(64/x)*(2**(64/y)))*2**(64/x)
#Htc=(((1/Hmsc)+(1/6))*(1/(Hcr**3)))*Hps*(ln(1-Hps))**2
def CalcStuffs(hps,hmsc):
    ##############
    #Coverage Rate
    #############
    Hcr=(sqrt(2)/sqrt(hmsc))*((e**(sqrt(2*hmsc))-1)/(e**(sqrt(2*hmsc))+1))

    #############
    #Time memory tradeof Curve coeiffiecient
    #############
    Htc=(((1/hmsc)+(1/6))*(((sqrt(hmsc)/sqrt(2))*((e**(sqrt(2*hmsc))+1)/(e**(sqrt(2*hmsc))-1)))**3)*hps*(ln(1-hps))**2)

    #############
    #Precomputation cost
    ############
    Hpc=(sqrt(hmsc)/sqrt(2))*((e**(sqrt(2*hmsc))+1)/(e**(sqrt(2*hmsc))-1))*(-ln(1-hps))

    print(" For probability: %.2f and matrix stopping constant: %.2f\n Has coverage: %.4f \n tm-coeif: %.4f \n Precomputational cost: %.4f\n"%(hps,hmsc,Hcr,Htc,Hpc))

    a=1.5
    for h in range(0,len(mx)):
        mx[h]=(a,eq1(a,hmsc)[0])
        a=a+0.50
   # print(mx)
    for k in range(0,len(mx)):
        print("     For m: 2^%i and t: 2^%i \n     Time: 2^%i \n     Memory: 2^%i\n"%(log(2**(64/mx[k][0]))/log(2) , log(2**(64/mx[k][1]))/log(2) , log(T(hmsc,hps,Hcr,mx[k][1]))/log(2.0) ,log(M(N,n(Hpc),mx[k][0],mx[k][1]))/log(2.0)))
    print("\n\n")

CalcStuffs(0.5768,2.25)
CalcStuffs(0.5768,1)
CalcStuffs(0.9,1)
