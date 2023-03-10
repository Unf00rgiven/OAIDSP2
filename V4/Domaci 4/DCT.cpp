#include "DCT.h"
#include "NxNDCT.h"
#include <math.h>

/* perform DCT */
void performDCT(uchar input[], int xSize, int ySize, int N)
{
	/* Create NxN buffer for one input block */
	uchar* inBlock = new uchar[N*N];

	/* Generate DCT kernel */
	double* DCTKernel = new double[N*N];
	GenerateDCTmatrix(DCTKernel, N);

	/* Create buffer for DCT coefficients */
	short* dctCoeffs = new short[N*N];

	/* Extend image borders */
	int xSize2, ySize2;
	uchar* input2;
	extendBorders(input, xSize , ySize, N, &input2, &xSize2, &ySize2);

	for (int y = 0; y < ySize2/N; y++)
	{
		for (int x = 0; x < xSize2/N; x++)
		{	

			/* Fill input block buffer */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					inBlock[j*N+i] = input2[(N*y+j)*(xSize2)+N*x+i];
				}
			}

			/* Invoke DCT */
			DCT(inBlock, dctCoeffs, N, DCTKernel);	

			/* Prepere DCT coefficients for drawing and put them in input */
			for(int i = 0; i < N*N; i++)
			{
				dctCoeffs[i] = abs(dctCoeffs[i]);
				if(dctCoeffs[i] < 0)
				{
					dctCoeffs[i] = 0;
				}
				else if(dctCoeffs[i] > 255)
				{
					dctCoeffs[i] = 255;
				}

				inBlock[i] = dctCoeffs[i];
			}

			/* Write output values to output image */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					input2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
				}
			}
		}
	}

	cropImage(input2, xSize2, ySize2, input, xSize, ySize);

	/* Delete used memory buffers coefficients */
	delete[] dctCoeffs;
	delete[] inBlock;
	delete[] DCTKernel;
	delete[] input2;
}

/* perform DCT and IDCT */
void performDCTandIDCT(uchar* input, int xSize, int ySize, int N)
{
	/* Create NxN buffer for one input block */
	uchar* inBlock = new uchar[N*N];

	/* Generate DCT kernel */
	double* DCTKernel = new double[N*N];
	GenerateDCTmatrix(DCTKernel, N);

	/* Create buffer for DCT coefficients */
	short* dctCoeffs = new short[N*N];

	/* Extend image borders */
	int xSize2, ySize2;
	uchar* input2;
	extendBorders(input, xSize , ySize, N, &input2, &xSize2, &ySize2);

	for (int y = 0; y < ySize2/N; y++)
	{
		for (int x = 0; x < xSize2/N; x++)
		{	

			/* Fill input block buffer */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					inBlock[j*N+i] = input2[(N*y+j)*(xSize2)+N*x+i];
				}
			}

			/* Invoke DCT */
			DCT(inBlock, dctCoeffs, N, DCTKernel);	



			/* Invoke IDCT */
			IDCT(dctCoeffs, inBlock, N, DCTKernel);	

			/* Write output values to output image */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					input2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
				}
			}
		}
	}

	cropImage(input2, xSize2, ySize2, input, xSize, ySize);

	/* Delete used memory buffers coefficients */
	delete[] dctCoeffs;
	delete[] inBlock;
	delete[] DCTKernel;
	delete[] input2;
}

/* 8x8 Quantization matrix */
static uchar quantizationMatrix[64] = 
{
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
};

