/*
*	File:	histogram.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "histogram.hpp"

using namespace std;
using namespace cv;

void histogram(const Mat &in, vector<int> &histogram)
{
	for(int y = 0 ; y < in.rows; y++)
	{
		for(int x = 0; x < in.cols; x++)
		{
			//  Reading the pixel.
			auto value = in.at<uchar> (y, x);
			histogram[value] = histogram[value] + 1;
		}
	}
}

void cumulativeHistogram(const Mat &in, vector<int> &histogram, vector<int> &cumulative_histogram)
{
	for(int i = 0; i < cumulative_histogram.size(); i++)
	{
		cumulative_histogram[i] = histogram[i] + cumulative_histogram[i - 1];
	}
}

void applyHistogram(const Mat &in, Mat &out, vector<int> &cumulative_histogram, vector<int> &factors, double &factor)
{
	for(int i = 0; i < cumulative_histogram.size(); i++)
	{
		factors[i] = cumulative_histogram[i] * factor;
	}

	for(int y = 0 ; y < in.rows; y++)
	{
		for(int x = 0; x < in.cols; x++)
		{
			out.at<uchar>(y, x) = factors[out.at<uchar>(y, x)];
		}
	}
}

void printHistogram(const vector<int> &histogram)
{
	for(int i = 0; i < histogram.size(); i++)
	{
		cout << "H[" << i << "] = " << histogram[i] << endl;
	}
}