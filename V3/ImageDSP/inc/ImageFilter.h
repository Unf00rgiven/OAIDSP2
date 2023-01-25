#ifndef IMAGE_FILTER_H_
#define IMAGE_FILTER_H_
#include <QDebug>

void convolve2D(uchar image[], int xSize, int ySize, double* filterCoeff, int N);

void extendBorders (uchar input[], int xSize, int ySize, uchar output[], int delta);
	
void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages);

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, double threshold);

void performCannyEdgeDetection(uchar input[], int xSize, int ySize, double threshold1, double threshold2);

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, double threshold);

void performNFplusCannyEdgeDetection(uchar input[], int xSize, int ySize, int stages, double threshold1, double threshold2);

#endif // IMAGE_FILTER_H_
