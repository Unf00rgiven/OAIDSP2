#include "NoiseReduction.h"
#include "ImageFilter.h"

#include <vector>
#include <algorithm>
#include <cmath>


void performMovingAverage (uchar input[], int xSize, int ySize, int N)
{
	//TO DO
}

void calculateGaussKernel(double kernel[], int N, double sigma)
{
	double C = 0;
	for(int n = 0; n < N; n++)
    {
        for(int k = 0; k < N; k++)
        {
            kernel[n*N+k] = exp( -((n - N/2)*(n - N/2) + (k - N/2)*(k - N/2)) / (2 * sigma * sigma));
			C += kernel[n*N+k];
		}
	}

	C = 1.0 / C;

	for(int n = 0; n < N; n++)
    {
        for(int k = 0; k < N; k++)
        {
            kernel[n*N+k] *= C;
		}
	}
}

void performGaussFilter (uchar input[], int xSize, int ySize, int N, double sigma)
{
	//TO DO
}

void performMedianFilter (uchar input[], int xSize, int ySize, int N)
{
	//TO DO
}
