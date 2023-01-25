
#include "Enums.hpp"

#include <visualizer.hpp>

#include <iostream>
#include <cstdlib>
using namespace std;

#include <unistd.h>

#define DEBUG(x) do{ std::cout << #x << " = " << x << std::endl; }while(0)

#include "opencv2/opencv.hpp"
using namespace cv;


Mat K_Means(Mat Input, int K) {
    Mat samples(Input.rows * Input.cols, Input.channels(), CV_32F);
    for (int y = 0; y < Input.rows; y++)
        for (int x = 0; x < Input.cols; x++)
            for (int z = 0; z < Input.channels(); z++)
                if (Input.channels() == 3) {
                    samples.at<float>(y + x * Input.rows, z) = Input.at<Vec3b>(y, x)[z];
                }
                else {
                    samples.at<float>(y + x * Input.rows, z) = Input.at<uchar>(y, x);
                }

    Mat labels;
    int attempts = 5;
    Mat centers;
    kmeans(samples, K, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);


    Mat new_image(Input.size(), Input.type());
    for (int y = 0; y < Input.rows; y++)
        for (int x = 0; x < Input.cols; x++)
        {
            int cluster_idx = labels.at<int>(y + x * Input.rows, 0);
            if (Input.channels()==3) {
                for (int i = 0; i < Input.channels(); i++) {
                    new_image.at<Vec3b>(y, x)[i] = centers.at<float>(cluster_idx, i);
                }
            }
            else {
                new_image.at<uchar>(y, x) = centers.at<float>(cluster_idx, 0);
            }
        }
    return new_image;
}



void findSign(Mat imgDil, Mat img){

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	//cout<< contours.size()<<endl;

	// Finding MAX conture
	Mat imgCrop, imgResize;
	int sign;
	int area, maxArea = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		if(contourArea(contours[i]) > maxArea) 
		{	
			maxArea = contourArea(contours[i]);
			sign = i;
		} 
		//cout<< contourArea(contours[i])<<endl;
	}

	// Drawing conture
	//cout << area << endl;
	float peri = arcLength(contours[sign], true);
	approxPolyDP(contours[sign], conPoly[sign], 0.02 * peri, true);
	//cout << conPoly[sign].size() << endl;
	//drawContours(img, conPoly, sign, Scalar(255, 0, 255), 1);
	
	// Frame
	boundRect[sign] = boundingRect(conPoly[sign]);
	rectangle(img, boundRect[sign].tl(), boundRect[sign].br(), Scalar(0, 255, 0), 1);

	// Points for crop	
	float w,h;
	Point tl,br;
	tl = boundRect[sign].tl();
	br = boundRect[sign].br();

	h = br.y - tl.y;
	w = br.x - tl.x;
	//cout << tl.x << endl;
	//cout << tl.y << endl;
	//cout << w << endl;
	//cout << h << endl;
	

	Rect roi(tl.x, tl.y, w, h);
	imgCrop = img(roi);

	imshow("Contours", img);
	waitKey(0);
	imshow("Crop", imgCrop);
	waitKey(0);

	// Black backround 
	for (int k = 0; k < img.rows; k++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			Point pixel;
			pixel.x = j;
			pixel.y = k;
			float dist = pointPolygonTest(contours[sign], pixel, false);

			//cout<<dist<<endl;

			if(dist == -1)
			{
				img.at<Vec3b>(pixel)[0] = 255;
				img.at<Vec3b>(pixel)[1] = 255;
				img.at<Vec3b>(pixel)[2] = 255;
				
			}
		}
	}
	
	resize(imgCrop, imgResize, Size(), 2.0, 2.0);

	imshow("BackgroundRemoved", imgCrop);
	waitKey(0);
	imshow("Resize", imgResize);
	waitKey(0);

	// Clustering
	int c = 3;
    Mat imgFinal = K_Means(imgResize, c);	

	imshow("Segmented", imgFinal);
	waitKey(0);
	
}


int main() {

	Mat srcHSV, mask1Red, mask2Red, maskGreen, maskYellow, maskBlack, maskBlue;

	Mat imgBlur, imgCanny, imgDil, imgErode;

	Mat src = cv::imread("data/opasnost.jpg");

	if(src.empty()){
		throw runtime_error("Cannot open image!");
	}

	imshow("Image", src);
	waitKey(0);

	cvtColor(src, srcHSV, COLOR_BGR2HSV);

	imshow("HSV", srcHSV);
	waitKey(0);

	inRange(srcHSV, Scalar(0, 61, 38), Scalar(10, 255, 255), mask1Red);
	inRange(srcHSV, Scalar(170, 61, 38), Scalar(180, 255, 255), mask2Red);
	inRange(srcHSV, Scalar(101, 150, 150), Scalar(140, 255, 255), maskBlue);
	inRange(srcHSV, Scalar(100, 0, 0), Scalar(180, 255, 30), maskBlack);
    inRange(srcHSV, Scalar(46, 61, 38), Scalar(100, 255, 255), maskYellow);
    inRange(srcHSV, Scalar(21, 150, 150), Scalar(33, 255, 255), maskGreen);	
	
	Mat mask = mask1Red | mask2Red | maskBlue | maskBlack| maskYellow | maskGreen;

	imshow("Mask", mask);
	waitKey(0);

	// Pretproccesing
	GaussianBlur(mask, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	erode(imgDil, imgErode, kernel);

	imshow("Blur", imgBlur);
	waitKey(0);
	imshow("Canny", imgCanny);
	waitKey(0);
	imshow("Dilate", imgDil);
	waitKey(0);
	imshow("Erode", imgErode);
	waitKey(0);

	// Detection
	findSign(mask, src);

	return 0;
}
