import numpy as np
import math

"""
blkdiag de Matlab
"""

def blkdiag(*args):
	shapes = [m.shape for m in args]
	totalh, totalw = reduce(lambda a,b: (a[0]+b[0],a[1]+b[1]) , shapes, (0,0))
	bd = np.zeros((totalh, totalw))
	hpointer, wpointer = 0, 0
	for m in args:
		bd[hpointer:hpointer+m.shape[0], wpointer:wpointer+m.shape[1]] = m
		hpointer += m.shape[0]
		wpointer += m.shape[1]
	return bd

"""
Determination de P_BA_Bg par les moindres carres
Definition du cardinal de I (=nbre total de couples de points)
"""

def moindresCarres(OgC_Bg, t) :
	N = len(OgC_Bg)
	Nt = N*(N-1)/2
# Calcul de At des (ci_A)1, (ci_A)2, (ci_A)3 pour i=1..Nt
	At = np.zeros((Nt, 3))
	k = 0
	for i in xrange(N) :
		for j in xrange(i + 1, N) :
			At[k,:] = t[i,:] - t[j,:]
			k = k + 1

# Determination de A
	A = blkdiag(At, At, At)

# Determination de y
	y = np.zeros((3*Nt, 1))
	k = 0
	for i in xrange(N) :
		for j in xrange(i + 1, N) :
			y[k] = OgC_Bg[i, 0] - OgC_Bg[j, 0]
			y[Nt+k] = OgC_Bg[i, 1] - OgC_Bg[j, 1]
			y[2*Nt+k] = OgC_Bg[i, 2] - OgC_Bg[j,2]
			k = k + 1
	Nt = N*(N-1)/2

#Resolution du systeme au sens des moindres carres
	x, _, _, _ = np.linalg.lstsq(A,y)
	
#Determination de la matrice de passage
	P_BA_Bg = np.array([[x[0, 0], x[1, 0], x[2, 0]],
						[x[3, 0], x[4, 0], x[5, 0]],
						[x[6, 0], x[7, 0], x[8, 0]]])
	
	return P_BA_Bg
