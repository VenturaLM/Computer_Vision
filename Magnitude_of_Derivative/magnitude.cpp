/*
*	File:	magnitude.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

/*	References:
*	Sobel derivative:	https://docs.opencv.org/3.4/d2/d2c/tutorial_sobel_derivatives.html
*	converTo():			https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html#adf88c60c5b4980e05bb556080916978b
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include "applyKernel.hpp"

using namespace std;
using namespace cv;

void magnitude(const Mat &sobel_result_x, const Mat &sobel_result_y, Mat &out)
{
	for(int y = 0; y < sobel_result_x.rows; y++)
	{
		for(int x = 0; x < sobel_result_x.cols; x++)
		{
			auto value_x = sobel_result_x.at<float> (y, x);
			auto value_y = sobel_result_y.at<float> (y, x);
			out.at<float> (y, x) = sqrt(pow(value_x, 2) + pow(value_y, 2));
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "Wrong execution!.\n\t./executable [input_image]" << endl;
		exit(-1);
	}

	try
	{
		//Mat image = imread(samples::findFile("lena.jpg"), IMREAD_GRAYSCALE);
		Mat image = imread(argv[1], IMREAD_GRAYSCALE);
		image.convertTo(image, CV_32FC1);
		Mat sobel_result_x = image.clone(), sobel_result_y = image.clone(), result = image.clone();

		if(image.empty())
		{
			cerr << "Error reading image." << endl;
			exit(-1);
		}

		float sobel_derivative_x[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
		float sobel_derivative_y[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
		Mat sobel_kernel_x(3, 3, CV_32FC1, sobel_derivative_x);
		Mat sobel_kernel_y(3, 3, CV_32FC1, sobel_derivative_y);
		//cout << sobel_kernel_x << endl;
		//cout << sobel_kernel_y << endl;

		applyKernel(image, sobel_result_x, sobel_kernel_x);
		applyKernel(image, sobel_result_y, sobel_kernel_y);

		magnitude(sobel_result_x, sobel_result_y, result);

		//	Must convert the image to <uchar>, as we initially do.
		result.convertTo(result, CV_8U);

		imshow("Result", result);

		waitKey(0);
	}catch(exception &ex)
	{
		cerr << ex.what() << endl;
	}
	return 0;
}