import scipy.optimize
from pylab import *
from numpy import *
from mpl_toolkits.mplot3d import Axes3

cal = open('cal.dat')
fig = figure(1); clf()
az = Axis3D(fig)

for f in [cal]:
    data = array([map(float, l.split()) for l in f.readlines()])

    N = len(data[0])

    x = data[:,0]
    y = data[:,1]
    z = data[:,2]
    ax.scater(x,  y, z)
    here

    A = array([[cx + r * cos(theta),
                cy + r * sin(theta)] for theta in arange(0, 2 * pi, 1 * pi/180)])
    # plot(A[:,0], A[:,1], 'g-')
    xy = data[:,1:3]

    def cost(params):
        cx, cy, r = params
        xy_ = xy - [cx, cy]
        thetas = arctan2(xy_[:,1], xy_[:,0])
        resids = xy_ - transpose([r * cos(thetas), r * sin(thetas)])
        return sum(ravel(resids ** 2))

    cx, cy, r = scipy.optimize.fmin(cost, [cx, cy, r], disp=False)
    print f.name, cx, cy, r
## acc_cal_lights_on.dat 550.150958354 507.218838209 249.831129791
## acc_cal_lights_off.dat 563.391868993 518.281081432 251.367556713

    A = array([[cx + r * cos(theta),
                cy + r * sin(theta)] for theta in arange(0, 2 * pi, 1 * pi/180)])
    plot(A[:,0], A[:,1])
