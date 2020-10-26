/*
*   File:   whitePatch.cpp
*   Author: Ventura Lucena Martínez
*   Email:  i72lumav@uco.es
*/

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <vector>
#include "whitePatch.hpp"
#include "histogram.hpp"

using namespace std;
using namespace cv;

void whitePatch(const Mat &in, Mat &out)
{
	out = in.clone();
	//in.copyTo(out);
	//	Maximum and RGB_factor values.
	double max_RGB = 0.0, B_factor = 0.0, G_factor = 0.0, R_factor = 0.0;

	//	Check the brightest pixel on the image.
	for(int y = 0; y < in.rows; y++)
	{
		const uchar *ptr = in.ptr<uchar> (y);
		for(int x = 0; x < in.cols; x++)
		{
			if(max_RGB < ptr[0] + ptr[1] + ptr [2])
			{
				max_RGB = ptr[0] + ptr[1] + ptr [2];
				B_factor = ptr[0];
				G_factor = ptr[1];
				R_factor = ptr[2];
			}
			ptr += 3;
		}
	}

	B_factor = (double)255 / B_factor;
	G_factor = (double)255 / G_factor;
	R_factor = (double)255 / R_factor;

	//  Writing on the output:
	for(int y = 0; y < out.rows; y++)
	{
		uchar *ptr = out.ptr<uchar> (y);
		for(int x = 0; x < out.cols; x++)
		{
			if(ptr[0] * B_factor > 255)
				ptr[0] = 255;
			else
				ptr[0] = ptr[0] * B_factor;

			if(ptr[1] * G_factor > 255)
				ptr[1] = 255;
			else
				ptr[1] = ptr[1] * G_factor;

			if(ptr[2] * R_factor > 255)
				ptr[2] = 255;
			else
				ptr[2] = ptr[2] * R_factor;

			ptr += 3;
		}
	}
}

void whitePatch(const Mat &in, Mat &out, float percentage)
{
	out = in.clone();
	//in.copyTo(out);
	//	Maximum and RGB_factor values.
	double max_RGB = 0.0, B_factor = 0.0, G_factor = 0.0, R_factor = 0.0;
	//	Elements interacting in the mean.
	double elements;

	//  Check the brightest pixel on the image.
	for(int y = 0; y < in.rows; y++)
	{
		const uchar *ptr = in.ptr<uchar> (y);
		for(int x = 0; x < in.cols; x++)
		{
			double currentPixelBrightness = ptr[0] * 0.114 + 
											ptr[1] * 0.587 + 
											ptr[2] * 0.299;
			if(max_RGB < currentPixelBrightness)
			{
				max_RGB = currentPixelBrightness;
				B_factor = ptr[0];
				G_factor = ptr[1];
				R_factor = ptr[2];
			}
			ptr += 3;
		}
	}


	//  Calculate the histogram:
	vector<int> histogram_vector(256, 0);
	Mat grayscale_image;
	cvtColor(in, grayscale_image, COLOR_BGR2GRAY);
	histogram(grayscale_image, histogram_vector);
	//printHistogram(histogram_vector);

	int pixels = in.cols * in.rows;
	int n_brightest_pixels = pixels * percentage;
	double sum = 0.0, threshold = 255.0;

	for (threshold = 255; threshold >= 0; threshold--)
	{
		if(sum <= n_brightest_pixels)
			sum += histogram_vector[threshold];
		else
			break;
	}

	//cout << "Threshold = " << threshold << endl;

	//  Get the mean:
	double B_sum = 0.0, G_sum = 0.0, R_sum = 0.0, n = 0.0;

	for(int y = 0; y < out.rows; y++)
	{
		uchar *ptr = out.ptr<uchar> (y);
		for(int x = 0; x < out.cols; x++)
		{
			double currentPixelBrightness = ptr[0] * 0.114 + 
									   ptr[1] * 0.587 + 
									   ptr[2] * 0.299;

			if(currentPixelBrightness >= threshold)
			{
				B_sum += ptr[0];
				G_sum += ptr[1];
				R_sum += ptr[2];
				n++;
			}
			ptr += 3;
		}
	}

	double B_mean = B_sum / n, G_mean = G_sum / n, R_mean = R_sum / n;


	B_factor = (double)255 / B_mean;
	G_factor = (double)255 / G_mean;
	R_factor = (double)255 / R_mean;
	
	//cout << "\nB sum = " << B_sum << endl;
	//cout << "G sum = " << G_sum << endl;
	//cout << "R sum = " << R_sum << endl;

	//cout << "\nB mean = " << B_mean << endl;
	//cout << "G mean = " << G_mean << endl;
	//cout << "R mean = " << R_mean << endl;

	//cout << "\nB factor = " << B_factor << endl;
	//cout << "G factor = " << G_factor << endl;
	//cout << "R factor = " << R_factor << endl;

	//  Writing on the output:
	for(int y = 0; y < out.rows; y++)
	{
		uchar *ptr = out.ptr<uchar> (y);
		for(int x = 0; x < out.cols; x++)
		{
			//  https://docs.opencv.org/4.4.0/de/d25/imgproc_color_conversions.html#color_convert_rgb_gray
			double currentPixelBrightness = ptr[0] * 0.114 + 
									   ptr[1] * 0.587 + 
									   ptr[2] * 0.299;

			if(ptr[0] * B_factor > 255)
				ptr[0] = 255;
			else
				ptr[0] = ptr[0] * B_factor;

			if(ptr[1] * G_factor > 255)
				ptr[1] = 255;
			else
				ptr[1] = ptr[1] * G_factor;

			if(ptr[2] * R_factor > 255)
				ptr[2] = 255;
			else
				ptr[2] = ptr[2] * R_factor;
			
			ptr += 3;
		}
	}
}