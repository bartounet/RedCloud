#include "s_options.h"

s_options s_options::getOptionsLow()
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
	res.PMVSCPU = 8;
	res.CutCoef = 0.5;
	res.PoissonDepth = 11;
	res.NumberOfFaces = 20000;
	res.TextureSize = 2048;

	return res;
}

s_options s_options::getOptionsMedium()
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
	res.PMVSCPU = 8;
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
	res.PMVSCPU = 8;
	res.CutCoef = 0.5;
	res.PoissonDepth = 11;
	res.NumberOfFaces = 20000;
	res.TextureSize = 2048;

	return res;
}
