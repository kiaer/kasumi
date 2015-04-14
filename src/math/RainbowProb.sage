import sys
from sage.all import *

N = 2**64
t = 2**10
m = 2**20
l = 3
rps = [0.58, 0.73, 0.90]
rmsc = 0.0

def calc_rmsc(ps):
    return 2*((1.0 - ps)**(-(1/(2*l))) - 1.0)

def solve_mt(m, rm):
    x = var('x')
    a = solve(x * m == rm * N, x, solution_dict=True)
#    print (float(log(a[0][x])/log(2)))
    return 2**(float(log(a[0][x])/log(2)))

def memory():
    return m * l * (1.25 * 10**-13)

def time_for_offline():
    return N * rmsc * l

def time_for_online():
    return (t * t) + t

print ""


plot_all_coef = plot([], plottitle="Comparison")
plot_all_calc = plot([])

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
    for x in xrange(0, 18):
        print ""
        m = 2**(37 + (0.5 * x))
        t = solve_mt(m, rmsc)
        lstm.append(m)
        lstt.append(t)
        # lstm.append()
        # lstt.append()
        print "t: 2^%f" % (float(log(t)/log(2)))
        print "m: 2^%f" % (float(log(m)/log(2)))
        print "Rmsc calculated: %f" % rmsc
        print "Memory used: " + str(memory()) + "TB"
        lsttime.append(time_for_online())
        lstmem.append(memory())
        print "Time used for offline phase: 2^" + str(float(log(time_for_offline())/log(2)))
        print "Time used for online phase: 2^" + str(float(log(time_for_online())/log(2)))
    f = open('rainbowtab.tex', 'a')
    col = [lstm, lstt, lstmem, lsttime]
    #Prob, RMSC, l, Offline comp
    infostr = "Success = %f, Rmsc = %f, l = %i, Offline phase = 2^%f" % (rps[i], rmsc, l, float(log(time_for_offline())/log(2)))
    f.write("\n \\ " + str(latex(infostr)) + "\n" + str(latex(table(columns=col, header_row=['m', 't', 'M(TB)', 'T']))))

    plottitle = "Rainbow coefficents for %i %% succesrate" % (rps[i] * 100)

    plot = list_plot_loglog(zip(lstm,lstt), plotjoined = True,base=2, title=plottitle, color='red', marker='o')
    plot.axes_labels(["m", "t"])
    #plot.show()
    plot.save('RainbowCoef' + str(rps[i] * 100) + '.png')

    plot2 = list_plot_semilogy(zip(lstmem, lsttime), plotjoined = True, base=2, title=plottitle, color='blue', marker='o')
    plot2.axes_labels(["M", "T"])
    #plot2.show()
    plot2.save('RainbowTime' + str(rps[i] * 100) + '.png')
    
    plot_all_coef  += (plot)
    plot_all_calc += (plot2)

plot_all_coef.axes_labels(["m", "t"])
#plot_all_coef.show()
plot_all_coef.save('RainbowAllCoef.png')

plot_all_calc.axes_labels(["M", "T"])
#plot_all_calc.show()
plot_all_calc.save('RainbowAllCalc.png')
