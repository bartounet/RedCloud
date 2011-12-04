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
bundlerDir = sys.argv[2]

GPS, NGps = gps.extractDonneesGPS(PhotoFolder)

C_RT = np.zeros((NGps, 4))

             #Calcul des coordonnees
for i in xrange(NGps) :
    (C_RT[i,:3],C_RT[i,3]) = cfg.calculXYZfromGPS(GPS[i,0], GPS[i,1], GPS[i,2])
	

outFile = open(bundlerDir + "\\camera-constraints.txt", 'w')

for i in xrange(NGps) :
#            print vert
		line = "" + (str)(i) + " " + (str)(C_RT[i, 0]) + " " + (str)(C_RT[i, 1]) + " " + (str)(C_RT[i, 2]) + " 1   1  1 \n"
		outFile.write(line)