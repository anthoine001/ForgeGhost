import serial
import sys
import string
import numpy as np
from scipy import fftpack
from matplotlib import pyplot as plt

# intialisation de la communication serie
print("Init")
try:
    ser = serial.Serial('/dev/ttyACM0', 9600)
except:
    ser = serial.Serial('/dev/ttyACM1', 9600)

def valeur(a):
    return data[0:len(a)-2]
resultats=list(range(600))
reception = 0
while True:
    try:
        if reception==0:
            for i in range(0,3):
                data = ser.readline()
                print valeur(data)
            print("lecture des 3 premiers paquets terminee")
            freq =float(valeur(data))*1000
            print (freq," Hz")
            for i in range (0,600,1):
                resultats[i] = float(valeur(ser.readline()))
            #ligne de controle
            print resultats[0]
            print resultats[599]
            #/
            print("reception terminee")
            reception = 0
            t=np.arange(1.,601)/freq
            axe_f = np.arange(0.,600)*freq/600
            fourier = fftpack.fft(resultats)/np.size(resultats)
            print("analyse spectrale terminee")
            # traitement graphique
            plt.figure()
            plt.subplot(121)
            plt.plot(t,resultats,'-')
            plt.subplot(122)
            plt.plot(axe_f,np.abs(fourier),'x-')
            plt.show()
    except:
        print"Unexpected error:", sys.exc_info()
        sys.exit()
