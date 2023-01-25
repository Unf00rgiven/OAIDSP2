#include "FeatureDetection.h"
#include "SIFTLib.h"
#include "ColorSpaces.h"
#include <list>

using namespace std;

/*******************************************************************************************************************************/
/* Find SIFT keypoints and mark each one with red color*/
/*******************************************************************************************************************************/
void SIFTDetect(uchar input[], int xSize, int ySize)
{
	SiftKeypointList kptList;

	uchar* Y_buff = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize / 4];
	char* V_buff = new char[xSize*ySize / 4];
	/* Convert input image to YUV420 image */
	RGBtoYUV420(input, xSize, ySize, Y_buff, U_buff, V_buff);


	/* Perform SIFT transformation  */
	/* TO DO*/

	/* Mark all keypoints in input image */
	/* TO DO*/

	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;
}

/*******************************************************************************************************************************/
/* Helper function. Splits image feature set in half (used for "two image" input)                                              */
/*******************************************************************************************************************************/
void splitFeatures(const SiftKeypointList& in, int width, SiftKeypointList& leftImageKP, SiftKeypointList& rightImageKP)
{
	for (SiftKeypoint kp : in)
	{
		if (kp.c < width / 2)
			leftImageKP.push_back(kp);
		else
			rightImageKP.push_back(kp);
	}
}

/*******************************************************************************************************************************/
/* Calculate Euclidean between two descriptor vectors                                                                          */
/*******************************************************************************************************************************/
double l2Distance(SiftKeypoint kp1, SiftKeypoint kp2)
{
	/* TO DO */
	return 0.0;
}

/*******************************************************************************************************************************/
/* Go through the first set of keypoints, and for each keypoint try to find the match in the second set                        */
/* One feature matches the other if Euclidean distance between them is lower than threshold.                                   */
/*******************************************************************************************************************************/
void matchFeatures(SiftKeypointList leftImageKP, SiftKeypointList rightImageKP, list<pair<QPoint, QPoint>>& matchPairs, double threshold)
{
	/* TO DO */
}

/*******************************************************************************************************************************/
/* Find SIFT keypoints, split the image in half, match the features from one image with those from another                     */
/* one and connect them with green lines                                                                                       */
/*******************************************************************************************************************************/
void SIFTDetectPlusMatch(uchar input[], int xSize, int ySize, double threshold)
{
	SiftKeypointList kptList, kptListLeft, kptListRight;

	uchar* Y_buff = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize / 4];
	char* V_buff = new char[xSize*ySize / 4];

	/* Convert input image to YUV420 image */
	/* TO DO */

	/* Perform SIFT transformation  */
	/* TO DO */

	/* Split the features of left and right images in separate lists */
	/* TO DO */

	/* Match the features from two images */
	list<pair<QPoint, QPoint>> matchedDots;
	/* TO DO */

	/* Draw a line for each mached feature pair */
	/* TO DO */

	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;

}

/*******************************************************************************************************************************/
/* Harris algorithm for corner detection                                                                                       */
/*******************************************************************************************************************************/
void HarisCornerDetection(uchar input[], int xSize, int ySize, double threshold, double alpha)
{
	double* sobelVertical = new double[xSize*ySize];
	double* sobelHorizontal = new double[xSize*ySize];
	uchar* Y_buff = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize / 4];
	char* V_buff = new char[xSize*ySize / 4];

	/* Convert input image to YUV420 image */
	RGBtoYUV420(input, xSize, ySize, Y_buff, U_buff, V_buff);

	/* Filter both images with corresponding Sobel operator */
	/* TO DO: */

	/* For each pixel with coordinates i,j , calculate the mSumIx, mSumIy and mSumIxy, then calculate the R factor and place it in new matrix */
	/* TO DO: */

	/* Find min and max of matrix R and scale entire matrix to [0, 255] */
	/* TO DO: */

	/* For each entry in R matrix, if the value is greater then threshold, check the 3x3 block around the pixel and if it is local maximum, draw a red circle in input image with radius 3 */

	delete[] sobelVertical;
	delete[] sobelHorizontal;
	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;

}


