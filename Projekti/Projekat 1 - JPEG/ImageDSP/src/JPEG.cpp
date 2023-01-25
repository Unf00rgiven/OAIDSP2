#include "JPEG.h"
#include "NxNDCT.h"
#include <cmath>
#include "JPEGBitStreamWriter.h"


#define DEBUG(x) do{ qDebug() << #x << " = " << x;}while(0)



// quantization tables from JPEG Standard, Annex K
const uint8_t QuantLuminance[8*8] =
    { 16, 11, 10, 16, 24, 40, 51, 61,
      12, 12, 14, 19, 26, 58, 60, 55,
      14, 13, 16, 24, 40, 57, 69, 56,
      14, 17, 22, 29, 51, 87, 80, 62,
      18, 22, 37, 56, 68,109,103, 77,
      24, 35, 55, 64, 81,104,113, 92,
      49, 64, 78, 87,103,121,120,101,
      72, 92, 95, 98,112,100,103, 99 };
const uint8_t QuantChrominance[8*8] =
    { 17, 18, 24, 47, 99, 99, 99, 99,
      18, 21, 26, 66, 99, 99, 99, 99,
      24, 26, 56, 99, 99, 99, 99, 99,
      47, 66, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99 };


uint8_t quantQuality(uint8_t quant, uint8_t quality) {
	// Convert to an internal JPEG quality factor, formula taken from libjpeg
	int16_t q = quality < 50 ? 5000 / quality : 200 - quality * 2;
	return clamp((quant * q + 50) / 100, 1, 255);
}

static void doZigZag(short block[], int N, short** output)
{
    int currNum = 0;
    int currDiagonalWidth = 1;
    int i;
    int row;
    int col;
    int k = 0;

    short* out = new short[N*N];

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

            out[k] = block[row * N + col];
            k++;
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
            out[k] = block[row * N + col];
            k++;
        }

        currDiagonalWidth--;
    }

    *output = out;
}
static void doZigZagZZ(uint8_t block[], int N, uint8_t** output)
{
    int currNum = 0;
    int currDiagonalWidth = 1;
    int i;
    int row;
    int col;
    int k = 0;

    uint8_t* out = new uint8_t[N*N];

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

            out[k] = block[row * N + col];
            k++;
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
            out[k] = block[row * N + col];
            k++;
        }

        currDiagonalWidth--;
    }

    *output = out;
}


