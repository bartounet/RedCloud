import os, subprocess, gzip

from sift import Sift

className = "SiftGPU"
class SiftGPU(Sift):
    
    win32Executable = "SiftGPU.exe"
    linuxExecutable = "SiftGPU"
    
    def __init__(self, distrDir):
        Sift.__init__(self, distrDir)

    def extract(self, photo, photoInfo):
        photoFile = open("%s.jpg.pgm" % photo, "rb")
        siftTextFile = open("%s.key" % photo, "w")
        #print "%s.jpg.pgm" % photo
        #print os.getcwd()
        #print self.executable
        subprocess.call([self.executable, "-i" , "%s.jpg.pgm" % photo, "-o", "%s.key" % photo, "-nomc", "-v", "0"])
        photoFile.close()
        siftTextFile.close()
        # gzip SIFT file and remove it
        siftTextFile = open("%s.key" % photo, "r")
        print photo, ":", siftTextFile.readline().split(' ',1)[0], "SIFT points found"
        siftGzipFile = gzip.open("%s.key.gz" % photo, "wb")
        siftGzipFile.writelines(siftTextFile)
        siftGzipFile.close()
        siftTextFile.close()
        #os.remove("%s.key" % photo)
