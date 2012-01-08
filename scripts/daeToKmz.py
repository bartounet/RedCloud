import sys
import os
import zipfile

#TODO: define arguments


def zipfolder(path, relname, archive):
    paths = os.listdir(path)
    for p in paths:
        p1 = os.path.join(path, p) 
        p2 = os.path.join(relname, p)
        if os.path.isdir(p1): 
            zipfolder(p1, p2, archive)
        else:
            archive.write(p1, p2) 

def create_zip(path, relname, archname):
    archive = zipfile.ZipFile(archname, "w", zipfile.ZIP_DEFLATED)
    if os.path.isdir(path):
        zipfolder(path, relname, archive)
    else:
        archive.write(path, relname)
    archive.close()
    
def doDaeToKmz(daeModel, daeTexture, geofile, kmzPath, resDir, kmzArchivePath):
    if (not os.path.exists(kmzPath)):
        os.mkdir(kmzPath)

    kmzFileDir = os.path.join(kmzPath, "files")
    if (not os.path.exists(kmzFileDir)):
        os.mkdir(kmzFileDir)

    kmzDaeModel = os.path.join(kmzFileDir, "model.dae")
    #shutil.copy(daeModel, kmzFileDir)
    fIn = open(geofile, 'r')
    coord = fIn.readline().split()
    scale = 1
    longitude = coord[1]
    latitude = coord[0]
    altitude = 0
    
    relativeModelPath = os.path.join("files", "model.dae")
    relativeTexturePath = os.path.join("files", "texture.png")
    
    
    kmlPath = os.path.join(kmzPath, "doc.kml")
    f = open(kmlPath, 'w')
    
    f.write('''<?xml version="1.0" encoding="UTF-8"?>
    <kml xmlns="http://www.opengis.net/kml/2.2"
     xmlns:gx="http://www.google.com/kml/ext/2.2">   <!-- required when using gx-prefixed elements -->
    <Placemark>
      <name>RedClouds</name>
      <description>3D reconstruction With: 
       RedClouds project</description>
    <Model id="Model"> 
      <altitudeMode>absolute</altitudeMode> 
      <Location> 
        <longitude>''' + str(longitude) + '''</longitude> 
        <latitude>''' + str(latitude) + '''</latitude> 
        <altitude>''' + str(altitude) + '''</altitude> 
      </Location> 
      <Orientation> 
        <tilt>0.0</tilt> 
        <roll>0.0</roll>
        <heading>-90.0</heading>
      </Orientation> 
      <Scale> 
        <x>''' + str(scale) + '''</x> 
        <y>''' + str(scale) + '''</y> 
        <z>''' + str(scale) + '''</z> 
      </Scale> 
      <Link> 
        <href>''' + relativeModelPath + '''</href> 
        <refreshMode>once</refreshMode> 
      </Link>
     <ResourceMap>
        <Alias>
          <targetHref>''' + relativeTexturePath + '''</targetHref>
          <sourceHref>''' + relativeTexturePath + '''</sourceHref>
        </Alias>
      </ResourceMap> 
     </Model>
    </Placemark>
    </kml>
    ''')

    f.close()
    create_zip(kmzPath, "", kmzArchivePath)
    

