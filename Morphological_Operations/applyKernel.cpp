/*
*	File:	applyKernel.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#include <opencv2/core/core.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

#include "applyKernel.hpp"

using namespace std;
using namespace cv;

void medianBlur_grayscale(const Mat &in, Mat &out, const Mat &kernel)
{
	if(kernel.cols <= 1)
		throw runtime_error("Kernel size too small.");

	if(kernel.cols != kernel.rows)
		throw runtime_error("Kernel not squared.");

	if(kernel.cols % 2 == 0)
		throw runtime_error("Kernel not odd.");

	//	Squares to get at the edge of the kernel, depending on the area of the kernel itself.
	int kernel_amplitude = (kernel.cols / 2);
	vector<uchar> v;

	for(int y = kernel_amplitude; y < in.rows - kernel_amplitude; y++)
	{
		for(int x = kernel_amplitude; x < in.cols - kernel_amplitude; x++)
		{
			//float sum = 0.0;
			for (int k = 0; k < kernel.rows; k++)
			{
				for (int z = 0; z < kernel.cols; z++)
				{
					//	The sum of the products of the kernel and the image is the convolution image.
					v.push_back(in.at<uchar> (y + k - kernel_amplitude, x + z - kernel_amplitude) * kernel.at<float> (k, z));
				}
			}
			sort(v.begin(), v.end());
			//	The result of each pixel.
			out.at<uchar> (y, x) = v[(v.size() / 2)];
			v.clear();
		}
	}
}

void medianBlur_colour(const cv::Mat &in, cv::Mat &out, const cv::Mat &kernel)
{
	if(kernel.cols <= 1)
		throw runtime_error("Kernel size too small.");

	if(kernel.cols != kernel.rows)
		throw runtime_error("Kernel not squared.");

	if(kernel.cols % 2 == 0)
		throw runtime_error("Kernel not odd.");

	//	Squares to get at the edge of the kernel, depending on the area of the kernel itself.
	int kernel_amplitude = (kernel.cols / 2);
	vector<uchar> v0;
	vector<uchar> v1;
	vector<uchar> v2;

	for(int y = kernel_amplitude; y < in.rows - kernel_amplitude; y++)
	{
		for(int x = kernel_amplitude; x < in.cols - kernel_amplitude; x++)
		{
			//float sum = 0.0;
			for (int k = 0; k < kernel.rows; k++)
			{
				for (int z = 0; z < kernel.cols; z++)
				{
					//	The sum of the products of the kernel and the image is the convolution image.
					v0.push_back(in.at<Vec3b> (y + k - kernel_amplitude, x + z - kernel_amplitude) [0] * kernel.at<float> (k, z));
					v1.push_back(in.at<Vec3b> (y + k - kernel_amplitude, x + z - kernel_amplitude) [1] * kernel.at<float> (k, z));
					v2.push_back(in.at<Vec3b> (y + k - kernel_amplitude, x + z - kernel_amplitude) [2] * kernel.at<float> (k, z));
				}
			}
			sort(v0.begin(), v0.end());
			sort(v1.begin(), v1.end());
			sort(v2.begin(), v2.end());
			//	The result of each pixel.
			out.at<Vec3b> (y, x)[0] = v0[(v0.size() / 2)];
			out.at<Vec3b> (y, x)[1] = v1[(v1.size() / 2)];
			out.at<Vec3b> (y, x)[2] = v2[(v2.size() / 2)];

			v0.clear();
			v1.clear();
			v2.clear();
		}
	}
}

void applyKernel_grayscale(const Mat &in, Mat &out, const Mat &kernel)
{
	if(kernel.cols <= 1)
		throw runtime_error("Kernel size too small.");

	if(kernel.cols != kernel.rows)
		throw runtime_error("Kernel not squared.");

	if(kernel.cols % 2 == 0)
		throw runtime_error("Kernel not odd.");

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
					sum += in.at<uchar> (y + k - kernel_amplitude, x + z - kernel_amplitude) * kernel.at<float> (k, z);
				}
			}
			out.at<uchar> (y, x) = sum;
		}
	}
}

void applyKernel_colour(const Mat &in, Mat &out, const Mat &kernel)
{
	if(kernel.cols <= 1)
		throw runtime_error("Kernel size too small.");

	if(kernel.cols != kernel.rows)
		throw runtime_error("Kernel not squared.");

	if(kernel.cols % 2 == 0)
		throw runtime_error("Kernel not odd.");

	int kernel_amplitude = (kernel.cols / 2);

	for(int y = kernel_amplitude; y < in.rows - kernel_amplitude; y++)
	{
		for(int x = kernel_amplitude; x < in.cols - kernel_amplitude; x++)
		{
			float sum0 = 0.0, sum1 = 0.0, sum2 = 0.0;
			for (int k = 0; k < kernel.rows; k++)
			{
				for (int z = 0; z < kernel.cols; z++)
				{
					sum0 += in.at<Vec3b> (y + k - kernel_amplitude, x + z - kernel_amplitude) [0] * kernel.at<float> (k, z);
					sum1 += in.at<Vec3b> (y + k - kernel_amplitude, x + z - kernel_amplitude) [1] * kernel.at<float> (k, z);
					sum2 += in.at<Vec3b> (y + k - kernel_amplitude, x + z - kernel_amplitude) [2] * kernel.at<float> (k, z);
				}
			}
			out.at<Vec3b> (y, x)[0] = sum0;
			out.at<Vec3b> (y, x)[1] = sum1;
			out.at<Vec3b> (y, x)[2] = sum2;
		}
	}
}

void erode_grayscale(const Mat &in, Mat &out, const Mat &kernel)
{
	if(kernel.cols <= 1)
		throw runtime_error("Kernel size too small.");

	if(kernel.cols != kernel.rows)
		throw runtime_error("Kernel not squared.");

	if(kernel.cols % 2 == 0)
		throw runtime_error("Kernel not odd.");

	int kernel_amplitude = (kernel.cols / 2);
	vector<uchar> v;

	for(int y = kernel_amplitude; y < in.rows - kernel_amplitude; y++)
	{
		for(int x = kernel_amplitude; x < in.cols - kernel_amplitude; x++)
		{
			for (int k = 0; k < kernel.rows; k++)
			{
				for (int z = 0; z < kernel.cols; z++)
				{
					v.push_back(in.at<uchar> (y + k - kernel_amplitude, x + z - kernel_amplitude) * kernel.at<float> (k, z));
				}
			}

			auto minimum = v[0];
			for(auto i = 1; i < v.size(); i++)
			{
				if(v[i] < minimum)
					minimum = v[i];
			}

			out.at<uchar> (y, x) = minimum;
			v.clear();
		}
	}
}

void dilation_grayscale(const Mat &in, Mat &out, const Mat &kernel)
{
	if(kernel.cols <= 1)
		throw runtime_error("Kernel size too small.");

	if(kernel.cols != kernel.rows)
		throw runtime_error("Kernel not squared.");

	if(kernel.cols % 2 == 0)
		throw runtime_error("Kernel not odd.");

	int kernel_amplitude = (kernel.cols / 2);
	vector<uchar> v;

	for(int y = kernel_amplitude; y < in.rows - kernel_amplitude; y++)
	{
		for(int x = kernel_amplitude; x < in.cols - kernel_amplitude; x++)
		{
			for (int k = 0; k < kernel.rows; k++)
			{
				for (int z = 0; z < kernel.cols; z++)
				{
					v.push_back(in.at<uchar> (y + k - kernel_amplitude, x + z - kernel_amplitude) * kernel.at<float> (k, z));
				}
			}
			
			auto maximum = v[0];
			for(auto i = 1; i < v.size(); i++)
			{
				if(v[i] > maximum)
					maximum = v[i];
			}

			out.at<uchar> (y, x) = maximum;
			v.clear();
		}
	}
}