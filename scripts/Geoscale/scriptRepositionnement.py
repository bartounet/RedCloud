import numpy as np
import math

def repositionnement(OgC_Bg, t) :
	N = len(OgC_Bg)
	G_g = np.mean(OgC_Bg, axis=0) #G_g vecteur ligne
	G_A = np.mean(t, axis=0) #G_A vecteur ligne

	#Tableau GgC
	GgC = OgC_Bg - np.tile(G_g, (N, 1))
	tt = np.zeros((N, 3))

	#Calcul de A1 (Arc3D) a partir de C1(GPS)
	tt[0,:] = np.linalg.norm(GgC[0,:]) / np.linalg.norm(t[0,:] - G_A) * (t[0,:] - G_A)
	#Base (e10,e20,e30) dans Rg
	e10_G = GgC[0,:] / np.linalg.norm(GgC[0,:])
	e20_G = (GgC[1,:] - np.dot(GgC[1,:], e10_G) * e10_G) / np.linalg.norm(GgC[1,:] - np.dot(GgC[1,:], e10_G) * e10_G)
	e30_G = np.cross(e10_G, e20_G)
		
	#Calcul de A2 en supposant les plans (A1G_AA2) et (C1G_gC2) coincidant
	GA1 = t[0,:] - G_A
	GA2 = t[1,:] - G_A
	e10 = GA1 / np.linalg.norm(GA1)
	e20 = (GA2 - np.dot(GA2,e10) * e10) / np.linalg.norm(GA2 - np.dot(GA2, e10) * e10)
	e30 = np.cross(e10, e20)
	cos = np.dot(GgC[0,:], GgC[1,:]) / (np.linalg.norm(GgC[0,:]) * np.linalg.norm(GgC[1,:]))
	sin = np.linalg.norm(np.cross(GgC[0,:], GgC[1,:])) / (np.linalg.norm(GgC[0,:]) * np.linalg.norm(GgC[1,:]))
	tt[1,:] = np.linalg.norm(GgC[1,:]) * cos * e10 + np.linalg.norm(GgC[1,:]) * sin * e20

	#Calcul des Ai a partir des Ci et C1
	for i in range (2, N) :
		e2_G = (GgC[i,:] - np.dot(GgC[i,:], e10_G) * e10_G) / np.linalg.norm(GgC[i,:] - np.dot(GgC[i, :], e10_G) * e10_G)
		e3_G = np.cross(e10_G, e2_G)
		e3_A = np.dot(e3_G, e10_G) * e10 + np.dot(e3_G, e20_G) * e20 + np.dot(e3_G, e30_G) * e30
		e2_A = np.cross(e3_A, e10)
			
		cos = np.dot(GgC[0,:], GgC[i,:]) / (np.linalg.norm(GgC[0,:]) * np.linalg.norm(GgC[i,:]))
		sin = np.linalg.norm(np.cross(GgC[0,:],GgC[i,:]))/(np.linalg.norm(GgC[0,:]) * np.linalg.norm(GgC[i,:]))
		tt[i,:] = np.linalg.norm(GgC[i,:]) * cos * e10 + np.linalg.norm(GgC[i,:]) * sin * e2_A
			
	ttt = np.zeros((N, 3))
	ttt[:,0:3] = t[:,0:3] - np.tile(G_A, (N, 1))
			
	t[:,0:3] = tt[:,0:3]+ np.tile(G_A, (N, 1))
		
	return t
