/*
*	File:	applyKernel.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

void applyKernel(const Mat &in, Mat &out, const Mat &kernel)
{
	if(kernel.cols <= 1)
		throw runtime_error("Kernel size too small.");

	if(kernel.cols != kernel.rows)
		throw runtime_error("Kernel not squared.");

	if(kernel.cols % 2 == 0)
		throw runtime_error("Kernel not odd.");

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
					sum += in.at<float> (y + k - kernel_amplitude, x + z - kernel_amplitude) * kernel.at<float> (k, z);
				}
			}
			//	The result of each pixel.
			out.at<float> (y, x) = sum;
		}
	}
}