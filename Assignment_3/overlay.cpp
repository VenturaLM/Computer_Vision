/*
*	File:	overlay.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*
*	References:
*		- getPerspectiveTransform:	https://docs.opencv.org/4.5.0/da/d54/group__imgproc__transform.html#gae66ba39ba2e47dd0750555c7e986ab85
*		- warpPerspective:			https://docs.opencv.org/4.5.0/db/d29/group__cudawarping.html#ga7a6cf95065536712de6b155f3440ccff
*/

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "overlay.hpp"

using namespace std;
using namespace cv;

int getThirdPoint(float n)
{
	if (n <= 1)
		return 6;

	else if (n > 1 and n <= 2)
		return 12;

	else if (n > 2 and n <= 3)
		return 18;

	else if (n > 3 and n <= 4)
		return 24;

	else if (n > 4 and n <= 5)
		return 30;

	else if (n > 5 and n <= 6)
		return 36;

	else if (n > 6 and n <= 7)
		return 42;
	
	else if (n > 7)
		return 48;

	return 0;
}

void overlay(Mat image, vector<Point2f> centers, Mat frame)
{
	int third = getThirdPoint(6.0 * (float)image.rows / (float)image.cols);

	vector<Point2f> dst;
	dst.push_back(centers[0]);
	dst.push_back(centers[5]);
	dst.push_back(centers[third]);
	dst.push_back(centers[third + 5]);

	vector<Point2f> src;
	src.push_back(Point2f(0, 0));
	src.push_back(Point2f(image.cols - 1, 0));
	src.push_back(Point2f(0, image.rows - 1));
	src.push_back(Point2f(image.cols - 1, image.rows - 1));
		

	Mat changed = Mat(image);
	//	Calculates a perspective transform from four pairs of the corresponding points.
	Mat perspectivetransform = getPerspectiveTransform(src, dst);
	//	Applies a perspective transformation to an image.
	warpPerspective(image, changed, perspectivetransform, frame.size());

	Mat mask = changed > 0;
	Mat merged = frame.clone();
	changed.copyTo(merged, mask);
	imshow("Overlay", merged);
}