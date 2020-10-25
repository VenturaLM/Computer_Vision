/*
*	File:	histogram.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	try{
		if(argc != 2)
		{
			cerr << "Usage: image." << endl;
			return 0;
		}

		//	The read image is directly in grayscale.
		Mat image = imread(argv[1], IMREAD_GRAYSCALE);
		Mat image2 = image.clone();

		if(image.rows == 0)
		{
			cerr << "Error reading image." << endl;
			return 0;
		}

		vector<int> histogram(256, 0);
		vector<double> cumulative_histogram(256, 0);
		vector<double> factors(256, 0);

//======================================================================================
		//HISTOGRAM
		for(int y = 0 ; y < image.rows; y++)
		{
			for(int x = 0; x < image.cols; x++)
			{
				//  Reading the pixel.
				auto value = image.at<uchar> (y, x);
				histogram[value] = histogram[value] + 1;
			}
		}

//======================================================================================
		//CUMULATIVE HISTOGRAM
		for(int i = 0; i < cumulative_histogram.size(); i++)
		{
			cumulative_histogram[i] = histogram[i] + cumulative_histogram[i - 1];
		}

//======================================================================================

		double factor = (double)255 / (double)(image.rows * image.cols);

		cout << "Pixels:\t\t " << (double)(image.rows * image.cols) << endl;
		cout << "Mult. Factor:\t " << factor << endl;


		for(int i = 0; i < cumulative_histogram.size(); i++)
		{
			factors[i] = cumulative_histogram[i] * factor;
		}

		for(int y = 0 ; y < image.rows; y++)
		{
			for(int x = 0; x < image.cols; x++)
			{
				image2.at<uchar>(y, x) = factors[image2.at<uchar>(y, x)];
			}
		}

		imshow("Image", image);
		imshow("Equalized_image", image2);

		waitKey(0);
	}catch(exception &ex)
	{
		cout << ex.what() << endl;
	}
}