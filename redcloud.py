#! /usr/bin/python
#

import sys
#     This file is part of RedCloud tool.
# 
#     Copyright (C) 2012 Munzer Thibaut, Jonathan Aigrain, Adrien Chey, Nicolas Fabretti
#     Original idea from O.R.A. by Adrien Normier
#     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
#                redcloud.contact@gmail.com   
# 
#     Redcloud is free software: you can redistribute it and/or modify
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

import string
import getopt
import os
import shutil
import logging
import time
import subprocess
import Image

sys.path.append('scripts/Geoscale/')
import Geoscale as geoscale

from scripts import osmcmvs
from scripts import osmbundler
from scripts import plyMerger
from scripts import ply2npts
from scripts import daeToKmz
from scripts import plyCut

def Usage() :
	print("Usage : ./redcloud.py <Photos directory> <Output directory>");
	sys.exit(1);

def getArgs() :
	currentPath = os.getcwd()
	if (len(sys.argv) != 3) :
		Usage();
	photoDir = sys.argv[1]
	if not(os.path.isabs(photoDir)):
		photoDir = os.path.join(currentPath, photoDir)
	resultDir = sys.argv[2];
	if not(os.path.isabs(resultDir)):
		resultDir = os.path.join(currentPath, resultDir)

	return (photoDir, resultDir);

# service function: get path of an executable (.exe suffix is added if we are on Windows)
def getBinPath(binaryName):
	if (sys.platform == "win32"):
		binaryName += ".exe"
	return binaryName

def parseOptionFile(filename):
	options = {}
	commentChar = '#'
	optionChar = '='
	f = open(filename)
	for line in f:
		# First, remove comments:
		if commentChar in line:
			# split on comment char, keep only the part before
			line, comment = line.split(commentChar, 1)
		# Second, find lines with an option=value:
		if optionChar in line:
			# split on option char:
			option, value = line.split(optionChar, 1)
			# strip spaces:
			option = option.strip()
			value = value.strip()
			# store in dictionary:
			options[option] = float(value)
	f.close()
	return options

def do(step):
	stepName = step.__name__
	printKiKoo(stepName)
	start = time.time()
	step()
	execTimeMin = (int)((time.time() - start) / 60.)
	execTimeSec = (int)((time.time() - start) % 60)
	execTime = str(str(execTimeMin) + " mins and " + str(execTimeSec) + " secs")
	print "--> Done in: ", execTime, "\n\n"
	benchmarkFile = open(os.path.join(redCouldDir, "Benchmark.txt"), 'a+')
	benchmarkFile.write(str("-" + stepName + ": " + execTime + "\n"))
	benchmarkFile.close

def stepCheckingBinary():
	for bin, path in bins.iteritems():
		if not(os.path.isfile(path)):
			print "!! Binary:", path, "Not found... ABORD!"
			exit()
	print "All binary are present"

def stepPreparePhotos():
	if (os.path.exists(pmvsDir)):
		print "Skip stepMatchFeature..."
	else:
		bundleManager.initEngine()
		bundleManager.preparePhotos()

def stepMatchFeature():
	if (os.path.exists(bundleDir)):
		print "Skip stepMatchFeature..."
	else:
		bundleManager.matchFeatures()

def stepBundleAdjustment():
	if (not os.path.exists(os.path.join(bundleDir, "bundle.out"))):
		bundleManager.doBundleAdjustment(int(options['BundlerInitPair']))
	else:
		print "Skip BundleAdjustment..."

def stepCMVS():
	if (os.path.exists(pmvsDir)):
		print "Folder:",pmvsDir,"already exist, Skip CMVS..."
	else:
		cmvsManager = osmcmvs.OsmCmvs(resultDir, binDirPath, options['CMVSNbClusters'])
		cmvsManager.doBundle2PMVS()
		cmvsManager.doCMVS(options['PMVSlevel'], options['PMVScsize'], options['PMVSthreshold'], options['PMVSwsize'], options['PMVSminImageNum'], options['PMVSCPU'])

def stepMergeCut():
	if (os.path.exists(plyMergeCut)):
		print "File:", plyMergeCut, "already exist, Skip stepMergeCut..."
	else:
		plyMerger.plyFusion(modelsDir, plyMerge)
		plyCut.plyCut(plyMerge, plyMergeCut, options['cutCoef'])

def stepGeoscale():
	if (options['noGPS'] == 1):
		print "noGPS option = 1 , Skip Geoscale..."
		plyGeoPos = plyMergeCut
		return 0
	if (os.path.exists(outGeo)):
		print "File:", outGeo, "already exist, Skip Geoscale..."
	else:
		geoscale.doGeoscale(photoDir, bundlerOut, outGeo, plyMergeCut, plyGeoPos)

def stepPoissonReconstruction():
	if (os.path.exists(plyPoisson)):
		print "File:", plyPoisson, "Skip PoissonReconstruction..."
	else:
		ply2npts.ply2npts(plyGeoPos, nptsFile)
		print "## Starting PoissonRecon"
		subprocess.call([bins["binPoissonRecon"], "--in" , nptsFile, "--out", plyPoisson, "--depth",  str(options['poissonDepth']), "--manifold"])
		os.remove(nptsFile)

def stepHDRecolor():
	if (os.path.exists(plyRecolorHD)):
		print "File:", plyRecolorHD, "Skip HDRecolor..."
	else:
		subprocess.call([bins["binSimplifier"], plyPoisson, plyPoissonClean, "--clean" ])
		subprocess.call([bins["binRecolor"], "-v" , plyGeoPos, plyPoissonClean, plyRecolorHD])

