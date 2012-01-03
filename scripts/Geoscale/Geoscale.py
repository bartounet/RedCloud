#! /bin/python

import numpy as np
import math
import scriptMoindresCarres as mc
import scriptRepositionnement as re
import ExtractData as ed
import Tools as tools

def doGeoscale(PhotoFolder, bundlerOut, outGeo, plyIn, plyOut):
	bundlerCams, goodCams = ed.extractBundlerData(bundlerOut)
	photosCams_gps = ed.extractPhotosData(PhotoFolder, goodCams)

	photosCams_local, center_earth = tools.localFromGPS(photosCams_gps)

	scalling = tools.computeScaling(bundlerCams, photosCams_local)

	bundlerCams_scaled = bundlerCams * scalling;
	bundlerCams_rp = re.repositionnement(photosCams_local, bundlerCams_scaled)
	rotation = mc.moindresCarres(photosCams_local, bundlerCams_rp)

	translation = tools.computeTranslation(photosCams_local, bundlerCams_rp, rotation)

	tools.applyTransformation(plyIn, plyOut, rotation, translation, scalling)
	tools.writeGeoData(center_earth, outGeo)
