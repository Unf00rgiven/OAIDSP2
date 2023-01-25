
#ifndef _K_MEANS_
#define _K_MEANS_

#include <QtGlobal>
#include <vector>

typedef struct
{
	//point coordinates
	int x; 
	int y;
	
	//feature vector
	std::vector<double> feature;
} KMeansPoint;

void IntensityBasedKMeans(uchar input[], int xSize, int ySize, int K);

void ColorBasedKMeans(uchar input[], int xSize, int ySize, int K);

void IntensityPlusPositionBasedKMeans(uchar input[], int xSize, int ySize, int K);

void histogramBasedKMeans(uchar input[], int xSize, int ySize, int blockSize, int K);

void SIFTBasedKMeans(uchar input[], int xSize, int ySize, int blockSize, int K);

#endif //  _K_MEANS_
