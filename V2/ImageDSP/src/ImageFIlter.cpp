#include "ImageFilter.h"


void convolve2D (uchar image[], int xSize, int ySize, double filterCoeff[], int N)
{
	//TO DO
}

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta)
{
	int newXSize = xSize + 2 * delta;
	int newYSize = ySize + 2 * delta;

	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			output[(j + delta) * newXSize + i + delta] = input[j * xSize + i];
		}
	}
	
	for (int i = 0; i < delta; i++)
	{
		memcpy(&output[i * newXSize + delta], &input[0], xSize);
		memcpy(&output[(ySize + delta + i)*newXSize + delta], &input[(ySize - 1) * xSize], xSize);
	}
	
	for (int i = 0; i < newYSize; i++)
	{
		memset(&output[i * newXSize], output[i * newXSize + delta], delta);
		memset(&output[i * newXSize + delta + xSize], output[i * newXSize + xSize + delta - 1], delta);
	}
}
	
void performNFFilter (uchar input[], int xSize, int ySize)
{
	//TO DO
}

void performVFFilter (uchar input[], int xSize, int ySize)
{
	//TO DO
}

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages)
{
	//TO DO
}
