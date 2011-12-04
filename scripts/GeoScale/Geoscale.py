#! /bin/python

import numpy as np
import math
import calculXYZfromGPS as cfg
import scriptMoindresCarres as mc
import scriptRepositionnement as re
import extractDonneesGPS as gps
import extractDonneesArc3D as arc
import bundler_out as bo
import applyTransformation as app
import sys

PhotoFolder = sys.argv[1]
bundlerOut = sys.argv[2]
outFile = sys.argv[3]
outGeo = sys.argv[4]
#ObjInput = sys.argv[3]
#ObjOutput = sys.argv[4]

#R, t, goodCams = bo.extractBundlerCams(bundlerOut)
goodCamsF, goodCamsR, goodCamsT, t, goodCams, PointsP, PointsC, pointsCamList = bo.extractBundlerData(bundlerOut)
GPS, NGps = gps.extractDonneesGPS(PhotoFolder, goodCams)
N = len(goodCams)

# Determination des coordonnees de Ci et Og dans RT

#CALCUL DES COORDONNEES DES POINTS Ci DANS LE REPERE RT

#Initialisation d'une matrice (Nx4) contenant les coordonnees xi, yi, zi
#dans le repere RT du point Ci et le rayon de la Terre Ri associe a la
#camera i
C_RT = np.zeros((N, 4))

             #Calcul des coordonnees
for i in xrange(N) :
	(C_RT[i,:3],C_RT[i,3]) = cfg.calculXYZfromGPS(GPS[i,0], GPS[i,1], GPS[i,2])

	

#CALCUL DES COORDONNEES DE Og DANS LE REPERE RT

"""Initialisation d'une matrice (Nx4) contenant les coordonnees xi, yi, zi
%dans le repere RT du point C*i (hauteur hi=0) et le rayon de la Terre Ri
%associe a la camera i"""
C0_RT = np.zeros((N, 4));

for i in xrange(N) :
	(C0_RT[i,:3],C_RT[i,3]) = cfg.calculXYZfromGPS(GPS[i,0], GPS[i,1], 0)


#Calcul des coordonnees
Og_RT = np.transpose(np.mean(C0_RT[:,:3], axis=0)) #Og_RT est defini comme un vecteur colonne


outGeoFile = open(outGeo + "/point.gps", 'w')
phi = math.acos(math.sqrt(Og_RT[0]**2 + Og_RT[1]**2)/math.sqrt(Og_RT[0]**2 + Og_RT[1]**2 + Og_RT[2]**2))
print Og_RT[0]/math.sqrt(Og_RT[0]**2 + Og_RT[1]**2)
lanbda = math.acos(Og_RT[0]/math.sqrt(Og_RT[0]**2 + Og_RT[1]**2))
print math.degrees(phi)
print math.degrees(lanbda)
print "---"
print GPS[0]
outGeoFile.write(str(math.degrees(phi)) + " " + str(math.degrees(lanbda)))


## Determination de la matrice de passage de la base BT a la base Bg

#CALCUL DES FACTEURS COSINUS/SINUS(LONGITUDE/LATITUDE)

#Calcul de sqrt(XT(Og)^2+YT(Og)^2)
g1 = np.sqrt(Og_RT[0]**2 + Og_RT[1]**2)

#Calcul de sqrt(XT(Og)^2+YT(Og)^2+ZT(Og)^2)
g2 = np.sqrt(Og_RT[0]**2 + Og_RT[1]**2 + Og_RT[2]**2)

#Calcul des facteurs
coslat = g1 / g2
sinlat = Og_RT[2] / g2
coslong = Og_RT[0] / g1
sinlong = Og_RT[1] / g1

#Calcul des angles
lat = np.arcsin(np.radians(sinlat))
long = np.arccos(np.radians(coslong))

#CALCUL DE LA MATRICE DE PASSAGE

P_BT_Bg = np.array([(-coslong*sinlat, -sinlong*sinlat, coslat),
					(sinlong, -coslong, 0),
					(coslong*coslat, coslat*sinlong, sinlat)])

print P_BT_Bg

