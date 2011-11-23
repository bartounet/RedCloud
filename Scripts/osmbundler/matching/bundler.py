import sys,os,subprocess,logging

from engine import MatchingEngine

className = "BundlerMatching"
class BundlerMatching(MatchingEngine):
    featuresListFileName = "list_features.txt"
    executable = ''
    
    def __init__(self, distrDir):
        if sys.platform == "win32":
            self.executable = os.path.join(distrDir, "KeyMatchFull.exe")
        else:
            self.executable = os.path.join(distrDir, "KeyMatchFull")
        logging.info("BundlerMatching executable path: %s" % self.executable)

    def match(self):
        logging.info("\nPerforming feature matching...")
        print self.executable
        print self.featuresListFileName
        subprocess.call([self.executable, self.featuresListFileName, self.outputFileName])
