import sys
from sage.all import *
I = 64.0
N = 2**I
mx=[1,1,1,1,1,1,1,1,1,1]
my=[1,1,1,1,1,1,1,1,1,1]
results=[[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]]
resultst=[[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0]]
MX=[[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0]]
matrix=[]


def eq1(y,h):
    x=var('x')
    assume(x,'real')
    sol = solve(2**(y)*((2**(x))**2)==h*N,x,solution_dict=True);sol
    return [abs(s[x]) for s in sol]

def mtl(k,hmsc):
    a=k
    for h in range(0,len(mx)):
        my[h]=(a,eq1(a,hmsc)[0])
        a=a+0.5
    return my

def T(hmsc,hps,Hcr,y):
    return n(n(1/hmsc+1/6)*n(hps/Hcr)*(n(2**(y))**2))

def ML(m,l):
    return m*l*I*2

#Htc=(((1/Hmsc)+(1/6))*(1/(Hcr**3)))*Hps*(ln(1-Hps))**2
def CalcStuffs(k,hps,hmsc):
    mx=mtl(k,hmsc)
    ##############
    #Coverage Rate for single table
    #############
    Hcr = (sqrt(2)/sqrt(hmsc))*((e**(sqrt(2*hmsc))-1)/(e**(sqrt(2*hmsc))+1))

    #############
    #Time memory tradeof Curve coeiffiecient
    #############
    Htc = (((1/hmsc)+(1/6))*(((sqrt(hmsc)/sqrt(2))*((e**(sqrt(2*hmsc))+1)/(e**(sqrt(2*hmsc))-1)))**3)*hps*(ln(1-hps))**2)

    #############
    #Precomputation cost
    ############
    Hpc = (sqrt(hmsc)/sqrt(2))*((e**(sqrt(2*hmsc))+1)/(e**(sqrt(2*hmsc))-1))*(-ln(1-hps))

    print(" For probability: %.2f and matrix stopping constant: %.2f\n each table has coverage: %.4f \n tm-coeif: %.4f \n Precomputational cost: %.4f\n"%(hps,hmsc,Hcr,Htc,Hpc))



    matrix.append(("hps: %.2f"%(hps),"hmsc: %.2f"%(hmsc),"Hcr : %.2f"%Hcr,"htc: %.2f"%Htc,"Hpc: %.2f"%Hpc,"",))
    matrix.append(("m","t","l","M in TB","T"))
    # print(mx)
    for k in range(0,len(mx)):
        m=(mx[k][0])
        t=(mx[k][1])
        l=n((1/(Hcr*hmsc))*(-ln(1-hps))*(2**t))

        results[k] = (m,t,n(log(l,2)))

        resultst[k] = (n(2**m), l)

        MX[k]= (ML(2**m,l)*(1.25*10**(-13)),T(hmsc,hps,Hcr,t))

        matrix.append(("$2^{%.2f}$"  %log(2**m,2),                # m
                       "$2^{%.2f}$" %log(2**t,2),                 # t
                       "$2^{%.2f}$" %log(l,2),                    # l
                       "$%.2f$"     %(ML(2**m,l)*(1.25*10**(-13))), # M in TB
                       "$2^{%.2f}$" %log(T(hmsc,hps,Hcr,t),2)))   # T



        print("     For m: 2^%.2f, t: 2^%.2f and l:2^%.2f \n     Time: 2^%.2f \n     Memory: 2^%.2f\n"
        %(
            m,                            #m
            t,                            #t
            log(l,2),                     #l
            log(T(hmsc,hps,Hcr,t),2),     #T
            log(ML(2**m,l),2)             #M
        ))

    print("\n\n")
    matrix.append(("","","","","",""))
    var('t','j','l','z')
    #a=list_plot((1/(Hcr*Hpc))*(-ln(1-hps))*t,(t,0,10))
    #a=points(results,plotjoined=True).rotateZ(pi/2)
    a=list_plot_loglog(resultst,base=2,plotjoined=True,title="Hellman %.2f for %.2f"%(hmsc,hps),marker='o')
    #a=list_plot(resultst,plotjoined=True,title="Hellman for %.2f"%(hps),marker='o')

    a.axes_labels(['t','l'])

    a.save('graphs/%.2fhej.png'%(hps))

    g=list_plot_semilogy(MX,base=2,plotjoined=True,title="Hellman %.2f for %.2f"%(hmsc,hps),marker='o')
   # g=list_plot(MX,plotjoined=True,title="Hellman for %.2f"%(hps),marker='o')
    g.axes_labels(['M','T'])

    g.save('graphs/%.2f %.2f Hellman.png'%(hps,hmsc))
    return g




comps=plot([],title='Comparison 1')
comp=CalcStuffs(10,0.5768,1)+CalcStuffs(9,0.73,1)+CalcStuffs(7,0.90,1)+comps
comp.axes_labels(['M in TB','T'])
comp.show()
comp.save("graphs/compare 1.png")

comps1=plot([],title='Comparison 2.25')
comp1=CalcStuffs(10,0.5768,2.25)+CalcStuffs(9,0.73,2.25)+CalcStuffs(7,0.90,2.25)+comps1
comp1.axes_labels(['M in TB','T'])
comp1.show()
comp1.save("graphs/compare 2.25.png")
comps2=plot([],title='Comparison of Hmsc')
comp2=CalcStuffs(9,0.73,0.75)+CalcStuffs(9,0.73,1.25)+CalcStuffs(9,0.73,1.75)+CalcStuffs(9,0.73,2.25)+CalcStuffs(9,0.73,2.75)+comps2
comp2.axes_labels(['M in TB','T'])
comp2.show()
comp2.save("graphs/compare hmsc.png")
f=open("HM_table.tex", 'w')
f.write(latex(table(rows=matrix)))
