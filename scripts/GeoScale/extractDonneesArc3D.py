#! /usr/bin/python

# EXTRACTION DES DONNEES ARC3D
import os
import re
import sys
import numpy as np

#function [R,t,N,List] = extractDonneesArc3D(dircam)
def extractDonneesArc3d(dircam):


#Creation d'une structure List contenant la liste des fichiers camera
#contenus dans dir
#List = dir(fullfile(dircam, 'camera.*'));

    camList = [f for f in os.listdir(dircam) if (f.find("camera") != -1)]
    camList.sort()
#Nombre de fichiers camera
#N = length(List);
    N = len(camList)

#Initialisation d'une matrice rotation R (3x3xN) et d'un vecteur
#translation t (Nx3)
#R = zeros([3 3 N]);
#t = zeros([N 3]);

    R = np.zeros( (3, 3, N) )
    t = np.zeros( (N, 3) )

#Lecture des N fichiers camera
    for i in range(N):
        f = open(dircam + "/" + camList[i])
        lineNumber = 0
        for l in f:
            if lineNumber == 6:
                R[:, 0, i] = map(float, l.split())
            if lineNumber == 7:
                R[:, 1, i] = map(float, l.split())
            if lineNumber == 8:
                R[:, 2, i] = map(float, l.split())
            if lineNumber == 10:
                t[i, :] = map(float, l.split())
            lineNumber += 1

    return [R, t, N]

'''for i=1:N
    camera = fullfile(dircam, List(i).name);
    fid = fopen(camera,'r');
    A = fscanf(fid,'%e');
    R(:,:,i) = [A(13) A(14) A(15); A(16) A(17) A(18); A(19) A(20) A(21)];
    t(i,:) = [A(22) A(23) A(24)];
    fclose(fid);
end
end'''

#extractDonneesArc3d(sys.argv[1])

