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
        bundleManager.doBundleAdjustment(BundlerinitPair)
    else:
        print "Skip BundleAdjustment..."

def stepGeoscale():
    if (os.path.exists(outGeo)):
        print "File:", outGeo, "already exist, Skip Geoscale..."
    else:
        shutil.copyfile(bundlerOut, bundlerOutTmp)
        geoscale.doGeoscale(photoDir, bundlerOut, bundlerOut, outGeo)

def stepCMVS():
    if (os.path.exists(pmvsDir)):
        print "Folder:",pmvsDir,"already exist, Skip CMVS..."
    else:
        cmvsManager = osmcmvs.OsmCmvs(resultDir, binDirPath, CMVSNbClusters)
        cmvsManager.doBundle2PMVS()
        cmvsManager.doCMVS(PMVSlevel, PMVScsize, PMVSthreshold, PMVSwsize, PMVSminImageNum, PMVSCPU)

def stepPoissonReconstruction():
    if (os.path.exists(plyPoisson)):
        print "File:", plyPoisson, "Skip PoissonReconstruction..."
    else:
        plyMerger.plyFusion(modelsDir, plyMerge)
        plyCut.plyCut(plyMerge, plyMergeCut, cutCoef)
        ply2npts.ply2npts(plyMergeCut, nptsFile)
        print "## Starting PoissonRecon"
        subprocess.call([poissonReconExecutable, "--in" , nptsFile, "--out", plyPoisson, "--depth",  str(poissonDepth), "--manifold"])
        os.remove(nptsFile)

def stepSimplify():
    if (os.path.exists(plySimplify)):
        print "File:", plySimplify, "Skip Simplify..."
    else:
        subprocess.call([simplifierExecutable, plyPoisson, plySimplify, str(numberOfFaces)])
    if (os.path.exists(plySimplyRecolor)):
        print "File:", plySimplyRecolor, "Skip Recolor..."
    else:
        subprocess.call([recolorExecutable, "-v" , plyMergeCut, plySimplify, plySimplyRecolor])
    if not(os.path.exists(kmzPath)):
        os.mkdir(kmzPath)
    if not(os.path.exists(kmzFileDir)):
        os.mkdir(kmzFileDir)
    if (os.path.exists(daeModel)):
        print "File:", daeModel, "Skip Texturer..."
    else:
        subprocess.call([texturerExecutable, plyMergeCut, plySimplyRecolor, kmzFileDir])
        im = Image.open(daeTexturePPM)
        im.save(daeTexturePNG)
        os.remove(daeTexturePPM)

def stepCreateKMZ():
    if (numberOfFaces > 20000):
        print "WARNNING: KMZ will not be create, numberOfFaces: ", numberOfFaces, "should be lower than 20000!"
    else:
        daeToKmz.doDaeToKmz(daeModel, daeTexturePNG, outGeo, kmzPath, redCouldDir, dataSetName)

def printKiKoo(title):
    kikoo = 51
    print "#"*kikoo
    space = abs((kikoo - 4 - len(title)))
    modulo = space % 2
    space /= 2
    print str("##"+" "*(space)+title+" "*(space + modulo)+"##")
    print "#"*kikoo

###############################################################################
## START
###############################################################################
begin = time.time()

printKiKoo("RedClouds Starting :)")

photoDir, resultDir = getArgs();
if (not os.path.exists(resultDir)):
    os.mkdir(resultDir)

dataSetName = os.path.basename(resultDir)
print dataSetName
distrPath = os.path.dirname(os.path.abspath(sys.argv[0]) )
binDirPath =  os.path.join(distrPath, "bin")
redCouldDir = os.path.join(resultDir, "RedClouds")
if (not os.path.exists(redCouldDir)):
    os.mkdir(redCouldDir)

bundleDir = os.path.join(resultDir, "bundle")
bundlerOut = os.path.join(bundleDir, "bundle.out")
bundlerOutTmp = os.path.join(bundleDir, "bundleTmp.out")
outGeo = os.path.join(redCouldDir, "geoData.txt")

pmvsDir = os.path.join(resultDir, "pmvs")
modelsDir = os.path.join(pmvsDir, "models")
plyMerge = os.path.join(redCouldDir, "merge.ply")
plyMergeCut = os.path.join(redCouldDir, "cut.ply")
nptsFile = os.path.join(redCouldDir, "cut.npts")

poissonReconExecutable = os.path.join(binDirPath, "PoissonRecon")
plyPoisson = os.path.join(redCouldDir, "poisson.ply")

simplifierExecutable = os.path.join(binDirPath, "qbms_release")
plySimplify = os.path.join(redCouldDir, "simplify.ply")

recolorExecutable = os.path.join(binDirPath, "vr_release")
plySimplyRecolor = os.path.join(redCouldDir, "plySimplyRecolor.ply")

texturerExecutable = os.path.join(binDirPath, "texturer_release")
kmzPath = os.path.join(redCouldDir, "kmz")
kmzFileDir = os.path.join(kmzPath, "files")
daeModel = os.path.join(kmzFileDir, "model.dae")
daeTexturePPM = os.path.join(kmzFileDir, "texture.ppm")
daeTexturePNG = os.path.join(kmzFileDir, "texture.png")


### OPTION:
maxPhotoDimension = 20000
maxSiftPoints = 2000

BundlerInitPair = 10

CMVSNbClusters = 15

PMVSlevel = 1
PMVScsize = 2
PMVSthreshold = 0.7
PMVSwsize = 7
PMVSminImageNum = 3
PMVSCPU = 8

cutCoef = 0.5
poissonDepth = 10
numberOfFaces = 20000



print "## Checking parameters:"
if not(os.path.exists(photoDir)):
    print ("*-ERROR: no photos directory at: ", photoDir)
    exit(1) 
print "--Photos directory: ", photoDir

# initialize OsmBundler manager class
print resultDir

bundleManager = osmbundler.OsmBundler(photoDir, resultDir, binDirPath, maxPhotoDimension)

steps = [
stepPreparePhotos,
stepMatchFeature,
stepBundleAdjustment,
stepGeoscale,
stepCMVS,
stepPoissonReconstruction,
stepSimplify,
stepCreateKMZ,
]

for step in steps:
    do(step)

end = time.time() - begin

print "See results at ", redCouldDir
printKiKoo("RedClouds - FINNISH :)")