/* perform quantization of DCT coeffs using Q matrix*/
void performDCTQuantization(uchar input[], int xSize, int ySize)
{
	/* TO DO */

    int N = 8;

    /* Create NxN buffer for one input block */
    uchar* inBlock = new uchar[N*N];

    /* Generate DCT kernel */
    double* DCTKernel = new double[N*N];
    GenerateDCTmatrix(DCTKernel, N);

    /* Create buffer for DCT coefficients */
    short* dctCoeffs = new short[N*N];

    /* Extend image borders */
    int xSize2, ySize2;
    uchar* input2;
    extendBorders(input, xSize , ySize, N, &input2, &xSize2, &ySize2);

    for (int y = 0; y < ySize2/N; y++)
    {
        for (int x = 0; x < xSize2/N; x++)
        {

            /* Fill input block buffer */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    inBlock[j*N+i] = input2[(N*y+j)*(xSize2)+N*x+i];
                }
            }

            /* Invoke DCT */
            DCT(inBlock, dctCoeffs, N, DCTKernel);


            for (int j = 0; j < N; j++)
            {
                for (int i = 0; i < N; i++)
                {
                    dctCoeffs[j*N + i] = round(dctCoeffs[j*N + i] / quantizationMatrix[j*N + i])*quantizationMatrix[j*N + i];
                }
            }


            /* Invoke IDCT */
            IDCT(dctCoeffs, inBlock, N, DCTKernel);

            /* Write output values to output image */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    input2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
                }
            }
        }
    }

    double MSE = 0;

    for(int i = 0; i < xSize; i++){
        for(int j = 0; j < ySize; j++){
            MSE += pow(input[i*xSize + j] - input2[i*xSize +j], 2);
        }
    }

    MSE /= (xSize * ySize);

    qInfo() << "MSE: " << MSE;


    cropImage(input2, xSize2, ySize2, input, xSize, ySize);

    /* Delete used memory buffers coefficients */
    delete[] dctCoeffs;
    delete[] inBlock;
    delete[] DCTKernel;
    delete[] input2;
}


static void doZigZag(short block[], int N, int B)
{
	/* TO DO */
    int currNum = 0;
    int currDiagonalWidth = 1;
    int i;
    int row;
    int col;

    while (currDiagonalWidth<N)
    {
        for (i = 0; i<currDiagonalWidth; i++)
        {
            if (currDiagonalWidth % 2 == 1)
            {
                row = currDiagonalWidth - i - 1;
                col = i;
            }
            else
            {
                row = i;
                col = currDiagonalWidth - i - 1;
            }
            // OBRADA PIKSELA SA KOORDINATAMA row I col
            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }
        currDiagonalWidth++;
    }
    while (currDiagonalWidth> 0)
    {
        for (i = currDiagonalWidth; i> 0; i--)
        {
            if (currDiagonalWidth % 2 == 1){
                row = N - currDiagonalWidth + i - 1;
                col = N - i;
            }
            else
            {
                row = N - i;
                col = N - currDiagonalWidth + i - 1;
            }
            // OBRADA PIKSELA SA KOORDINATAMA row I col
            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }

        currDiagonalWidth--;
    }

}


/* perform masking of DCT coeffs */
void performMaskDCTCoeffs(uchar input[], int xSize, int ySize, int N, int B)
{
	/* TO DO */
    /* Create NxN buffer for one input block */
    uchar* inBlock = new uchar[N*N];

    /* Generate DCT kernel */
    double* DCTKernel = new double[N*N];
    GenerateDCTmatrix(DCTKernel, N);

    /* Create buffer for DCT coefficients */
    short* dctCoeffs = new short[N*N];

    /* Extend image borders */
    int xSize2, ySize2;
    uchar* input2;
    extendBorders(input, xSize , ySize, N, &input2, &xSize2, &ySize2);

    for (int y = 0; y < ySize2/N; y++)
    {
        for (int x = 0; x < xSize2/N; x++)
        {

            /* Fill input block buffer */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    inBlock[j*N+i] = input2[(N*y+j)*(xSize2)+N*x+i];
                }
            }

            /* Invoke DCT */
            DCT(inBlock, dctCoeffs, N, DCTKernel);

            doZigZag(dctCoeffs, N, B);

            /* Invoke IDCT */
            IDCT(dctCoeffs, inBlock, N, DCTKernel);

            /* Write output values to output image */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    input2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
                }
            }
        }
    }

    cropImage(input2, xSize2, ySize2, input, xSize, ySize);

    /* Delete used memory buffers coefficients */
    delete[] dctCoeffs;
    delete[] inBlock;
    delete[] DCTKernel;
    delete[] input2;

}
