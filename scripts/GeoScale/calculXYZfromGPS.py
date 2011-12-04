import numpy as np
import math

"""calculXYZfromGPS retourne les coordonnees dans le referentiel geocentrique
(T,XT,YT,ZT) d'un point defini par sa longitude long (en degre), sa latitude
lat (en degre) et sa hauteur h (en metre par rapport au niveau de la mer),
et le rayon de la Terre a cette latitude

La Terre etant approximee a un ellipsoide de rayon equatorial 6 378,137 km
et de rayon polaire 6 356,7523142 km, le rayon de la Terre depend de la latitude"""

def calculXYZfromGPS(longi, lat, h) :
	rlong = math.radians(longi)
	rlat = math.radians(lat)
	re = 6378137
	rp = 6356752.3142
	Rterre = (rp**2 / (1 + (math.cos(rlat))**2 * (rp**2 / re**2 - 1)))**0.5;
	M_T = [(Rterre+h) * math.cos(rlat)*math.cos(rlong),
			(Rterre+h) * math.cos(rlat)*math.sin(rlong),
			(Rterre+h) * math.sin(rlat)];
	return [M_T, Rterre]

