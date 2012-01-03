import numpy as np
from PIL import Image
from PIL.ExifTags import TAGS, GPSTAGS
import os

def extractBundlerData(fileName):
	f = open(fileName, "r")
	
	f.readline()
	[nbCam, nbPoint] = map(float, f.readline().split())
	nbCam = (int)(nbCam)
	nbPoint = (int)(nbPoint)
	
	camsF = np.zeros( (nbCam, 3) )
	camsR = np.zeros( (3, 3, nbCam) )
	camsT = np.zeros( (nbCam, 3) )
	camsP = np.zeros( (nbCam, 3) )
	CamsNbPoint = np.zeros(nbCam)
	
	# read cams infos
	for i in range(nbCam):
		f.readline()
		camsR[0, :, i] = map(float, f.readline().split())
		camsR[1, :, i] = map(float, f.readline().split())
		camsR[2, :, i] = map(float, f.readline().split())
		camsT[i, :] = map(float, f.readline().split())
		camsP[i, :] = np.transpose(- np.dot(np.transpose(camsR[:, :, i]), np.transpose(camsT[i, :])))
		
	for i in range(nbPoint):
		f.readline()
		f.readline()	
		camsList = map(float, f.readline().split())
		for cam in range((int)(camsList[0])):
			CamsNbPoint[(int)(camsList[cam * 4 + 1])] += 1

	goodCams = []
	for i in range(nbCam):
		if CamsNbPoint[i] > 20:
			goodCams.append(i)

	goodCamsP = np.zeros( (len(goodCams), 3) )
	for i in range(len(goodCams)):
		goodCamsP[i, :] = camsP[goodCams[i], :]
		
	return goodCamsP, goodCams
	


def get_exif_data(image):
	"""Returns a dictionary from the exif data of an PIL Image item. Also converts the GPS Tags"""
	exif_data = {}
	info = image._getexif()
	if info:
		for tag, value in info.items():
			decoded = TAGS.get(tag, tag)
			if decoded == "GPSInfo":
				gps_data = {}
				for t in value:
					sub_decoded = GPSTAGS.get(t, t)
					gps_data[sub_decoded] = value[t]
					
				exif_data[decoded] = gps_data
			else:
				exif_data[decoded] = value
				
	return exif_data

def _get_if_exist(data, key):
	if key in data:
		return data[key]
	return None

	
def _convert_to_degress(value):
	"""Helper function to convert the GPS coordinates stored in the EXIF to degress in float format"""
	d0 = value[0][0]
	d1 = value[0][1]
	d = float(d0) / float(d1)
	
	m0 = value[1][0]
	m1 = value[1][1]
	m = float(m0) / float(m1)
	
	s0 = value[2][0]
	s1 = value[2][1]
	s = float(s0) / float(s1)
	
	return d + (m / 60.0) + (s / 3600.0)
	

def get_lon_lat_alt(exif_data):
	"""Returns the latitude and longitude, if available, from the provided exif_data (obtained through get_exif_data above)"""
	lon = None
	lat = None
	alt = None
	
	if "GPSInfo" in exif_data:
		gps_info = exif_data["GPSInfo"]
		
		gps_longitude = _get_if_exist(gps_info, 'GPSLongitude')
		gps_longitude_ref = _get_if_exist(gps_info, 'GPSLongitudeRef')
		gps_latitude = _get_if_exist(gps_info, "GPSLatitude")
		gps_latitude_ref = _get_if_exist(gps_info, 'GPSLatitudeRef')
		gps_altitude = _get_if_exist(gps_info, 'GPSAltitude')
		
		if gps_latitude and gps_latitude_ref and gps_longitude and gps_longitude_ref:
			lon = _convert_to_degress(gps_longitude)
			if gps_longitude_ref != "E":
				lon = 0 - lon
			lat = _convert_to_degress(gps_latitude)
			if gps_latitude_ref != "N":
				lat = 0 - lat
				
		if gps_altitude:
			alt = gps_altitude[0]
			
	return lon,lat, alt

def extractPhotosData(dirgps, goodCams):
	gpsList = [f for f in os.listdir(dirgps) if (f.lower().find(".jpg") != -1)]
	gpsList.sort()

	GPS = np.zeros( (len(goodCams), 3) )
	for i in range(len(goodCams)):
		img = Image.open(dirgps + "/" + gpsList[goodCams[i]])
		exif_data = get_exif_data(img)
		GPS[i, :] = get_lon_lat_alt(exif_data)
		
	return GPS
