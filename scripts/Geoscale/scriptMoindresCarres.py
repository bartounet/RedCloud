#     This file is part of Geoscale tool (see http://www.geoscale.org/).
# 
#     Copyright (C) 2011 O.R.A., Jonathan Cloux (head developer), Benoît Millot
#     Original idea from O.R.A. by Adrien Normier
#     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
#                jonathanclx@gmail.com   
# 
#     Geoscale is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     Geoscale is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
# 
#     You should have received a copy of the GNU General Public License
#     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

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
