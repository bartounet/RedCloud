#! /usr/bin/python

import numpy as np
from PIL import Image
from PIL.ExifTags import TAGS, GPSTAGS
import sys
import os

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

# EXTRACTION DES DONNEES GPS DES IMAGES 'CAMERA'


#function [GPS, Ngps] = extractDonneesGPS(dirgps)
def extractDonneesGPS(dirgps, goodCams):

#Creation d'une structure List contenant la liste des fichiers jpg du
#dossier dirgps
#List_gps = dir(fullfile(dirgps,'*.jpg'));
	gpsList = [f for f in os.listdir(dirgps) if (f.lower().find(".jpg") != -1)]
	
	gpsList.sort()
	
	print gpsList

#Nombre d'images 'camera'
#Ngps = length(List_gps);
	Ngps = len(gpsList)

#Initialisation d'une matrice GPS contenant la longitude (en degre), la
#latitude (en degre) et la hauteur (en metre) de chaque image 'camera'
#GPS = [];
	GPS = np.zeros( (len(goodCams), 3) )

#Lecture des Ngps images 'camera' avec la fonction imfinfo()
	for i in range(len(goodCams)):
		img = Image.open(dirgps + "/" + gpsList[goodCams[i]])
		exif_data = get_exif_data(img)
		GPS[i, :] = get_lon_lat_alt(exif_data)
		
	return GPS, len(goodCams)

#print extractDonneesGPS(sys.argv[1])
