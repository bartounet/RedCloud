#! /usr/bin/python

import os
import re
import sys
import numpy as np

def applyTransformation(inFileName, outFileName, mRot, vTrans, scale, cams):

	inFile = open(inFileName)
	outFile = open(outFileName, 'w')
	for cam in range(len(cams)):
		outFile.write( "v " + " ".join(map(str, np.reshape(cams[cam], 3))) + "\n")
	for line in inFile:
		sLine = line.split()
		if len(sLine) and sLine[0] == "v":
			vert = np.zeros((3, 1))
			vert[0, 0] = (float)(sLine[1])
			vert[1, 0] = (float)(sLine[2])
			vert[2, 0] = (float)(sLine[3])
#            print vert
			vert = (np.dot(mRot, vert * scale) + vTrans)
#            print vert
			#vert = np.dot(mRot, vert * scale)
			sLine[1:] = map(str, np.reshape(vert, 3))
			newLine = " ".join(sLine)
			newLine += "\n"
			outFile.write(newLine)
 #           print newLine
		else:
			outFile.write(line)
	return

#applyTransformation(sys.argv[1], sys.argv[2], np.identity(3), np.zeros((3, 1)), 1)

