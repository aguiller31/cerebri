import serial # Librairie pour récupérer les données de la communication série
import matplotlib.pyplot as plt # Librairie graphique
import drawnow # Librairie pour rafraîchir une figure matplotlib
import json
import re
import matplotlib.gridspec as gridspec
import csv
import time
vecteurs = []
my_title = 'Vecteurs' # Préciser ici votre titre de graphique
my_ylabel = 'Y' # Titre des ordonnées
my_xlabel = 'X' # Titre des abscisses
my_style = 'ro-' # Style de la courbe matplotlib
nb_points = 100 # Nombre de points sur le graphique
arduino_port = 'COM16' # Préciser ici le port série. Ex. : sous Windows "COM4", sous linux "/dev/ttyACM0" ou "/dev/ttyUSB0"
arduino_baudrate = 115200 # La fréquence de la communication série (baudrate). Doit être identique à celui du code .ino
angle=[]
speed=[]
steer=[]
steer_gain=[]
def is_json(myjson):
  try:
    json.loads(myjson)
  except ValueError as e:
    return False
  return True

# Fonction principale
def main():
  
    # Gestion de l'interruption (fermeture de la fenêtre)
    def handle_close(evt): 
        
        # Check if data exists
        if angle and speed and steer and steer_gain:
            filename = f"data_{int(time.time())}.csv"  # Generate unique filename
            print(f"Saving data to: {filename}")

            # Create CSV header
            header = ", ".join(["angle", "speed", "steer", "steer_gain"])

            # Combine data into a single list of lists for easy writing
            data_to_save = list(zip(angle, speed, steer, steer_gain))

            try:
                with open(filename, 'w', newline='') as csvfile:
                    writer = csv.writer(csvfile)
                    #writer.writerow(header.split(", "))  # Write header row
                    writer.writerows(data_to_save)  # Write data rows
            except Exception as e:
                print(f"Error saving data: {e}")
        else:
            print("No data to save.")
            print('Closed Figure!')
        arduinoData.flush()
        arduinoData.close()
    fig = plt.figure()
    fig.canvas.mpl_connect('close_event', handle_close)
    
    gs = gridspec.GridSpec(1, 2, width_ratios=[2, 8]) 
    fig.set_facecolor('#212121')
    
    def makeFig(): # Fonction pour tracer le graphique
         # Utiliser gridspec pour définir les ratios de hauteur

        ax0 = plt.subplot(gs[0])
        
        ax0.set_xlim([0, 72])
        ax0.set_ylim([0, 51])
        ax0.invert_yaxis()
        ax0.set_xlabel('X',color="white")
        ax0.set_ylabel('Y',color="white")
        ax0.set_title('Vecteurs',color="white")
        ax0.set_aspect('equal')
        ax0.tick_params(colors="white")
        ax0.set_facecolor("#212121")
        for Ax,Ay,Bx, By, color in vecteurs:
            ax0.arrow(Ax, Ay, Bx-Ax, By-Ay, head_width=0.5, head_length=0.5,color=color)
            
        ax1 = plt.subplot(gs[1])
        ax1.set_xlabel('T',color="white")
        ax1.set_ylabel('%',color="white")
        ax1.set_title('Données',color="white")
        #ax1.set_xlim(0, 100)
        #ax1.set_ylim(-120, 120)
        ax1.plot(angle,label='angle')

        ax1.plot(steer,label='steer')

        ax1.plot(speed,label='speed')
        ax1.plot(steer_gain,label='steer_gain')
        ax1.tick_params(colors="white")
        ax1.legend()
        ax1.set_facecolor("#212121")
        plt.tight_layout()
        #plt.show()
    makeFig()
    arduinoData = serial.Serial(arduino_port, arduino_baudrate) # Lancement de la communication série avec Arduino
    plt.ion() # Mode interactif
    cnt = 0 # Compteur du nombre de points
    while True: 
        while (arduinoData.inWaiting()==0): # Vérification que l'on reçoit quelque chose
            pass 

        arduinoString = arduinoData.readline().decode().replace('\t','')# Récupération du contenu de la ligne
        #if "Timestamp" in arduinoString and "steer_gain" in arduinoString:
        # print(arduinoString)
        if "data - " in arduinoString and "- dataend" in arduinoString:
          #data = arduinoString.replace("data - ", "").replace(" - dataend", "").split(" - ")
          
          
          m=re.search('data - (.*) - dataend',arduinoString);
            #print(m)
          if m != None:
              arduinoString=m.group(1)
              data = arduinoString.split(" - ")
                #print(arduinoString);
                #if is_json(arduinoString) == True:
                    #data = json.loads(arduinoString)

                    #try:
              vecteurs.append((float(data[0]),float(data[1]), float(data[2]), float(data[3]),'red'))
              vecteurs.append((float(data[4]),float(data[5]), float(data[6]), float(data[7]),'red'))
              angle.append(float(data[8]))
              speed.append(float(data[10]))
              steer.append(float(data[9]))
              steer_gain.append(float(data[11]))
              drawnow.drawnow(  makeFig  ) # Rafraichissement de la figure matplotlib
              cnt=cnt+1
              vecteurs.pop(1) # Si plus de nb_points, enlever la première valeur de la liste data
              vecteurs.pop(0)
                        #if(cnt>nb_points):
                            
                            #angle.pop(0)
                            #speed.pop(0)
                            #steer.pop(0)
                            #steer_gain.pop(0)
                    #except:
                      #print("An exception occurred")
        
# Fenêtre principale et appel de la fonction main
if __name__ == '__main__':
  main()
