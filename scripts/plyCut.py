#! /usr/bin/python
# A script wich Cut ply file Base on a 3D gaussian distribution

import sys
import math

# ==========================================================
# ----------------------------------------------------------
# ==========================================================

def plyCut(plyInput, plyOutput, ratio):
    print "## Starting Gaussian Cut of point cloud:"
    
    plyInputFile = open(plyInput, 'r')
    xTotal = 0
    yTotal = 0
    zTotal = 0
    nbVertex = 0
    vertexs = []
    vertexLine = False
    for line in plyInputFile:
        if (not vertexLine):
            if (line == 'end_header\n' or line == 'end_header\r\n'):
		        vertexLine = True
        else:
            #print line
            vertexStr = line.split(' ')
            vertex = []
            vertex.append(float(vertexStr[0]))
            vertex.append(float(vertexStr[1]))
            vertex.append(float(vertexStr[2]))
            vertex.append(line)
            
            xTotal += vertex[0]
            yTotal += vertex[1]
            zTotal += vertex[2]
            nbVertex += 1   
            vertexs.append(vertex)     
        
    plyInputFile.close()
    
    xMoy = xTotal / nbVertex
    yMoy = yTotal / nbVertex
    zMoy = zTotal / nbVertex
    print "xMoy:", xMoy
    print "yMoy:", yMoy
    print "zMoy:", zMoy
    
    sortedVertexs = []
    
    dists = 0
    ##QuickSort??
    for vertex in vertexs:
        dist = ((vertex[0] - xMoy)**2 + (vertex[1] - yMoy)**2 + (vertex[2] - zMoy)**2)
        dists += dist
        vertex.insert(0, dist)
     
    standarDerivation = math.sqrt(dists / len(vertexs))
    print "StandarDerivation:", standarDerivation
        
    val = (ratio/0.341)
    vertexCut = []
    for vertex in vertexs:
        if (vertex[0] < ((standarDerivation**2) * val)):
            vertexCut.append(vertex)
          
    print "Nb inital Vertexs:",len(vertexs)
    print "Nb Vertexs left:",len(vertexCut)
    print "ratio =", float(len(vertexCut)) / float(len(vertexs))
    nbMaxVertex = int(nbVertex * ratio)
    nbMaxVertex = len(vertexCut)
    plyOutputFile = open(plyOutput, 'w')

    plyOutputFile.write('''ply
format ascii 1.0
element vertex '''+str(nbMaxVertex)+'''
property float x
property float y
property float z
property float nx
property float ny
property float nz
property uchar diffuse_red
property uchar diffuse_green
property uchar diffuse_blue
end_header\n''')
          
    for i in range(0, nbMaxVertex):
        plyOutputFile.write(vertexCut[i][4])
         
    plyOutputFile.close()
    print "Gaussian Cut Finnish!"



# ==========================================================
# ----------------------------------------------------------
# ==========================================================