## Determination des vecteurs OgCi dans la base Bg

#Initialisation d'une matrice (Nx3) contenant les composantes dans la base
#Bg des vecteurs OgCi
OgC_Bg = np.zeros((N, 3))

#Calcul des composantes
for i in xrange(N) :
	OgC_Bg[i,:] = np.dot(P_BT_Bg, (np.transpose(C_RT[i, 0:3]) - Og_RT))

## Determination de la matrice de passage de la base BA a la base Bg

#CALCUL DU FACTEUR D'ECHELLE

delta_gps=10 #Incertitude absolue du GPS Nikon GP-1 (10 metres)

#Initialisation de deux matrices M(i,j) ou M(i,j)=norm(t(i)-t(j))
Echelle_BT = np.zeros((N, N))
Echelle_BA = np.zeros((N, N))

#Echelles dans les bases BA et BT
for i in xrange(N) :
	for j in xrange(i, N) :
		Echelle_BT[i,j] = np.linalg.norm(C_RT[i,0:3] - C_RT[j,0:3])
		Echelle_BA[i,j] = np.linalg.norm(t[i,:] - t[j,:])

             #Stockage des normes Echelle_BT(i,j) et Echelle_BA(i,j) dans deux vecteurs C_list
             #et T_list (plutot que des matrices) ou ne ne garde que les
             #Echelle_BT(i,j)>crit
crit = 300 # Valeur a determiner apres etude
C_list = np.zeros((1,N*N))
T_list = np.zeros((1,N*N))
k=0
for i in xrange(N) :
	for j in xrange(i + 1, N) :
		#print Echelle_BT[i, j]
		if Echelle_BT[i, j] > crit :
			C_list[0, k] = Echelle_BT[i, j]
			T_list[0, k] = Echelle_BA[i, j]
			k=k+1

C_list.resize(k)
T_list.resize(k)

Nc = k # Nombre de couples utilises

#Calcul de Echelle_T_A_moy (facteur d'echelle)
Echelle_T_A_moy = sum(C_list / T_list) / len(C_list);

#Calcul de deltaRg
deltaRg = 2*delta_gps / len(C_list) * sum(1 / T_list);

# Calcul de deltaRg/Echelle_T_A_moy (incertitude relative du rapport
# d'echelle due a l'incertitude GPS)
Uncert_Rel_Rg = deltaRg / Echelle_T_A_moy;

#CALCUL DE LA MATRICE DE PASSAGE

#Mise a l'echelle des donnees Arc3d
#print Echelle_T_A_moy


t = t * Echelle_T_A_moy;

t = re.repositionnement(N, OgC_Bg, t)

#On utilise alors la methode des moindres carres sur les nouvelles donnees
#repositionnees

P_BA_Bg = mc.moindresCarres(N, OgC_Bg, t)

G_g = np.mean(OgC_Bg, axis=0) #G_g vecteur ligne
G_A = np.mean(t[:,0:3], axis=0) #G_A vecteur ligne

#Retablissement des t
#t = t / Echelle_T_A_moy;

#Calcule translation
vTrans = np.zeros( (3, 1) )

G_g = np.reshape(G_g, (3, 1))
G_A = np.reshape(G_A, (3, 1))
vTrans[:] = G_g - np.dot(P_BA_Bg, G_A)

print np.dot(P_BA_Bg, np.transpose(P_BA_Bg))

#app.applyTransformation(ObjInput, ObjOutput, P_BA_Bg, vTrans, Echelle_T_A_moy, OgC_Bg)
bo.applyTransformation(outFile, goodCamsF, goodCamsR, goodCamsT, t, goodCams, PointsP, PointsC, pointsCamList, P_BA_Bg, vTrans, Echelle_T_A_moy)
#bo.applyTransformation(goodCamsF, goodCamsR, goodCamsT, t, goodCams, PointsP, PointsC, pointsCamList, np.eye(3), vTrans, 1)
#bo.applyTransformation(goodCamsF, goodCamsR, goodCamsT, t, goodCams, PointsP, PointsC, pointsCamList, P_BA_Bg, np.zeros((3, 1)), 1)