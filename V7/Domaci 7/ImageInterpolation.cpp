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
    for(int i = 0; i < xSize; i++)
    {
        for(int j = 0; j < ySize; j++)
        {
            int Xp = floor(i * cos(angle) - j * sin(angle) - m * cos(angle) + n * sin(angle) + m);
            int Yp = floor(j * cos(angle) + i * sin(angle) - m * sin(angle) - n * cos(angle) + n);


            if(Xp > xSize || Xp < 0 || Yp < 0 || Yp > ySize)
            {
                output[j * 3 * xSize + i * 3] = 255;
                output[j * 3 * xSize + i * 3 + 1] = 255;
                output[j * 3 * xSize + i * 3 + 2] = 255;
            } else
            {
                output[j * 3 * xSize + i * 3] =  input[Yp * 3 * xSize + Xp * 3];
                output[j * 3 * xSize + i * 3 + 1] = input[Yp * 3 * xSize + Xp * 3 + 1];
                output[j * 3 * xSize + i * 3 + 2] = input[Yp * 3 * xSize + Xp * 3 + 2];
            }

        }
    }

}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
    /* TO DO */
    for(int i = 0; i < xSize; i++)
    {
        for(int j = 0; j < ySize; j++)
        {
            double Xp = i * cos(angle) - j * sin(angle) - m * cos(angle) + n * sin(angle) + m;
            double Yp = j * cos(angle) + i * sin(angle) - m * sin(angle) - n * cos(angle) + n;
            double a = Xp - floor(Xp);
            double b = Yp - floor(Yp);

            int m = floor(Xp);
            int n = floor(Yp);

            for (int k = 0; k < 3; k++)
            {
                if(m > xSize || m < 0 || n < 0 || n > ySize)
                {
                    output[j * 3 * xSize + i * 3 + k] = 255;
                } else
                {
                    double X00 = (1 - a) * (1 - b) * input[n * xSize * 3 + m * 3 + k];
                    double X01 = (a) * (1 - b) * input[n * xSize * 3 + (m+1) * 3 + k];
                    double X10 = (1 - a) * (b) * input[(n+1) * xSize * 3 + m * 3 + k];
                    double X11 = (a) * (b) * input[(n + 1) * xSize * 3 + (m+1) * 3 + k];

                    output[j * xSize * 3 + i * 3 + k] = round(X00 + X01 + X10 + X11);
                }
            }

        }
    }

}


