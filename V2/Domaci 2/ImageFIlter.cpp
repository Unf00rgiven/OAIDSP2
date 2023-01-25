#include "ImageFilter.h"
#include <cmath>


void convolve2D (uchar image[], int xSize, int ySize, double filterCoeff[], int N)
{
    //TO DO
    int delta = (N - 1)/2;



    uchar *extendedImage = new uchar[(xSize + 2 * delta) * (ySize + 2 * delta)];   // niz prosirene slike
    extendBorders(image, xSize, ySize, extendedImage, delta);                      // prosirenje slike i kopiranje elemenata

    for(int i = 0; i<xSize; i++)
    {

        for(int j=0; j<ySize; j++)
        {
            double accum = 0;

            for (int m = 0; m <N; m++)                               // iteriranje po velicini kernela
            {
                for (int n = 0; n <N; n++)
                {
                    accum += extendedImage[(j + n) * (xSize+ N - 1) + i + m] * filterCoeff[(N - n) * N - m - 1];
                }
            }
            if (accum > 255.0)                              // provera prekoracenja akumulirane vrednosti
            image[j * xSize + i] = 255;
            else if (accum < 0.0)
            image[j * xSize + i] = 0;
            else
            image[j * xSize + i] = floor(accum+0.5);
        }
    }

    delete[] extendedImage;             // oslobadjanje memorije, asistent naglasio da treba
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
    double coeffs[9] = {1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9};
    convolve2D (input , xSize, ySize, coeffs, 3);
}

void performVFFilter (uchar input[], int xSize, int ySize)
{
	//TO DO
    double coeffs[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    convolve2D (input , xSize, ySize, coeffs, 3);
}

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages)
{
	//TO DO
    double coeffs[9] = {1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9, 1.0/9};
    for(int i = 0; i < stages; i++)
    {
        convolve2D (input , xSize, ySize, coeffs, 3);
    }
}
