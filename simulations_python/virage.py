import matplotlib.pyplot as plt
import numpy as np
from math import *

# Create a figure and a set of subplots
def distance(xA,yA,xB,yB):
    return sqrt((yA-yB)**2+(xA-xB)**2)
fig, ax = plt.subplots()

vecteurs = []
vecteurs2 = []
vecteurs3 = []
vecteurs4 = []
vecteurs.append((10,0, 0, 51,'red'))
vecteurs.append((60,0, 70, 51,'red'))

beta_ref = []
dx_ref_beta=vecteurs[0][2]-vecteurs[0][0]
dy_ref_beta=vecteurs[0][3]-vecteurs[0][1]
beta_ref.append(np.arctan(dy_ref_beta/dx_ref_beta))

dx_ref_sigma=vecteurs[1][2]-vecteurs[1][0]
dy_ref_sigma=vecteurs[1][3]-vecteurs[1][1]
beta_ref.append(np.arctan(dy_ref_sigma/dx_ref_sigma))

def get_angle():
    i=0
    for Ax,Ay,Bx, By, color in vecteurs:
        plt.arrow(Ax, Ay, Bx-Ax, By-Ay, head_width=0.5, head_length=0.5,color=color)

    vecteurs2.append((26,20, 14, 48,'blue'))

    for Ax,Ay,Bx, By, color in vecteurs2:
        print("=================================================")
        l=1;
        ref=0;
        if(vecteurs2[i][2]<72/2):
            print("Cas",i,": vecteur de gauche, on cherche celui de droite")
        else:
            ref=1;
            l=-1;
            print("Cas",i,": vecteur de droite, on cherche celui de gauche")
        omega = distance(vecteurs2[i][2],vecteurs2[i][3],vecteurs2[i][0],vecteurs2[i][1])/distance(vecteurs[ref][2],vecteurs[ref][3],vecteurs[ref][0],vecteurs[ref][1])
        dist_1 = omega*distance(vecteurs[1][2],vecteurs[1][3],vecteurs[0][2],vecteurs[0][3])
        dy_1 = vecteurs2[i][3]-vecteurs2[i][1]
        dx_1 = vecteurs2[i][2]-vecteurs2[i][0]
        
        beta_2 = np.arctan(dx_1/dy_1)
        beta_1 = pi+l*beta_2-abs(beta_ref[ref])
        
        #inutile, sert à tracer le deuxième vecteur
        dy_pt1 = np.cos(beta_1)*dist_1
        dx_pt1 = np.sin(beta_1)*dist_1
        
        y_pt1 = vecteurs2[i][3]+dy_pt1
        x_pt1 = vecteurs2[i][2]+l*dx_pt1
        #fin inutile
        dist_2 = omega*distance(vecteurs[1][0],vecteurs[1][1],vecteurs[0][0],vecteurs[0][1])
        sigma = pi/2 - beta_1;
        #inutile, sert à tracer le deuxième vecteur
        dy_pt2 = dist_2*sin(sigma)
        dx_pt2 = dist_2*cos(sigma)
        
        y_pt2 = vecteurs2[i][1]+dy_pt2
        x_pt2 = vecteurs2[i][0]+l*dx_pt2
        vecteurs3.append((x_pt2,y_pt2, x_pt1, y_pt1,'green'))
        #fin inutile
        
        
        milieu_bas_1 = dist_1/2
        milieu_haut_1 = dist_2/2
        
        dy_milieu_bas_1 = np.cos(beta_1)*milieu_bas_1
        dx_milieu_bas_1 = np.sin(beta_1)*milieu_bas_1
        
        y_milieu_bas_1 = vecteurs2[i][3]+dy_milieu_bas_1
        x_milieu_bas_1 = vecteurs2[i][2]+l*dx_milieu_bas_1
        
        
        dy_milieu_haut_1 = milieu_haut_1*sin(sigma)
        dx_milieu_haut_1 = milieu_haut_1*cos(sigma)
        
        y_milieu_haut_1 = vecteurs2[i][1]+dy_milieu_haut_1
        x_milieu_haut_1 = vecteurs2[i][0]+l*dx_milieu_haut_1
        vecteurs4.append((x_milieu_haut_1,y_milieu_haut_1, x_milieu_bas_1, y_milieu_bas_1,'pink'))
        
        angle_route1 = np.arctan((y_milieu_haut_1-y_milieu_bas_1)/(x_milieu_haut_1-x_milieu_bas_1))
        signe_angle_route1 = -angle_route1/abs(angle_route1)
        angle_virage1=signe_angle_route1*pi/2+angle_route1
        if(signe_angle_route1 > 0):
            print("Tourner à gauche de",abs(angle_virage1*180/pi),"°")
        else:
            print("Tourner à droite de",abs(angle_virage1)*180/pi,"°")
        i=i+1






    for Ax,Ay,Bx, By, color in vecteurs2:
        plt.arrow(Ax, Ay, Bx-Ax, By-Ay, head_width=0.5, head_length=0.5,color=color)
    for Ax,Ay,Bx, By, color in vecteurs3:
        plt.arrow(Ax, Ay, Bx-Ax, By-Ay, head_width=0.5, head_length=0.5,color=color)

    for Ax,Ay,Bx, By, color in vecteurs4:
        plt.arrow(Ax, Ay, Bx-Ax, By-Ay, head_width=0.5, head_length=0.5,color=color)

# Invert the y-axis
ax = plt.gca()
ax.set_xlim([0, 72])
ax.set_ylim([0, 51])
ax.invert_yaxis()
# Display the plot
get_angle()
plt.show()
