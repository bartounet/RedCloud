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
import Scripts.plyMerger

currentPath = os.getcwd()
distrPath = os.path.dirname( os.path.abspath(sys.argv[0]) )
binDirPath =  os.path.join(distrPath, "Bin")

if sys.platform == "win32": 
    binDirPath =  os.path.join(binDirPath, "Windows")
else:
    binDirPath =  os.path.join(binDirPath, "Linux")

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

Benchmark = []

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

print "## MatchFeatures:"
start = time.clock()
manager.matchFeatures()
print "--> Done in: ", time.time() - start, "secs" 

print "## DoBundleAdjustment:"
start = time.time()
manager.doBundleAdjustment()
print "--> Done in: ", time.time() - start, "secs" 

print "## doCMVS:"
start = time.time()
manager = Scripts.osmcmvs.OsmCmvs(resultDir, binDirPath)
manager.doBundle2PMVS()
manager.doCMVS()
print "--> Done in: ", time.time() - start, "secs" 

print "LOL"

print "## CMVS Fusion:"
start = time.time()
redCouldDir = os.path.join(resultDir, "RedClouds") 
if (not os.path.exists(redCouldDir)):
    os.mkdir(redCouldDir)
print resultDir
modelsDir = os.path.join(resultDir, "pmvs", "models")
Scripts.plyMerger.plyFusion(modelsDir, os.path.join(redCouldDir, "fusion.ply"))
print "--> Done in: ", time.time() - start, "secs" 

#PoissonRecon
'''
sys.argv=[working_dir + "\PoissonRecon\ply2npts.py",
          bundler_tmp + "\cmvs.ply"]
exec(open(working_dir + "\PoissonRecon\ply2npts.py").read())

os.system(working_dir + "\PoissonRecon\PoissonRecon.exe --in " + bundler_tmp + "\cmvs.npts " +
	"--out " + optim_tmp + "\mesh.ply" + " --depth 11")
'''

#cleaning
#FIXME

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

