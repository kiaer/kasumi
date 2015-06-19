import sys
from sage.all import *
I = 64.0
N = 2**I
mx=[1,1,1,1,1,1,1,1,1,1]
my=[1,1,1,1,1,1,1,1,1,1]
result=[(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1),(1,1)]
matrix=[]
m=10

#mzero= m/(1-e^(-tmax/t))

#dtmp=dtc/(dps*(ln(1-dps))**2)
#l=




def eq1(y,h):
    x=var('x')
    assume(x,'real')
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

def TL(t,l):
    return t*l

def mtl(k,dmsc):
    m=k
    for h in range(0,len(mx)):
        my[h]=(m,eq1(m,dmsc)[0])
        m=m+0.5
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
    return x*y*I

def L(dcr,dmsc,dps,x):
    return (1/(dcr*dmsc))*(-ln(1-dps))*(2**(x))

#Htc=(((1/Hmsc)+(1/6))*(1/(Hcr**3)))*Hps*(ln(1-Hps))**2
def CalcStuffs(dps,dmsc,k):
    mx=mtl(k,dmsc)
    ##############
    #Coverage Rate
    #############
    dcr=2/(sqrt(1+2*dmsc)+1)
    #############
    #Time memory tradeof Curve coeiffiecient
    #############
    dtc=((2+1/dmsc)*(((sqrt(1+dmsc*2)+1)/2)**3))*(dps*((ln(1-dps))**2))
    print(((2+1/dmsc)*(((sqrt(1+dmsc*2)+1)/2)**3)))
    print((dps*((ln(1-dps))**2)))
    #############
    #Precomputation cost
    ############
    dpc=((sqrt(1+dmsc*2)+1)/2)*(-ln(1-dps))
    dtmp=(2+(1/dmsc))*(((sqrt(1+dmsc*2)+1)/2)**3)

    print(" For probability: %.2f and matrix stopping constant: %.2f\n Has coverage: %.4f \n tm-coeif: %.5f \n dtmp: %.5f \n Precomputational cost: %.4f\n"%(dps,dmsc,dcr,dtc,dtmp,dpc))



    matrix.append(("dps: %.2f"  %dps,
                   "dmsc: %.2f" %dmsc,
                   "dcr: %.2f"  %dcr,
                   "dtc: %.2f"  %dtc,
                   "dpc: %.2f"  %dpc,
                   "dtmp %.2f"  %dtmp,))

    matrix.append(("m","t","l","M in tb","T"))

    for k in range(0,len(mx)):
        m = (mx[k][0])
        t = (mx[k][1])
        l = L(dcr,dmsc,dps,t)
        Ms=n(M(2**m,l))
        Ts=n(2**t*l)
        result[k]=(Ms*(1.25*10**(-13)),Ts)
        matrix.append(("$2^{%.2f}$"  %log(2**m,2),
                       "$2^{%.2f}$" %log(2**t,2),
                       "$2^{%.2f}$" %log(l,2),
                       "$%.2f$ TB"  %(Ms*(1.25*10**(-13))),
                       "$2^{%.2f}$" %log(Ts,2)))

        print("     For m: 2^%.2f, t: 2^%.2f and l:2^%.2f \n     Time: 2^%.2f \n     Memory: 2^%.2f\n"%(                                   log(2**m,2),
                    log(2**t,2),
                    log(l,2),                                                                                                              log(Ts,2),
                    log(Ms,2)
                    ))
    print("\n\n")
    matrix.append(("","","","","",""))
    g=list_plot_semilogy(result,base=2,plotjoined=True,title="DP %.2f for %.2f"%(dmsc,dps),marker='o')
   # g=list_plot(MX,plotjoined=True,title="Hellman for %.2f"%(hps),marker='o')
    g.axes_labels(['M','T'])

    g.save('graphs/%.2fMT.png'%(dps))
    return g


comps=plot([],title='Comparison DP')
comp=CalcStuffs(0.60,0.562047,11)+CalcStuffs(0.73,0.562047,10)+CalcStuffs(0.90,0.562047,8)+comps
comp.axes_labels(['M in TB','T'])
comp.show()
comp.save("graphs/compare DP with large th.png")
comps1=plot([],title='Comparison Dmsc')
comp1=CalcStuffs(0.73,0.20,9)+CalcStuffs(0.73,0.35,9)+CalcStuffs(0.73,0.562047,9)+CalcStuffs(0.73,0.75,9)+CalcStuffs(0.73,1,9)+CalcStuffs(0.73,1.25,9)+comps
comp1.axes_labels(['M in TB','T'])
comp1.show()
comp1.save("graphs/compare Dmsc.png")
f=open("DP_table.tex", 'w')
f.write(latex(table(rows=matrix)))

#mx=mtl(2.59169)
#CalcStuffs(0.60,1.26453,2.59169)
#CalcStuffs(0.73,1.26453,2.59169)
#CalcStuffs(0.90,1.26453,2.59169)