//JPEGBitStreamWriter streamer("example.jpg");
void performJPEGEncoding(uchar Y_buff[], char U_buff[], char V_buff[], int xSize, int ySize, int quality)
{
    DEBUG(quality);
    // BLOCK SIZE
    int N = 8;

    auto s = new JPEGBitStreamWriter("example.jpg");

    // SIGNED Y -> UNSIGNED Y
    char* Y_buffchar = new char[xSize*ySize];
    for(int i = 0; i < ySize; i++)
    {
        for(int k = 0; k < xSize; k++)
            Y_buffchar[i*xSize + k] = Y_buff[i*xSize + k] - 128.0;
    }

    int newxSizeY = xSize;
    int newySizeY = ySize;
    int newxSizeU = xSize / 2;
    int newySizeU = ySize / 2;
    int newxSizeV = xSize / 2;
    int newySizeV = ySize / 2;


    char* Y_block1 = new char[N*N];
    char* Y_block2 = new char[N*N];
    char* Y_block3 = new char[N*N];
    char* Y_block4 = new char[N*N];
    char* U_block = new char[N*N];
    char* V_block = new char[N*N];

    short* Ycoeffs1 = new short[N*N];
    short* Ycoeffs2 = new short[N*N];
    short* Ycoeffs3 = new short[N*N];
    short* Ycoeffs4 = new short[N*N];
    short* Ucoeffs = new short[N*N];
    short* Vcoeffs = new short[N*N];

    short* Ycoeffs1out = new short[N*N];
    short* Ycoeffs2out = new short[N*N];
    short* Ycoeffs3out = new short[N*N];
    short* Ycoeffs4out = new short[N*N];
    short* Ucoeffsout = new short[N*N];
    short* Vcoeffsout = new short[N*N];

    char* outputY = new char[xSize*ySize];
    char* outputV = new char[newxSizeV*newySizeV];
    char* outputU = new char[newxSizeU*newySizeU];

    // Extend borders
    if(xSize%8 != 0 || ySize%8 != 0)
    {
        extendBorders(Y_buffchar, xSize, ySize, N, &outputY, &newxSizeY, &newySizeY);
        extendBorders(U_buff, xSize, ySize, N, &outputU, &newxSizeV, &newySizeV);
        extendBorders(V_buff, xSize, ySize, N, &outputV, &newxSizeU, &newySizeU);
    }
    else
    {
        outputY = Y_buffchar;
        outputV = V_buff;
        outputU = U_buff;
    }

    // ZIGZAG on LUMINANCE, CHROMINANCE matrix
    uint8_t* luminance = new uint8_t[N*N];
    uint8_t* chrominance = new uint8_t[N*N];
    uint8_t* luminanceZZ = new uint8_t[N*N];
    uint8_t* chrominanceZZ = new uint8_t[N*N];
    for(int i = 0;i < N; i++)
    {
        for(int k = 0; k < N; k++)
        // Quality parameter
        {
            luminance[i*N + k] = quantQuality(QuantLuminance[i*N + k], quality);
            chrominance[i*N + k] = quantQuality(QuantChrominance[i*N + k], quality);
        }
    }
    doZigZagZZ(luminance, N, &luminanceZZ);
    doZigZagZZ(chrominance, N, &chrominanceZZ);

    s->writeHeader();
    s->writeQuantizationTables((uint8_t*)luminanceZZ, (uint8_t*)chrominanceZZ);
    s->writeImageInfo(newxSizeY, newySizeY);
    s->writeHuffmanTables();

    // Kernel
    double* DCTKernel = new double[N*N];
    GenerateDCTmatrix(DCTKernel, N);

    for(int y = 0, yimage = 0; yimage < newySizeY/N; y++, yimage += 2)
    {
        for(int x = 0, ximage = 0; ximage < newxSizeY/N; x++, ximage += 2)
        {
            for(int i = 0; i < N; i++)
            {
                for(int k = 0; k < N; k++)
                {
                    // Block fill
                    Y_block1[i*N+k] = outputY[(N*yimage+i)*(newxSizeY)+N*ximage+k];
                    Y_block2[i*N+k] = outputY[(N*yimage+i)*(newxSizeY)+N*(ximage+1)+k];
                    Y_block3[i*N+k] = outputY[(N*(yimage+1)+i)*(newxSizeY)+N*ximage+k];
                    Y_block4[i*N+k] = outputY[(N*(yimage+1)+i)*(newxSizeY)+N*(ximage+1)+k];
                    U_block[i*N+k] = outputU[(N*y+i)*(newxSizeU)+N*x+k];
                    V_block[i*N+k] = outputV[(N*y+i)*(newxSizeV)+N*x+k];
                }
            }

        // DCT
        DCT(Y_block1, Ycoeffs1, N, DCTKernel);
        DCT(Y_block2, Ycoeffs2, N, DCTKernel);
        DCT(Y_block3, Ycoeffs3, N, DCTKernel);
        DCT(Y_block4, Ycoeffs4, N, DCTKernel);
        DCT(U_block, Ucoeffs, N, DCTKernel);
        DCT(V_block, Vcoeffs, N, DCTKernel);

        // QUANTIZATION
        for(int i = 0; i < N; i++)
        {
            for(int k = 0; k < N; k++)
            {
                Ycoeffs1 [k*N + i] = round(Ycoeffs1 [k*N + i] / luminance[k*N + i]);
                Ycoeffs2 [k*N + i] = round(Ycoeffs2 [k*N + i] / luminance[k*N + i]);
                Ycoeffs3 [k*N + i] = round(Ycoeffs3 [k*N + i] / luminance[k*N + i]);
                Ycoeffs4 [k*N + i] = round(Ycoeffs4 [k*N + i] / luminance[k*N + i]);
                Ucoeffs [k*N + i] = round(Ucoeffs [k*N + i] / chrominance[k*N + i]);
                Vcoeffs [k*N + i] = round(Vcoeffs [k*N + i] / chrominance[k*N + i]);
            }
        }


        // ZIGZAG for YUV
        doZigZag(Ycoeffs1, N, &Ycoeffs1out);
        doZigZag(Ycoeffs2, N, &Ycoeffs2out);
        doZigZag(Ycoeffs3, N, &Ycoeffs3out);
        doZigZag(Ycoeffs4, N, &Ycoeffs4out);
        doZigZag(Ucoeffs, N, &Ucoeffsout);
        doZigZag(Vcoeffs, N, &Vcoeffsout);

        // Write to file
        s->writeBlockY((int16_t*)Ycoeffs1out);
        s->writeBlockY((int16_t*)Ycoeffs2out);
        s->writeBlockY((int16_t*)Ycoeffs3out);
        s->writeBlockY((int16_t*)Ycoeffs4out);
        s->writeBlockU((int16_t*)Ucoeffsout);
        s->writeBlockV((int16_t*)Vcoeffsout);

        }
    }


    s->finishStream();

    delete[] Y_block1;
    delete[] Y_block2;
    delete[] Y_block3;
    delete[] Y_block4;
    delete[] U_block;
    delete[] V_block;

    delete[] Ycoeffs1;
    delete[] Ycoeffs2;
    delete[] Ycoeffs3;
    delete[] Ycoeffs4;
    delete[] Ucoeffs;
    delete[] Vcoeffs;

    delete[] Ycoeffs1out;
    delete[] Ycoeffs2out;
    delete[] Ycoeffs3out;
    delete[] Ycoeffs4out;
    delete[] Ucoeffsout;
    delete[] Vcoeffsout;

    delete[] DCTKernel;
    delete[] luminance;
    delete[] chrominance;
    delete[] Y_buffchar;

    delete s;
}

