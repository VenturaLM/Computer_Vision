/*
*	File:	median_blur.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

/*	References:
*		http://opencvexamples.blogspot.com/2013/10/assessing-pixel-values-of-image.html
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "applyKernel.hpp"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	try
	{
		if(argc != 2)
		{
			cout << "Usage: ./executable [input_image]" << endl;
			exit(-1);
		}

		cout << "\n\tSelect your option:\n\t\t1. Grayscaled-image.\n\t\t2. Coloured image.\n\t\t3. Help.\n\t\t4. Exit.\n\t> ";
		int option, size;
		cin >> option;
		switch(option)
		{
			//	Grayscale image.
			case 1:
			{
				Mat image = imread(argv[1], IMREAD_GRAYSCALE);
				Mat medianBlur_result = image.clone();
				Mat applyKernel_result = image.clone();

				cout << "\n\tSelect kernel size:\n\t> ";
				cin >> size;

				Mat medianBlur_kernel = Mat::ones(size, size, CV_32FC1);

				Mat applyKernel_kernel(size, size, CV_32FC1);
				applyKernel_kernel = Mat::ones(3, 3, CV_32FC1) / (float)9;

				if(image.empty())
				{
					cout << "Error reading the image." << endl;
					exit(-1);
				}
				medianBlur_grayscale(image, medianBlur_result, medianBlur_kernel);
				applyKernel_grayscale(image, applyKernel_result, applyKernel_kernel);

				imshow("Median blur filter", medianBlur_result);
				imshow("Box filter", applyKernel_result);
			}
			break;

			//	Colour image.
			case 2:
			{
				Mat image = imread(argv[1], CV_32FC1);
				Mat medianBlur_result = image.clone();
				Mat applyKernel_result = image.clone();

				cout << "\n\tSelect kernel size:\n\t> ";
				cin >> size;

				Mat medianBlur_kernel = Mat::ones(size, size, CV_32FC1);

				Mat applyKernel_kernel(size, size, CV_32FC1);
				applyKernel_kernel = Mat::ones(3, 3, CV_32FC1) / (float)9;

				if(image.empty())
				{
					cout << "Error reading the image." << endl;
					exit(-1);
				}
				medianBlur_colour(image, medianBlur_result, medianBlur_kernel);
				applyKernel_colour(image, applyKernel_result, applyKernel_kernel);

				imshow("Median blur filter", medianBlur_result);
				imshow("Box filter", applyKernel_result);
			}
			break;

			case 3:
				cout << "\n[Description]\n---------------\nThis program applies the median-blur filter to images. Then a new image corrected by a 1/9 kernel pops in order to be compared with the previous one.\n";
				cout << "\n[Usage]\n---------------\n1. Usage: ./executable [input_image]\n";
			break;

			case 4:
				cout << "Successfully exited." << endl;
				return 0;
			break;

			default:
			break;
		}

		waitKey(0);
	}catch(exception &ex)
	{
		cerr << ex.what() << endl;
	}
	return 0;
}