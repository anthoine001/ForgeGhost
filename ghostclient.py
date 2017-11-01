
#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
# Version de decembre 2016 incluant la fonction taille de lot jusque 49



try:
    from tkinter import  *
except:
    from Tkinter import  *
import socket
import argparse

def marquerNiveau(x,y):
#procedure pour dessiner le marqueur rond a l'emplacement x,y
    canvas.coords(marqueur,taille*x,hauteur/2-taille*(1-y),taille*(x+1),hauteur/2-taille*(2-y))
    
parser = argparse.ArgumentParser(description='wipbox client')
parser.add_argument('--ip', nargs=1, help='adresse ip')
parser.add_argument('--port', type=int, help='numero de port')
parser.add_argument('--stock', type=int, help='stock')
#ATTENTION : nous allons coder les noms de machine sur 6 caracteres obligatoirement
parser.add_argument('--nom', type=str, help='nom de la machine')
args = parser.parse_args()

if(args.ip is not None):
    serveur_ip = args.ip[0]
serveur_port = 1111
if(args.port is not None):
    serveur_port = args.port
#ATTENTION ne pas depasser 49
stock=29
if(args.stock is not None):
    stock = args.stock

print("adress " + serveur_ip + "[" + str(serveur_port) + "]" )


#Definition de la taille de l ecran et du stock
fenetre = Tk()
threshold=input ("quelle est le seuil d'alerte pour cette machine ? ")
#protection sur le nombre de cellules du tableau
if stock>49:
    stock = 49
lTab = stock//10+1
print("le tableau aura ",lTab," lignes")
largeur =fenetre.winfo_screenwidth()
hauteur=fenetre.winfo_screenheight()
taille=largeur/10
level =0

#initialisation de la fenetre
fenetre.title(args.nom)
fenetre.resizable(0,0)
canvas = Canvas(fenetre, width=largeur, height=hauteur, background='black')
try:
    photo = PhotoImage(file="ntn.jpeg")
    canvas.create_image(0, 0, anchor=NW, image=photo)
except:
    print ("load img fail")
coordx=level


#dessin du tableau
i=0
for i in range(stock+1):
    if i<int(threshold):
        couleur='lime green'
    elif i==int(threshold):
        couleur='sandybrown'
    elif i>int(threshold):
        couleur='indianred3'
    j= i//10
    canvas.create_rectangle(taille*(i-10*j),hauteur/2-(2-j)*taille,taille*(i-10*j+1),hauteur/2-(1-j)*taille, fill=couleur,outline='white')
    canvas.create_text(taille*(0.5+i-10*j),hauteur/2-(1.5-j)*taille,justify=CENTER,text=i,font="arial 18 bold")
#position initiale du curseur    
marqueur=canvas.create_oval(0,0,0,0,outline='cornflower blue',width=16)
marquerNiveau(0,0)
canvas.pack()


def send_level(level):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((serveur_ip, serveur_port))
    print (level)
    if level<10:
        codebyte=("0"+str(level)).encode('ascii')
    elif level>=10:
        codebyte=str(level).encode('ascii')
    nombyte=str(args.nom).encode('ascii')
    thresholdbyte = str(threshold).encode('ascii')
    print (codebyte)
    print (nombyte)
    s.send((nombyte) + (codebyte) + (thresholdbyte))
    

#Detection du clic, de la position et positionnement d'un symbole
def touche(event):
    if hauteur/2-2*taille<event.y<hauteur/2-(2-lTab)*taille:
        # Capturer la case qui a ete cliquee
        coordx=int(event.x/taille)
        coordy=int((event.y-(hauteur/2-2*taille))/taille)
        print(coordx,coordy)
        level=coordx+10*coordy
        if level<=stock:
            marquerNiveau(coordx, coordy)
            print("le niveau de stock est :",level)
            send_level(level)
        else :
            print("erreur de click")
canvas.bind("<Button-1>", touche)


fenetre.mainloop()
