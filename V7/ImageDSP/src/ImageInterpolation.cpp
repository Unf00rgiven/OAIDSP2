#include "ImageInterpolation.h"
#include <math.h>


void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
	double Fx = ((double)newXSize) / xSize;
	double Fy = ((double)newYSize) / ySize;

	for (int i = 0; i < newXSize; i++)
	{
		for (int j = 0; j < newYSize; j++)
		{
			int ii = floor((i - Fx / 2) / Fx + 1);
			int jj = floor((j - Fy / 2) / Fy + 1);

			if (ii >= xSize)
				ii = xSize - 1;
			if (jj >= ySize)
				jj = ySize - 1;


			for (int k = 0; k < 3; k++)
			{

				output[j * newXSize * 3 + i * 3 + k] = input[jj * xSize * 3 + ii * 3 + k];
			}

		}

	}




}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	double Fx = ((double)newXSize) / xSize;
	double Fy = ((double)newYSize) / ySize;

	for (int i = 0; i < newXSize; i++)
	{
		for (int j = 0; j < newYSize; j++)
		{
			int m = floor(i / Fx);
			int n = floor(j / Fy);

			double a = i / Fx - m;
			double b = j / Fy - n;


			for (int k = 0; k < 3; k++)
			{
				double X00 = (1 - a) * (1 - b) * input[n * xSize * 3 + m * 3 + k];
				double X01 = (a) * (1 - b) * input[n * xSize * 3 + (m+1) * 3 + k];
				double X10 = (1 - a) * (b) * input[(n+1) * xSize * 3 + m * 3 + k];
				double X11 = (a) * (b) * input[(n + 1) * xSize * 3 + (m+1) * 3 + k];

				output[j * newXSize * 3 + i * 3 + k] = round(X00 + X01 + X10 + X11);
			}

		}

	}
}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}


