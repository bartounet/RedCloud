#ifndef S_OPTIONS_H
#define S_OPTIONS_H

struct s_options
{
	int MaxPhotoDimension;
	int BundlerInitPair;
	int CMVSNbClusters;
	int PMVSlevel;
	int PMVScsize;
	float PMVSthreshold;
	int PMVSwsize;
	int PMVSminImageNum;
	int PMVSCPU;
	float CutCoef;
	int PoissonDepth;
	int NumberOfFaces;
	int TextureSize;
	static s_options getOptionsLow();
	static s_options getOptionsMedium();
	static s_options getOptionsHigh();
};

typedef struct s_options s_options;

#endif // S_OPTIONS_H
