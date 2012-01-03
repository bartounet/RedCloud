import numpy as np
import math

def earthFromGPS(longi, lat, h) :
	rlong = math.radians(longi)
	rlat = math.radians(lat)
	re = 6378137
	rp = 6356752.3142
	Rterre = (rp**2 / (1 + (math.cos(rlat))**2 * (rp**2 / re**2 - 1)))**0.5;
	M_T = [(Rterre+h) * math.cos(rlat)*math.cos(rlong),
			(Rterre+h) * math.cos(rlat)*math.sin(rlong),
			(Rterre+h) * math.sin(rlat)];
	return M_T
	
def localFromGPS(photosCams_gps):
	N = len(photosCams_gps)
    #CALCUL DES COORDONNEES DES POINTS Ci DANS LE REPERE RT
	C_RT = np.zeros((N, 3))
	for i in xrange(N):
		C_RT[i,:] = earthFromGPS(photosCams_gps[i,0], photosCams_gps[i,1], photosCams_gps[i,2])

    #CALCUL DES COORDONNEES DE Og DANS LE REPERE RT
	C0_RT = np.zeros((N, 3));
	for i in xrange(N) :
		C0_RT[i,:] = earthFromGPS(photosCams_gps[i,0], photosCams_gps[i,1], 0)
	Og_RT = np.transpose(np.mean(C0_RT, axis=0))	
	
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

    ## Determination des vecteurs OgCi dans la base Bg

    #Initialisation d'une matrice (Nx3) contenant les composantes dans la base
    #Bg des vecteurs OgCi
	OgC_Bg = np.zeros((N, 3))

    #Calcul des composantes
	for i in xrange(N) :
	    OgC_Bg[i,:] = np.dot(P_BT_Bg, (np.transpose(C_RT[i, :]) - Og_RT))
		
	return OgC_Bg, Og_RT
	

def computeScaling(bundlerCams_bundler, photosCams_local):
	N = len(bundlerCams_bundler)
	Echelle_BT = np.zeros((N, N))
	Echelle_BA = np.zeros((N, N))

    #Echelles dans les bases BA et BT
	for i in xrange(N) :
		for j in xrange(i, N) :
			Echelle_BT[i,j] = np.linalg.norm(photosCams_local[i, :] - photosCams_local[j, :])
			Echelle_BA[i,j] = np.linalg.norm(bundlerCams_bundler[i,:] - bundlerCams_bundler[j,:])

	crit = 300
	C_list = np.zeros((1,N*N))
	T_list = np.zeros((1,N*N))
	k=0
	for i in xrange(N) :
		for j in xrange(i + 1, N) :
			if Echelle_BT[i, j] > crit :
				C_list[0, k] = Echelle_BT[i, j]
				T_list[0, k] = Echelle_BA[i, j]
				k=k+1
	C_list.resize(k)
	T_list.resize(k)

	Nc = k # Nombre de couples utilises

    #Calcul de Echelle_T_A_moy (facteur d'echelle)
	Echelle_T_A_moy = sum(C_list / T_list) / len(C_list);
	return Echelle_T_A_moy
	
def computeTranslation(bundlerCams_bundler, photosCams_local, rotation):
	G_g = np.mean(photosCams_local, axis=0) #G_g vecteur ligne
	G_A = np.mean(bundlerCams_bundler, axis=0) #G_A vecteur ligne

	translation = np.zeros( (3, 1) )
	G_g = np.reshape(G_g, (3, 1))
	G_A = np.reshape(G_A, (3, 1))
	translation[:] = G_g - np.dot(rotation, G_A)
	return translation
	
def applyTransformation(inFileName, outFileName, mRot, vTrans, scale):
	inFile = open(inFileName)
	outFile = open(outFileName, 'w')
	header = True
	for line in inFile:
		if header:
			outFile.write(line)
		else:
			sLine = line.split()
			
			v = np.zeros((3, 1))
			v[0, 0] = (float)(sLine[0])
			v[1, 0] = (float)(sLine[1])
			v[2, 0] = (float)(sLine[2])
			v = (np.dot(mRot, v * scale) + vTrans)
			
			n = np.zeros((3, 1))
			n[0, 0] = (float)(sLine[3])
			n[1, 0] = (float)(sLine[4])
			n[2, 0] = (float)(sLine[5])
			n = np.dot(mRot, n)
			
			sLine[0:3] = map(str, np.reshape(v, 3))
			sLine[3:6] = map(str, np.reshape(n, 3))
			newLine = " ".join(sLine)
			newLine += "\n"
			outFile.write(newLine)			
		if "end_header" in line:
			header = False

def writeGeoData(Og_RT, fileName):
	outGeoFile = open(fileName, 'w')
	phi = math.acos(math.sqrt(Og_RT[0]**2 + Og_RT[1]**2)/math.sqrt(Og_RT[0]**2 + Og_RT[1]**2 + Og_RT[2]**2))
	lanbda = math.acos(Og_RT[0]/math.sqrt(Og_RT[0]**2 + Og_RT[1]**2))
	outGeoFile.write(str(math.degrees(phi)) + " " + str(math.degrees(lanbda)))