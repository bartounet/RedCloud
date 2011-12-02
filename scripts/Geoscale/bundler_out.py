#! /usr/bin/python

# EXTRACTION DES DONNEES ARC3D
import os
import re
import sys
import numpy as np

def extractDonneesBundler(fileName):

	f = open(fileName, "r")
	
	f.readline()
	[nbCam, nbPoint] = map(float, f.readline().split())
	nbCam = (int)(nbCam)
	nbPoint = (int)(nbPoint)
	
	R = np.zeros( (3, 3, nbCam) )
	t = np.zeros( (nbCam, 3) )
	CamsNbPoint = np.zeros(nbCam)
	
	for i in range(nbCam):
		f.readline()
		R[:, 0, i] = map(float, f.readline().split())
		R[:, 1, i] = map(float, f.readline().split())
		R[:, 2, i] = map(float, f.readline().split())
		t[i, :] = map(float, f.readline().split())
		
	print t[0]
	print t[nbCam - 1]
	for i in range(nbPoint):
		f.readline()
		f.readline()	
		camsList = map(float, f.readline().split())
		for cam in range((int)(camsList[0])):
			CamsNbPoint[(int)(camsList[cam * 4 + 1])] += 1

			
	sortCamsNbPoint = np.sort(CamsNbPoint)
	for i in range(nbCam):
		print("cam " + (str)(i) + " - " + (str)(sortCamsNbPoint[i]))
		
	return [R, t, CamsNbPoint, nbCam]

#Initialisation d'une matrice rotation R (3x3xN) et d'un vecteur
#translation t (Nx3)
#R = zeros([3 3 N]);
#t = zeros([N 3]);

    #R = np.zeros( (3, 3, N) )
    #t = np.zeros( (N, 3) )

#Lecture des N fichiers camera

                #R[:, 1, i] = map(float, l.split())

def extractBundlerCams(fileName):

	f = open(fileName, "r")
	
	f.readline()
	[nbCam, nbPoint] = map(float, f.readline().split())
	nbCam = (int)(nbCam)
	nbPoint = (int)(nbPoint)
	
	R = np.zeros( (3, 3, nbCam) )
	t = np.zeros( (nbCam, 3) )
	CamsNbPoint = np.zeros(nbCam)
	
	outFile = open("tmp", 'w')
	
	for i in range(nbCam):
		f.readline()
		R[:, 0, i] = map(float, f.readline().split())
		R[:, 1, i] = map(float, f.readline().split())
		R[:, 2, i] = map(float, f.readline().split())
		t[i, :] = map(float, f.readline().split())
		t[i, :] = np.transpose(- np.dot(R[:, :, i], np.transpose(t[i, :])))
		line = map(str, t[i, :])
		newLine = " ".join(line)
		newLine += " 0 0 255\n"
		outFile.write(newLine)

	for i in range(nbPoint):
		f.readline()
		f.readline()	
		camsList = map(float, f.readline().split())
		for cam in range((int)(camsList[0])):
			CamsNbPoint[(int)(camsList[cam * 4 + 1])] += 1

	goodCams = []
	for i in range(nbCam):
		if CamsNbPoint[i] > -1:
			goodCams.append(i)
	
	goodR = np.zeros( (3, 3, len(goodCams)) )
	goodt = np.zeros( (len(goodCams), 3) )
	
	for i in range(len(goodCams)):
		goodR[:, :, i] = R[:, :, goodCams[i]]
		goodt[i, :] = t[goodCams[i], :]
	
	
	print "selected camera : " + str((len(goodCams) + 0.) / nbCam)
		
	return [goodR, goodt, goodCams]
	
	
