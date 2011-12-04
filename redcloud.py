#! /usr/bin/python
#

import sys
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

from scripts import osmcmvs as osmcmvs 
from scripts import osmbundler as osmbundler 
from scripts import plyMerger as plyMerger 
from scripts import ply2npts as ply2npts
from scripts import daeToKmz as daeToKmz 


def Usage() :
	print("Usage : ./redcloud.py <Dossier Photos> <Dossier de sortie>");
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


def do(step):
    stepName = step.__name__
    print "#############################"
    print "## ", stepName
    print "#############################"
    start = time.time()
    step()
    execTimeMin = (int)((time.time() - start) / 60.)
    execTimeSec = (int)((time.time() - start) % 60)
    execTime = str(str(execTimeMin) + " mins and " + str(execTimeSec) + " secs")
    print "--> Done in: ", execTime, "\n\n"
    benchmarkFile = open(os.path.join(redCouldDir, "Benchmark.txt"), 'a+')
    benchmarkFile.write(str("-" + stepName + ": " + execTime + "\n"))
    benchmarkFile.close


def stepPreparePhotos():
    bundleManager.initEngine()
    bundleManager.preparePhotos()

def stepMatchFeature():
    bundleManager.matchFeatures()

def stepBundleAdjustment():
    if (not os.path.exists(os.path.join(resultDir, "bundle", "bundle.out"))):
        bundleManager.doBundleAdjustment()
    else:
        print "Skip BundleAdjustment"

def stepGeoscale():
    shutil.copyfile(bundlerOut, bundlerOutTmp)
    outGeo =  os.path.join(redCouldDir, "geoData.txt")
    geoscale.doGeoscale(photoDir, bundlerOut, bundlerOut, outGeo)

def stepCMVS():
    if (not os.path.exists(os.path.join(resultDir, "pmvs"))):
        cmvsManager = osmcmvs.OsmCmvs(resultDir, binDirPath, CMVSNbClusters)
        cmvsManager.doBundle2PMVS()
        cmvsManager.doCMVS(CMVSLevel)
    else:
        print "Folder pmvs already exist, Skip CMVS..."

def stepPoissonReconstruction():
    plyMerger.plyFusion(modelsDir, plyMerge)
    ply2npts.ply2npts(plyMerge, nptsFile)
    subprocess.call([poissonReconExecutable, "--in" , nptsFile, "--out", plyPoisson, "--depth",  "10", "--manifold"])

def stepSimplify():
    subprocess.call([simplifierExecutable, plyPoisson, plySimplify])
    subprocess.call([recolorExecutable, "-v" , plyMerge, plySimplify, plySimplyRecolor])

def stepCreateKMZ():
    subprocess.call([texturerExecutable, plyMerge, plySimplyRecolor, daeModel])    
    im = Image.open("./texture.ppm")
    im.save("texture.png")	
    daeToKmz.daeToKmz(daeModel, daeTexture, geofile, kmlPath)


begin = time.time()
print "###############################"
print "##       RedClouds :)        ##"
print "###############################"

photoDir, resultDir = getArgs();
if (not os.path.exists(resultDir)):
    os.mkdir(resultDir)

distrPath = os.path.dirname(os.path.abspath(sys.argv[0]) )
binDirPath =  os.path.join(distrPath, "bin")
redCouldDir = os.path.join(resultDir, "RedClouds")
if (not os.path.exists(redCouldDir)):
    os.mkdir(redCouldDir)


bundlerOut = os.path.join(resultDir, "bundle", "bundle.out")
bundlerOutTmp = os.path.join(resultDir, "bundle", "bundleTmp.out")
outGeo =  os.path.join(redCouldDir, "geoData.txt")

modelsDir = os.path.join(resultDir, "pmvs", "models")
plyMerge = os.path.join(redCouldDir, "merge.ply")
nptsFile = os.path.join(redCouldDir, "merge.npts")

poissonReconExecutable = os.path.join(binDirPath, "PoissonRecon")
plyPoisson = os.path.join(redCouldDir, "poisson.ply")

simplifierExecutable = os.path.join(binDirPath, "qbms_release")
plySimplify = os.path.join(redCouldDir, "simplify.ply")

recolorExecutable = os.path.join(binDirPath, "vr_release")
plySimplyRecolor = os.path.join(redCouldDir, "plySimplyRecolor.ply")

texturerExecutable = os.path.join(binDirPath, "Texturer")
daeModel = os.path.join(redCouldDir, "model.dae")

Benchmark = {}

### OPTION:
maxPhotoDimension = 2000
maxSiftPoints = 2000
CMVSLevel = 0
CMVSNbClusters = 20

print "## Checking parameters:"
if not(os.path.exists(photoDir)):
    print ("*-ERROR: no photos directory at: ", photoDir)
    exit(1) 
print "--Photos directory: ", photoDir

if (not os.path.exists(resultDir)):
    os.mkdir(resultDir)
    print "--Result directory (created): ", resultDir
else:
    print "--Result directory (already exist): ", resultDir
    
# initialize OsmBundler manager class
print resultDir

bundleManager = osmbundler.OsmBundler(photoDir, resultDir, binDirPath, maxPhotoDimension)

steps =[
stepPreparePhotos,
stepMatchFeature,
stepBundleAdjustment,
stepGeoscale,
stepCMVS,
stepPoissonReconstruction,
stepSimplify,
#stepCreateKMZ,
]

for step in steps:
    do(step)

end = time.time() - begin

print Benchmark	

print "###############################"
print "## RedClouds - FINNISH :)    ##"
print "###############################"
