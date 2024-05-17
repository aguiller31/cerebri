import matplotlib.pyplot as plt
import numpy as np
max_angular_speed=60
y0=0.3#pour angle de 0%
y1=0.3#pour angle de 20%
y2=0.3 #pour angle de 40%
y3=0.4 #pour angle de 60%
y4=0.8 #pour angle de 80%
y5=0.9 #pour angle de 100%



a=round(((-2.6042e-9)*y0+(1.3021e-8)*y1+(-2.6042e-8)*y2+(2.6042e-8)*y3+(-1.3021e-8)*y4+(2.6042e-9)*y5)*1e12)*1e-12
b=round(((7.8125e-7)*y0+(-3.6458e-6)*y1+(6.7708e-6)*y2+(-6.25e-6)*y3+(2.8646e-6)*y4+(-5.2083e-7)*y5)*1e12)*1e-12
c=round(((-8.8542e-5)*y0+(3.6979e-4)*y1+(-6.1458e-4)*y2+(5.1042e-4)*y3+(-2.1354e-4)*y4+(3.6458e-5)*y5)*1e12)*1e-12
d=round(((4.6875e-3)*y0+(-1.6042e-2)*y1+(2.2292e-2)*y2+(-1.625e-2)*y3+(6.3542e-3)*y4+(-1.0417e-3)*y5)*1e12)*1e-12
e=round(((-1.1417e-1)*y0+(2.5e-1)*y1+(-2.5e-1)*y2+(1.6667e-1)*y3+(-6.25e-2)*y4+0.01*y5)*1e12)*1e-12
f=y0
#speed at(%)  0  10   20 30 40 50 60 70 80 90 100 100
speeds =    [100,100,100,90,80,80,70,70,60,60,60,60]
#steer at(%)   0    10   20   30   40   50   60   70   80   90  100  100
steers =    [0.30,0.30,0.30,0.30,0.30,0.35,0.40,0.70,0.80,0.85,0.90,0.90]


print(a,b,c,d,e,f)
def gain(x):
    return round((a*(x**5)+b*(x**4)+c*(x**3)+d*(x**2)+e*x+f)*1e3)*1e-3
def gain_speed(x):
    if x > 100:
        x=100
    q=int((x-x%10)/10)
    return (speeds[q+1]-speeds[q])/10*(x-q*10)+speeds[q]

def gain_steer(x):
    if x > 100:
        x=100
    q=int((x-x%10)/10)
    return (steers[q+1]-steers[q])/10*(x-q*10)+steers[q]

x = np.linspace(0, 100, 1000)*100/15
y1 = [gain(i) for i in x]
y3 = [gain_steer(i) for i in x]
y=x*y1
y4=x*y3
y2 = [gain_speed(i) for i in x]


fig, axs = plt.subplots(1, 3)  # 1 ligne, 2 colonnes
axs[0].plot(x, y)
axs[0].plot(x, y4)
axs[1].plot(x, y2)
axs[2].plot(x, y1)
axs[2].plot(x, y3)


axs[0].set_xlabel('angle mesuré')
axs[0].set_ylabel('steer(%) = x*gain(x)')
axs[0].set_title('Plot of the steer (%)')
axs[0].set_xlim(0, 100)
axs[0].set_ylim(0, 100)

axs[1].set_xlabel('angle mesuré')
axs[1].set_ylabel('speed(%)')
axs[1].set_title('Plot of the speed (%)')
axs[1].set_xlim(0, 100)
axs[1].set_ylim(0, 200)

axs[2].set_xlabel('angle mesuré')
axs[2].set_ylabel('gain_steer(x)')
axs[2].set_title('Plot of the steer gain')
axs[2].set_xlim(0, 100)
axs[2].set_ylim(0, 2)


plt.show()
