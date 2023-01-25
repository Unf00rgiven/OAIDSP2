#include "ImageHistogram.h"
#include "qcustomplot.h"


/*******************************************************************************************************************************/
/* Simple contrast improvement */
/*******************************************************************************************************************************/
void simpleContrastImprovement (const uchar input[], int xSize, int ySize, uchar output[])
{
	uchar min = input[0];
	uchar max = input[0];
	
	for (int i = 0; i < xSize * ySize; i++)
	{
		min = (min > input[i]) ? input[i] : min;
		max = (max < input[i]) ? input[i] : max;
	}

	for (int i = 0; i < xSize * ySize; i++)
	{
		output[i] = (input[i] - min) * 255 / (max - min);
	}
}


/*******************************************************************************************************************************/
/* Calculate image histogram */
/*******************************************************************************************************************************/
void calculateHistogram(const uchar input[], int xSize, int ySize, int histogram[])
{

	for (int i = 0; i < 256; i++)
		histogram[i] = 0;

	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			histogram[input[j * xSize + i]]++;
}


/*******************************************************************************************************************************/
/* Equalize image histogram */
/*******************************************************************************************************************************/
void equalizeHistogram(const uchar input[], int xSize, int ySize, uchar output[], uchar L)
{
	int histogram[256];
	calculateHistogram(input, xSize, ySize, histogram);

	int cdf[256];
	int pom = 0;

	for (int i = 0; i < 256; i++)
	{
		pom += histogram[i];
		cdf[i] = pom;
	}

	int cdf_min;
	for (int i = 0; i < 256; i++)
	{
		if (cdf[i] > 0)
		{
			cdf_min = cdf[i];
			break;
		}
	}

	for (int i = 0; i < xSize * ySize; i++)
	{
		output[i] = (cdf[input[i]] - cdf_min) * L / (xSize * ySize - cdf_min);
	}
}

/*******************************************************************************************************************************/
/* Modify colour saturation */
/*******************************************************************************************************************************/
void modifySaturation(const uchar inputRGB[], const uchar inputY[], int xSize, int ySize, uchar outputRGB[], double S)
{
	/* TODO */
}

/*******************************************************************************************************************************/
/* Plot image histogram */
/* Returns QImage containing graph with histogram data. */
/* start and end parameters define first and last value for X axis */
/*******************************************************************************************************************************/
QImage createHistogramPlot(int histogram[], int start, int end)
{
     QCustomPlot* customPlot = new QCustomPlot();
	 
	 int vectorSize = end-start + 1;
	 int maxValue = 0;
	 QVector<double> x(vectorSize), y(vectorSize);
	 for (int i=0; i<vectorSize; i++)
	 {
	   x[i] = i + start;  
	   y[i] = histogram[i];
	   maxValue = maxValue>=histogram[i] ? maxValue : histogram[i];
	 }
	 
	 // create graph and assign data to it:
     customPlot->addGraph();
     customPlot->graph(0)->setData(x, y);
	 // give the axes some labels:
     customPlot->xAxis->setLabel("x");
     customPlot->yAxis->setLabel("y");
	 // set axes ranges, so we see all data:
     customPlot->xAxis->setRange(start, end);
     customPlot->yAxis->setRange(0, maxValue);
     QImage outputImage = customPlot->toPixmap().toImage();
     
	 delete customPlot;
     return outputImage;
}


