import sys

#TODO: define arguments

def doDaeToKmz(daeModel, daeTexture, geofile, kmlPath):
    fIn = open(geofile, 'r')
    coord = fIn.readline().split()
    scale = 1
    longitude = coord[1]
    latitude = coord[0]
    altitude = 0

    f = open(kmlPath, 'w')

    f.write('''<?xml version="1.0" encoding="UTF-8"?>
    <kml xmlns="http://www.opengis.net/kml/2.2"
     xmlns:gx="http://www.google.com/kml/ext/2.2">   <!-- required when using gx-prefixed elements -->
    <Placemark>
      <name>view point</name>
    <Model id="Model"> 
      <altitudeMode>absolute</altitudeMode> 
      <Location> 
        <longitude>''' + str(longitude) + '''</longitude> 
        <latitude>''' + str(latitude) + '''</latitude> 
        <altitude>''' + str(altitude) + '''</altitude> 
      </Location> 
      <Orientation> 
        <heading>-90.0</heading> <!-- -->
        <tilt>0.0</tilt> 
        <roll>0.0</roll> 
      </Orientation> 
      <Scale> 
        <x>''' + str(scale) + '''</x> 
        <y>''' + str(scale) + '''</y> 
        <z>''' + str(scale) + '''</z> 
      </Scale> 
      <Link> 
        <href>''' + daeModel + '''</href> 
        <refreshMode>once</refreshMode> 
      </Link>
     <ResourceMap>
        <Alias>
          <targetHref>''' + daeTexture + '''</targetHref>
          <sourceHref>''' + daeTexture + '''</sourceHref>
        </Alias>
      </ResourceMap> 
     </Model>
    </Placemark>
    </kml>
    ''')

    f.close()
