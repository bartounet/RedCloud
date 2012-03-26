#     This file is part of Geoscale tool (see http://www.geoscale.org/).
# 
#     Copyright (C) 2011 O.R.A., Jonathan Cloux (head developer), Benoît Millot
#     Original idea from O.R.A. by Adrien Normier
#     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
#                jonathanclx@gmail.com   
# 
#     Geoscale is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     Geoscale is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
# 
#     You should have received a copy of the GNU General Public License
#     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

#! /bin/python

import numpy as np
import math
import scriptMoindresCarres as mc
import scriptRepositionnement as re
import ExtractData as ed
import Tools as tools

def doGeoscale(PhotoFolder, bundlerOut, outGeo, plyIn, plyOut):
	bundlerCams, goodCams = ed.extractBundlerData(bundlerOut)
	photosCams_gps = ed.extractPhotosData(PhotoFolder, goodCams)

	photosCams_local, center_earth = tools.localFromGPS(photosCams_gps)

	scalling = tools.computeScaling(bundlerCams, photosCams_local)

	bundlerCams_scaled = bundlerCams * scalling;
	bundlerCams_rp = re.repositionnement(photosCams_local, bundlerCams_scaled)
	rotation = mc.moindresCarres(photosCams_local, bundlerCams_rp)

	translation = tools.computeTranslation(bundlerCams_scaled, photosCams_local, rotation)

	tools.applyTransformation(plyIn, plyOut, rotation, translation, scalling)
	tools.writeGeoData(center_earth, outGeo)
