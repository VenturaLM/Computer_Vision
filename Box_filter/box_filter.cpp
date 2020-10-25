/*
*	File:	box_filter.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

/*
Implement a 3x3 box filter for an image. 

Make a program that reads an image in grey scale, apply a box filter and save the result. To make it modular, create a function

void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel);

that applies any kernel 3x3 to the image.
*/

#include <iostream>
//  Core routines:
#include <opencv2/core/core.hpp>
//  Methods:    imread, imshow, namedWindow, waitKey
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void applyKernel(const Mat &in, Mat &out, const Mat &kernel)
{
	if(kernel.cols <= 1)
	{
		throw runtime_error("Kernel size too small.");
	}

	if(kernel.cols != kernel.rows)
	{
		throw runtime_error("Kernel not squared.");
	}

	if(kernel.cols % 2 == 0)
	{
		throw runtime_error("Kernel not odd.");
	}

	//	Squares to get at the edge of the kernel, depending on the area of the kernel itself.
	int kernel_amplitude = (kernel.cols / 2);

	for(int y = kernel_amplitude; y < in.rows - kernel_amplitude; y++)
	{
		for(int x = kernel_amplitude; x < in.cols - kernel_amplitude; x++)
		{
			float sum = 0.0;
			for (int k = 0; k < kernel.rows; k++)
			{
				for (int z = 0; z < kernel.cols; z++)
				{
					//	The sum of the products of the kernel and the image is the convolution image.
					sum += in.at<uchar> (y + k - kernel_amplitude, x + z - kernel_amplitude) * kernel.at<float> (k, z);
				}
			}
			//	The result of each pixel.
			out.at<uchar> (y, x) = sum;
		}
	}
}

int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		cerr << "Usage: image." << endl;
		return 0;
	}

	try
	{
		Mat image = imread(argv[1], IMREAD_GRAYSCALE);
		Mat result = image.clone();
		Mat kernel(3, 3, CV_32FC1);
		kernel = Mat::ones(3, 3, CV_32FC1) / (float)9;

		if(image.rows == 0)
		{
			cerr << "Error reading image." << endl;
			return 0;
		}

		applyKernel(image, result, kernel);

		namedWindow("Image");
		namedWindow("Result");

		imshow("Image", image);
		imshow("Result", result);

		waitKey(0);
	}catch(exception &ex)
	{
		cerr << ex.what() << endl;
	}
	return 0;
}