#! /usr/bin/python
# A script wich Cut ply file Base on a 3D gaussian distribution

import sys
import math

# ==========================================================
# ----------------------------------------------------------
# ==========================================================

def plyCut(plyInput, plyOutput, ratio):
    
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
            for string in vertexStr:
                vertex.append(float(string))
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
        if (vertex[0] > ((standarDerivation**2) * val)): 
            #print vertex[0]
            lol = 1    
        else:
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
        vertex = vertexCut[i]
        vertex.pop(0)
        vertexStr = ' '.join([str(val) for val in vertex[:6]])
        vertexStr += ' '.join([str(int(val)) for val in vertex[5:]])
        
        if (i != nbMaxVertex):
            vertexStr += "\n"
        plyOutputFile.write(vertexStr)
         
    plyOutputFile.close()
    print "Gaussian Cut Finnish!"

# ==========================================================
# ----------------------------------------------------------
# ==========================================================
