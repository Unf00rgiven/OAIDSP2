#include "NoiseReduction.h"
#include "ImageFilter.h"

#include <vector>
#include <algorithm>
#include <cmath>


void performMovingAverage (uchar input[], int xSize, int ySize, int N)
{
	//TO DO
    double filterCoeff[N*N];
    for(int i = 0; i<N*N; i++)
    {
        filterCoeff[i] = 1.0/(N*N);
    }

    convolve2D(input, xSize, ySize, filterCoeff,  N);

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
    double gausCoeff[N*N];
    calculateGaussKernel(gausCoeff, N, sigma);
    convolve2D(input, xSize, ySize, gausCoeff,  N);
}

void bubbleSort(uchar input[], int n)
{
    int i,j;
    bool swapped;

    for(i = 0; i < n; i++)
    {
        swapped = false;
        for(j=0; j < n-i-1 ;j++)
        {
            if(input[j]>input[j+1])
            {
                uchar tmp = input[j];
                input[j] = input [j+1];
                input[j+1] = tmp;
                swapped = true;
            }
        }

        if (swapped = false) break;
    }

}

void performMedianFilter (uchar input[], int xSize, int ySize, int N)
{
	//TO DO
    int newXSize = xSize + (N - 1);
    int newYSize = ySize + (N - 1);
    int delta = (N-1)/2;

    uchar* extendedImage = new uchar[newXSize*newYSize];
    extendBorders(input, xSize, ySize, extendedImage, delta);

    for(int i = 0; i < xSize; i++)
    {
        for(int j = 0; j < ySize; j++)
        {

            uchar* sortBuffer = new uchar[N*N];

            for(int m = 0; m < N; m++ )
            {
                for(int n = 0; n < N; n++)
                {

                    sortBuffer[m*N + n] = extendedImage[(j + n) * newXSize + i + m];
                }
            }

            bubbleSort(sortBuffer, (N*N));

            int medijan = (N*N-1)/2;
            input[j*xSize + i] = sortBuffer[medijan];

            delete[] sortBuffer;


        }
    }

    delete[] extendedImage;
}


