import sys
from sage.all import *
I = 64.0
N = 2**I
mx=[1,1,1,1,1,1,1,1,1,1]
my=[1,1,1,1,1,1,1,1,1,1]
result=[(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1)]
m=10
#mzero= m/(1-e^(-tmax/t))

#dtmp=dtc/(dps*(ln(1-dps))**2)
#l=




def eq1(y,h):
    x=var('x')
    sol = solve(2**(y)*((2**(x))**2)==h*N,x,solution_dict=True);sol
    return [ abs(n(s[x])) for s in sol]

def T(dmsc,dps,dcr,t,th):
    t_hat=t*th
    p1=(dps/(dcr*dmsc))
    p2=n(1-e**(-t_hat/t))
    p3=n(dmsc/(1-e**(-t_hat/t)))
    p4=n(8/(e**(t_hat/(2*t))))
    p5=n((3*t_hat)/t)
    p6=n((t_hat**2)/(2*(t**2)))
    p7=n(e**(t_hat/t))
    p8=n(1/(e**((2*t_hat)/t)))
    return n(p1*((p2+p3*(2-p4+((5+p5-p6)/p7)+p8))*t**2))


def mtl(dmsc):
    m=16
    for h in range(0,len(mx)):
        my[h]=(m,eq1(m,dmsc)[0])
        m=m+1
    return my

def DTC(dmsc,dcr,dps,t,th):
    t_hat=t*th
    p5=n((3*t_hat)/t)
    p6=n((t_hat**2)/(2*(t**2)))
    p7=n(e**((t_hat)/t))
    return(((2*dmsc+1)-((8*dmsc)/(e**(t_hat/(2*t)))) +(((5+p5-p6)*dmsc-2)/p7)+(dmsc+1)/(e**((2*t_hat)/t)))*(((dps*(ln(1-dps)))**2)/(1-e**(-t_hat/t))*(dcr**3)*dmsc))

def DTC2(dmsc,dcr,dps,t,th):
    t_hat=t*th
    p5=n((3*t_hat)/t)
    p6=n((t_hat**2)/(2*(t**2)))
    p7=n(e**((t_hat)/t))
    e1=(2*dmsc+1)-(8*dmsc/(e**(t_hat/2*t)))+(((5+p5-p6)*dmsc-2)/p7)
    e2=(1-e**(t_hat/t))*dcr*(dmsc)
    return null

def M(x,y):
    return x*y

def L(dcr,dmsc,dps,x):
    return (1/(dcr*dmsc))*(-ln(1-dps))*(2**(x))

#Htc=(((1/Hmsc)+(1/6))*(1/(Hcr**3)))*Hps*(ln(1-Hps))**2
def CalcStuffs(dps,dmsc,th):
    ##############
    #Coverage Rate
    #############
    dcr=2/(sqrt(1+2*dmsc)+1)
    #############
    #Time memory tradeof Curve coeiffiecient
    #############
    dtc=(2+(1/dmsc)*(1/(2/(sqrt(1+dmsc*2)+1))**3))*(dps*((ln(1-dps))**2))
    #############
    #Precomputation cost
    ############
    dpc=((sqrt(1+dmsc*2)+1)/2)*(-ln(1-dps))
    dtmp=(2+(1/dmsc))*(((sqrt(1+dmsc*2)+1)/2)**3)

    print(" For probability: %.2f and matrix stopping constant: %.2f\n Has coverage: %.4f \n tm-coeif: %.5f \n dtmp: %.5f \n Precomputational cost: %.4f\n"%(dps,dmsc,dcr,dtc,dtmp,dpc))


   # print(mx)
    for k in range(0,len(mx)):
        Ms=n(M(2**(mx[k][0]),L(dcr,dmsc,dps,mx[k][1])))
        TT=n(T(dmsc,dps,dcr,2**(mx[k][1]),th))
        Ts=n(2**(mx[k][1])*L(dcr,dmsc,dps,mx[k][1]))
       # print(2**(mx[k][1]))
        #print(L(dcr,dmsc,dps,mx[k][1]))
        result[k]=(Ms*(1.25*10**(-13)),Ts)
        print("     For m: 2^%.2f, t: 2^%.2f and l:2^%.2f \n     Time: 2^%.2f \n     Memory: 2^%.2f\n Tcoif: %.2f \n"%(log(2**(mx[k][0]))/log(2), log(2**(mx[k][1]))/log(2),log(L(dcr,dmsc,dps,mx[k][1]))/log(2),log(Ts)/log(2.0),log(Ms)/log(2),DTC(dmsc,dcr,dps,2**(mx[k][1]),th)))
    print("\n\n")

    g=list_plot_semilogy(result,base=2,plotjoined=True,title="DP %.2f for %.2f"%(dmsc,dps),marker='o')
   # g=list_plot(MX,plotjoined=True,title="Hellman for %.2f"%(hps),marker='o')
    g.axes_labels(['M','T'])

    g.save('tables/%.2fMT.png'%(dps))
    return g

mx=mtl(0.562047)
comps=plot([],title='Comparison DP')
comp=CalcStuffs(0.60,0.562047,10)+CalcStuffs(0.73,0.562047,10)+CalcStuffs(0.90,0.562047,10)+comps
comp.axes_labels(['M in TB','T'])
comp.show()
comp.save("tables/compare DP with large th.png")

#mx=mtl(2.59169)
#CalcStuffs(0.60,1.26453,2.59169)
#CalcStuffs(0.73,1.26453,2.59169)
#CalcStuffs(0.90,1.26453,2.59169)
