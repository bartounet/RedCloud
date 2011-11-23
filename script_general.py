#! /usr/bin/python
#

import sys
import string
import getopt
import os
import shutil
import logging
import Scripts.osmbundler
import time
import Scripts.osmcmvs
import subprocess
from Scripts import plyMerger as plyMerger 
from Scripts import ply2npts as ply2npts

currentPath = os.getcwd()
distrPath = os.path.dirname( os.path.abspath(sys.argv[0]) )
binDirPath =  os.path.join(distrPath, "Bin")

def Usage() :
	print("Usage : ./ScriptGeneral <Dossier Photos> <Dossier de sortie>");
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
    
print "## initialize OsmBundler manager class:"
logging.basicConfig(level=logging.INFO, format="%(message)s")

# initialize OsmBundler manager class
manager = Scripts.osmbundler.OsmBundler(photoDir, resultDir, binDirPath)
print resultDir
print "## PreparePhotos:"
start = time.time()
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
manager = Scripts.osmcmvs.OsmCmvs(resultDir, binDirPath)
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
print nptsFile
print plyPoisson
subprocess.call([poissonReconExecutable, "--in" , nptsFile, "--out", plyPoisson, "--depth",  "11"])
print "--> Done in: ", time.time() - start, "secs" 
Benchmark["PoissonRecon"] = time.time() - start

#MeshSimplification

os.system(working_dir + "\Simplifier\QuadricBasedMeshSimplifier.exe " + optim_tmp + "\mesh.ply "
	+ optim_tmp + "\mesh_simplified.ply")


#VertexRecolor

os.system(working_dir + "\VertexRecolor\VertexRecolor.exe " + bundler_tmp + "\cmvs.ply "
	+ optim_tmp + "\mesh_simplified.ply " + optim_tmp + "\mesh_recolor.ply")


#TextureMaker

os.system(working_dir + "\Texturer\Texturer.exe " + bundler_tmp + "\cmvs.ply "
	+ optim_tmp + "\mesh_recolor.ply " + optim_tmp + "\mesh_textured.dae")

im = Image.open("./texture.ppm")
im.save("texture.png")	
	
#Ply2Kml
sys.argv=[working_dir + "\daeTokmz\DaeToKmz.py",
          optim_tmp + "\mesh_textured.dae", optim_tmp + "\\texture.png", geo_tmp + "\point.gps", working_dir + "\\" + output]
exec(open(working_dir + "\daeTokmz\DaeToKmz.py").read())