def extractBundlerData(fileName):

	f = open(fileName, "r")
	
	f.readline()
	[nbCam, nbPoint] = map(float, f.readline().split())
	nbCam = (int)(nbCam)
	nbPoint = (int)(nbPoint)
	
	camsF = np.zeros( (nbCam, 3) )
	camsR = np.zeros( (3, 3, nbCam) )
	camsT = np.zeros( (nbCam, 3) )
	camsP = np.zeros( (nbCam, 3) )
	CamsNbPoint = np.zeros(nbCam)
	
	# read cams infos
	for i in range(nbCam):
		camsF[i, :] = map(float, f.readline().split())
		camsR[0, :, i] = map(float, f.readline().split())
		camsR[1, :, i] = map(float, f.readline().split())
		camsR[2, :, i] = map(float, f.readline().split())
		camsT[i, :] = map(float, f.readline().split())
		camsP[i, :] = np.transpose(- np.dot(np.transpose(camsR[:, :, i]), np.transpose(camsT[i, :])))

	pointsP = np.zeros( (nbPoint, 3) )
	pointsC = np.zeros( (nbPoint, 3) )
	pointsCamList = []
		
	for i in range(nbPoint):
		pointsP[i, :] = map(float, f.readline().split())
		pointsC[i, :] = map(float, f.readline().split())	
		camsList = map(float, f.readline().split())
		pointCamList = np.zeros( (camsList[0], 4) )
		for cam in range((int)(camsList[0])):
			CamsNbPoint[(int)(camsList[cam * 4 + 1])] += 1
			
			pointCamList[cam, :] = camsList[cam * 4 + 1 : cam * 4 + 5]
			
		pointsCamList.append(pointCamList)

	goodCams = []
	for i in range(nbCam):
		if CamsNbPoint[i] > -1:
			goodCams.append(i)
	
	goodCamsF = np.zeros( (len(goodCams), 3) )
	goodCamsR = np.zeros( (3, 3, len(goodCams)) )
	goodCamsT = np.zeros( (len(goodCams), 3) )
	goodCamsP = np.zeros( (len(goodCams), 3) )
	
	for i in range(len(goodCams)):
		goodCamsF[i, :] = camsF[goodCams[i], :]
		goodCamsR[:, :, i] = camsR[:, :, goodCams[i]]
		goodCamsT[i, :] = camsT[goodCams[i], :]
		goodCamsP[i, :] = camsP[goodCams[i], :]
	
	
	print "selected camera : " + str((len(goodCams) + 0.) / nbCam)
		
	return [goodCamsF, goodCamsR, goodCamsT, goodCamsP, goodCams, pointsP, pointsC, pointsCamList]



def applyTransformation(outFile, goodCamsF, goodCamsR, goodCamsT, t, goodCams, pointsP, pointsC, pointsCamList, mRot, vTrans, scale):

	print mRot
	print vTrans
	print scale

	outFile = open(outFile, 'w')

	outFile.write("# Bundle file v0.3\n")
	outFile.write(str(len(goodCams)) + " " + str(len(pointsP)) + "\n")
	
	#####debug
	outLogFile = open("geo.ply", 'w')
	outLogFile.write("ply\n\
format ascii 1.0\n\
element face 0\n\
property list uchar int vertex_indices\n\
element vertex " + str(len(goodCams) + len(pointsP)) + "\n\
property float x\n\
property float y\n\
property float z\n\
property uchar diffuse_red\n\
property uchar diffuse_green\n\
property uchar diffuse_blue\n\
end_header\n")
	
	##/debug
	
	for i in range(len(goodCams)):
		#goodCamsF[i, 0] *= scale
		outFile.write(" ".join(map(str, goodCamsF[i])) + "\n")
		newR = np.dot(goodCamsR[:, :, i], np.transpose(mRot))
		outFile.write(" ".join(map(str, newR[0, :])) + "\n")
		outFile.write(" ".join(map(str, newR[1, :])) + "\n")
		outFile.write(" ".join(map(str, newR[2, :])) + "\n")
		newT = goodCamsT[i] * scale - np.reshape(np.dot(newR, vTrans), (3))
		#newT = goodCamsT[i] - np.reshape(np.dot(newR, vTrans), (3))
		outFile.write(" ".join(map(str, newT)) + "\n")
		
		outLogFile.write( " ".join(map(str, np.reshape(- np.dot(np.transpose(newR), np.reshape(newT, (3, 1))), 3))) + " 255 0 0\n")
		#outLogFile.write( " ".join(map(str, np.reshape(- np.dot(np.transpose(goodCamsR[:, :, i]), np.reshape(goodCamsT[i], (3, 1))), 3))) + " 255 0 0\n")
		
		
	for i in range(len(pointsP)):
		newP = np.reshape(np.dot(mRot, np.reshape(pointsP[i], (3, 1)) * scale) + vTrans, (3))
		#newP = np.reshape(np.dot(mRot, np.reshape(pointsP[i], (3, 1))) + vTrans, (3))
		outFile.write(" ".join(map(str, newP)) + "\n")
		outFile.write(" ".join(map(str, map(int, pointsC[i]))) + "\n")
		camList = str(len(pointsCamList[i]))
		for cam in range(len(pointsCamList[i])):
			camList += " " + " ".join(map(str, map(int, pointsCamList[i][cam, :2])))
			camList += " " + " ".join(map(str, pointsCamList[i][cam, 2:]))
		camList += "\n"
		outFile.write(camList)
	
		outLogFile.write(" ".join(map(str, np.reshape(newP, 3))) + " " + " ".join(map(str, map(int, np.reshape(pointsC[i], 3)))) + "\n")
	
	return
