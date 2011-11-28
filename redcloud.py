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

from scripts import osmcmvs as osmcmvs 
from scripts import osmbundler as osmbundler 
from scripts import plyMerger as plyMerger 
from scripts import ply2npts as ply2npts

currentPath = os.getcwd()
distrPath = os.path.dirname( os.path.abspath(sys.argv[0]) )
binDirPath =  os.path.join(distrPath, "bin")

def Usage() :
	print("Usage : ./redcloud.py <Dossier Photos> <Dossier de sortie>");
	sys.exit(1);

def manage_options() :	
    if (len(sys.argv) != 3) :
	    Usage();	
    photoDir = sys.argv[1]
    if not(os.path.isabs(photoDir)):
        photoDir = os.path.join(currentPath, photoDir)
    resultDir = sys.argv[2];
    if not(os.path.isabs(resultDir)):
        resultDir = os.path.join(currentPath, resultDir)

    return (photoDir, resultDir);


print "###############################"
print "##       RedClouds :)        ##"
print "###############################"

Benchmark = {}

#Options + Dossier temporaires
working_dir = os.path.dirname(os.path.realpath(sys.argv[0]));
photoDir, resultDir = manage_options();

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

print "## PreparePhotos:"
start = time.time()
#logging.basicConfig(level=logging.INFO, format="%(message)s")
manager = osmbundler.OsmBundler(photoDir, resultDir, binDirPath)
manager.preparePhotos()
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["PreparePhotos"] = time.time() - start

print "## MatchFeatures:"
start = time.time()
manager.matchFeatures()
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["MatchFeatures"] = time.time() - start

print "## DoBundleAdjustment:"
start = time.time()
manager.doBundleAdjustment()
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["BundleAdjustment"] = time.time() - start

print "## doCMVS:"
start = time.time()
manager = osmcmvs.OsmCmvs(resultDir, binDirPath)
manager.doBundle2PMVS()
manager.doCMVS()
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["CMVS"] = time.time() - start

print "## plyMerge:"
start = time.time()
redCouldDir = os.path.join(resultDir, "RedClouds") 
if (not os.path.exists(redCouldDir)):
    os.mkdir(redCouldDir)
print resultDir
modelsDir = os.path.join(resultDir, "pmvs", "models")
plyMerge = os.path.join(redCouldDir, "merge.ply")
plyMerger.plyFusion(modelsDir, plyMerge)
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["plyMerge"] = time.time() - start

print "## ply2npts:"
start = time.time()
nptsFile = os.path.join(redCouldDir, "merge.npts")
ply2npts.ply2npts(plyMerge, nptsFile)
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["ply2npts"] = time.time() - start

print "## PoissonRecon:"
start = time.time()
poissonReconExecutable = os.path.join(binDirPath, "PoissonRecon")
plyPoisson = os.path.join(redCouldDir, "poisson.ply")
subprocess.call([poissonReconExecutable, "--in" , nptsFile, "--out", plyPoisson, "--depth",  "10", "--manifold"])
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["PoissonRecon"] = time.time() - start

print "## Cleaner:"
start = time.time()
cleanerExecutable = os.path.join(binDirPath, "Cleaner")
plyClean = os.path.join(redCouldDir, "clean.ply")
subprocess.call([cleanerExecutable, "-v" , plyPoisson, plyClean])
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["Cleaner"] = time.time() - start

print "## Simplifier:"
start = time.time()
simplifierExecutable = os.path.join(binDirPath, "qbms_final")
plySimplify = os.path.join(redCouldDir, "clean.ply")
subprocess.call([simplifierExecutable, plyClean, plySimplify])
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["Simplifier"] = time.time() - start

print "## Recolor:"
start = time.time()
recolorExecutable = os.path.join(binDirPath, "vr_final")
plySimplyRecolor = os.path.join(redCouldDir, "plySimplyRecolor.ply")
subprocess.call([recolorExecutable, "-v" , plyMerge, plySimplify, plySimplyRecolor])
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["Recolor"] = time.time() - start

print "## Texturer:"
start = time.time()
texturerExecutable = os.path.join(binDirPath, "Texturer")
daeModel = os.path.join(redCouldDir, "model.dae")
subprocess.call([texturerExecutable, plyMerge, plySimplyRecolor, daeModel])
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["Texturer"] = time.time() - start

im = Image.open("./texture.ppm")
im.save("texture.png")	

print Benchmark	
#Ply2Kml
sys.argv=[working_dir + "\daeTokmz\DaeToKmz.py",
          optim_tmp + "\mesh_textured.dae", optim_tmp + "\\texture.png", geo_tmp + "\point.gps", working_dir + "\\" + output]
exec(open(working_dir + "\daeTokmz\DaeToKmz.py").read())

