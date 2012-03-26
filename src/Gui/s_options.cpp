//     This file is part of RedCloud tool.
// 
//     Copyright (C) 2012 Munzer Thibaut, Jonathan Aigrain, Adrien Chey, Nicolas Fabretti
//     Original idea from O.R.A. by Adrien Normier
//     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
//                redcloud.contact@gmail.com   
// 
//     Redcloud is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     Geoscale is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

#include "s_options.h"

s_options s_options::getOptionsLow()
{
	s_options res;

	res.MaxPhotoDimension = 2000;
	res.BundlerInitPair = 2;
	res.CMVSNbClusters = 15;
	res.PMVSlevel = 1;
	res.PMVScsize = 2;
	res.PMVSthreshold= 0.7;
	res.PMVSwsize = 7;
	res.PMVSminImageNum = 3;
	res.PMVSCPU = 4;
	res.CutCoef = 0.5;
	res.PoissonDepth = 8;
	res.NumberOfFaces = 20000;
	res.TextureSize = 2048;

	return res;
}

s_options s_options::getOptionsMedium()
{
	s_options res;

	res.MaxPhotoDimension = 4000;
	res.BundlerInitPair = 2;
	res.CMVSNbClusters = 15;
	res.PMVSlevel = 1;
	res.PMVScsize = 2;
	res.PMVSthreshold= 0.7;
	res.PMVSwsize = 7;
	res.PMVSminImageNum = 3;
	res.PMVSCPU = 4;
	res.CutCoef = 0.5;
	res.PoissonDepth = 11;
	res.NumberOfFaces = 20000;
	res.TextureSize = 2048;

	return res;
}

s_options s_options::getOptionsHigh()
{
	s_options res;

	res.MaxPhotoDimension = 20000;
	res.BundlerInitPair = 2;
	res.CMVSNbClusters = 15;
	res.PMVSlevel = 1;
	res.PMVScsize = 2;
	res.PMVSthreshold= 0.7;
	res.PMVSwsize = 7;
	res.PMVSminImageNum = 3;
	res.PMVSCPU = 4;
	res.CutCoef = 0.5;
	res.PoissonDepth = 12;
	res.NumberOfFaces = 20000;
	res.TextureSize = 2048;

	return res;
}
