/*
*	File:	overlay.hpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#ifndef __OVERLAY_HPP__
#define __OVERLAY_HPP__

using namespace std;
using namespace cv;

int getThirdPoint(float n);
void overlay(Mat image, vector<Point2f> centers, Mat frame);

#endif