def stepSimplify():
	if (os.path.exists(daeModel)):
		print "File:", daeModel, "Skip Simplify..."
	else:
		if not(os.path.exists(kmzPath)):
			os.mkdir(kmzPath)
		if not(os.path.exists(kmzFileDir)):
			os.mkdir(kmzFileDir)
		if (options['numberOfFaces'] > 20000):
			print "WARNNING: The 3D Model for the KMZ will not be accepted by googleEarth, numberOfFaces: ", options['numberOfFaces'], "should be lower than 20000!"
		
		subprocess.call([bins["binSimplifier"], plyPoisson, plyGeoPos, kmzFileDir, str(options['numberOfFaces']), str(options['textureSize'])])
		im = Image.open(daeTexturePPM)
		im.save(daeTexturePNG)
		os.remove(daeTexturePPM)
		
def stepCreateKMZ():
	if (options['noGPS'] == 1):
		print "noGPS option = 1 , Skip CreateKMZ..."
		return 0
	if (os.path.exists(kmzArchivePath)):
		print "File:", kmzArchivePath, "Skip Simplify..."
	else:
		daeToKmz.doDaeToKmz(daeModel, daeTexturePNG, outGeo, kmzPath, redCouldDir, kmzArchivePath)

def printKiKoo(title):
	kikoo = 51
	print "#"*kikoo
	space = abs((kikoo - 4 - len(title)))
	modulo = space % 2
	space /= 2
	print str("##"+" "*(space)+title+" "*(space + modulo)+"##")
	print "#"*kikoo
	sys.stdout.flush()

###############################################################################
## START
###############################################################################
begin = time.time()

printKiKoo("RedCloud Starting :)")

photoDir, resultDir = getArgs();
if (not os.path.exists(resultDir)):
	os.mkdir(resultDir)

dataSetName = os.path.basename(resultDir)
print dataSetName
distrPath = os.path.dirname(os.path.abspath(sys.argv[0]) )
binDirPath =  os.path.join(distrPath, "bin")
redCouldDir = os.path.join(resultDir, "RedCloud")
if (not os.path.exists(redCouldDir)):
	os.mkdir(redCouldDir)

bins = {}
bins["binSift"] = "SiftGPU"
bins["binKeyMatchFull"] = "KeyMatchFull"
bins["binBundler"] = "bundler" 
bundleDir = os.path.join(resultDir, "bundle")
bundlerOut = os.path.join(bundleDir, "bundle.out")
bundlerOutTmp = os.path.join(bundleDir, "bundleTmp.out")
outGeo = os.path.join(redCouldDir, "geoData.txt")

bins["binCmvs"] = "cmvs"
bins["binGenOption"] = "genOption"
bins["binPmvs2"] = "pmvs2"
bins["binRadialUndistort"] = "RadialUndistort"
bins["binBundle2PMVS"] = "Bundle2PMVS"
bins["binBundle2Vis"] = "Bundle2Vis"
bins["binJhead"] = "jhead"
pmvsDir = os.path.join(resultDir, "pmvs")
modelsDir = os.path.join(pmvsDir, "models")
plyMerge = os.path.join(redCouldDir, "merge.ply")
plyMergeCut = os.path.join(redCouldDir, "cut.ply")
plyGeoPos = os.path.join(redCouldDir, "geoPos.ply")

nptsFile = os.path.join(redCouldDir, "cut.npts")

bins["binPoissonRecon"] = "PoissonRecon"
plyPoisson = os.path.join(redCouldDir, "poisson.ply")
plyPoissonClean = os.path.join(redCouldDir, "poissonClean.ply")

bins["binSimplifier"] = "qbms_release"
plySimplify = os.path.join(redCouldDir, "simplify.ply")

bins["binRecolor"] = "vr_release"
plyRecolorHD = os.path.join(redCouldDir, "recolorHD.ply")

kmzPath = os.path.join(redCouldDir, "kmz")
kmzFileDir = os.path.join(kmzPath, "files/")
daeModel = os.path.join(kmzFileDir, "model.dae")
daeTexturePPM = os.path.join(kmzFileDir, "texture.ppm")
daeTexturePNG = os.path.join(kmzFileDir, "texture.png")
kmzArchive = dataSetName + ".kmz"
kmzArchivePath = os.path.join(redCouldDir, kmzArchive)

for bin, path in bins.iteritems():
	binPath = getBinPath(path)
	bins[bin] = os.path.join(binDirPath, binPath)

if not(os.path.exists('option.txt')):
	print ("*-ERROR: no option.txt file")
	exit(1)

options = parseOptionFile('option.txt')

if (options['noGPS'] == 1):
	plyGeoPos = plyMergeCut

print "## Checking parameters:"
if not(os.path.exists(photoDir)):
	print ("*-ERROR: no photos directory at: ", photoDir)
	exit(1) 
print "Photos: ", photoDir
print "Results: ", resultDir
sys.stdout.flush()

steps = [
stepCheckingBinary,
stepPreparePhotos,
stepMatchFeature,
stepBundleAdjustment,
stepCMVS,
stepMergeCut,
stepGeoscale,
stepPoissonReconstruction,
stepHDRecolor,
stepSimplify,
stepCreateKMZ,
]

# initialize OsmBundler manager class
bundleManager = osmbundler.OsmBundler(photoDir, resultDir, binDirPath,  options['maxPhotoDimension'])

for step in steps:
	do(step)

end = time.time() - begin

print "See results at ", redCouldDir
printKiKoo("RedCloud - FINNISH :)")

