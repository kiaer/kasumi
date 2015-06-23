import sys
from sage.all import *
from sage.symbolic.integration.integral import definite_integral


N = 2**64
t = 2**2
m = 2**10
l = 1
rps = [0.87, 0.89, 0.73]
rmsc = 0.0

def calc_rmsc(ps):
    return 2*((1.0 - ps)**(-(1/(2*l))) - 1.0)

def solve_mt(m, rm):
    x = var('x')
    a = solve(x * m == rm * N, x, solution_dict=True)
#    print (float(log(a[0][x])/log(2)))
    return 2**(float(log(a[0][x])/log(2)))

def memory():
    return m * l * (1.25 * 10**-13) * 64

def time_for_offline(rm):
    return N * rm * l

def time_for_online(rm):
    #print n(log(((t * t) + t), 2))
    #print (t * t) * l * ((matrix([[((2*l-1)+(2*l+1)*rm)*(2 + rm)**2],[-4*((2*l - 1)+l*(2*l + 3)*rm)*(2/(2 + rm)**(2*l))]])) / ((2*l + 1) * (2*l + 2) * (2*l + 3) * rm**2))
    a = t**2 * l * definite_integral(x * (1 + rm *(1 - x)) * (1 - rm / (2 + rm))**(2*l), x, 0, 1) + t * l * (2 + rm - 2 * (2 / (2 + rm))**(2 * l) / (2 * l + 1) * rm)
    #print log(a, 2)
    return a

def big_T(t,l,rm):
    return (t**2)*l*((((2*l-1)+(2*l+1)*rm)*((2+rm)**2)-4*((2*l-1)+l*(2*l+3)*rm)*((2/(2+rm))**(2*l)))/((2*l+1)*(2*l+2)*(2*l+3)*(rm**2)))

def lookups(m,t,rm):
    return (t*((2+rm-2*((2/(2+rm))**2))/(3*rm)))

#def rtc:

print ""


plot_all_coef = plot([], plottitle="Comparison")
plot_all_calc = plot([], plottitle="yo dawg")
eff = plot([], plottitle="Comparison 1")

for i in xrange(0,3):
    lstm = []
    lstm2 = []
    lstt = []
    lstt2 = []
    lsttime = []
    lsttime2 = []
    lstmem = []
    lstmem2 = []
    print "Proberbility required set to: %f with tables set: %i" % (rps[i], l)
    rmsc = calc_rmsc(rps[i])
    for x in xrange(0, 5):
        print ""
        #64 bit - 35.5
        #32 bit -
        m = 2**(39 + (0.2 * x))
        t = solve_mt(m, rmsc)
        lstm.append(m)
        temp = "$2^{%.2f}$" % log(m, 2)
        lstm2.append(temp)
        #print "$2^{%.2f}$" % log(m, 2)
        temp = "$2^{%.2f}$" % log(t, 2)
        lstt.append(t)
        lstt2.append(temp)
        print "t: 2^%f" % (float(log(t)/log(2)))
        print "m: 2^%f" % (float(log(m)/log(2)))
        print "Rmsc calculated: %f" % rmsc
        print "Memory used: " + str(memory()) + "TB"
        temp = "$2^{%.2f}$" % log(time_for_online(rmsc), 2)
        lsttime.append(time_for_online(rmsc))
        lsttime2.append(temp)
        temp = "$%.2f$" % memory()
        lstmem.append(memory())
        lstmem2.append(temp)
        print "Time used for offline phase: 2^" + str(float(log(time_for_offline(rmsc))/log(2)))
        print "Time used for online phase: 2^" + str(float(log(time_for_online(rmsc))/log(2)))
        print "Time used for online phase: 2^" + str(float(log(big_T(t,1,rmsc))/log(2)))
        print "\n"
    # f = open('rainbowtab.tex', 'a')

    col = [lstm2, lstt2, lstmem2, lsttime2]
    #Prob, RMSC, l, Offline compp
    # infostr = "Success = %f, Rmsc = %f, l = %i, Offline phase = 2^%f" % (rps[i], rmsc, l, float(log(time_for_offline(rmsc))/log(2)))
    # f.write("\n \\ " + str(latex(infostr)) + "\n" + str(latex(table(columns=col, header_row=['m', 't', 'M(TB)', 'T']))))

 #    plottitle = "Rainbow coefficents for %i %% succesrate" % ((rps[i] * 100))
#     plottitle2 = "Rainbow T and M for %i %% succesrate" % ((rps[i] * 100))

#     plot = list_plot_loglog(zip(lstm,lstt), plotjoined = True,base=2, title=plottitle, color='red', marker='o')
#     plot.axes_labels(["m", "t"])
#     #plot.show()
#     plot.save('graphs/RainbowCoef' + str(rps[i] * 100) + '.png')

#     plot2 = list_plot_semilogy(zip(lstmem, lsttime), plotjoined = True, base=2, title=plottitle2, color='blue', marker='o')
#     plot2.axes_labels(["M", "T"])
#     #plot2.show()
#     plot2.save('graphs/RainbowTime' + str(rps[i] * 100) + '.png')

#     plot_all_coef  += (plot)
#     plot_all_calc += (plot2)

# plot_all_coef.axes_labels(["m", "t"])
# #plot_all_coef.show()
# plot_all_coef.save('graphs/RainbowAllCoef.png')

# plot_all_calc.axes_labels(["M", "T"])
# plot_all_calc += (eff)
# #plot_all_calc.title("HILLDAWG")
# plot_all_calc.show()
# plot_all_calc.save('graphs/RainbowAllCalc.png')
