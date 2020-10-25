/*
*	File:	morphological_operations.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

/*	References:
*
*/

/*	Task:
*	Create a program that reads an input image, threshold the image, and then applies the morphological operations: 
*	erode, dilate, open, close; saving the result into a file.
*/

/*	Arguments:
*		./prog in_image out_image -thres <val> -op <(erode|dilate|open|close)>
*			-thres <val> : threshold value. The image will be thresholded using this value. pixels above this value will be set to 255, the others to 0.
*			-op <string>:  morphological operation to be applied.
*/

/*	Execution:
*		./morphological_operations ../lplate.jpg ../erode.jpg -thres 150 -op erode
*		./morphological_operations ../lplate.jpg ../dilate.jpg -thres 150 -op dilate
*		./morphological_operations ../lplate.jpg ../open.jpg -thres 150 -op open
*		./morphological_operations ../lplate.jpg ../close.jpg -thres 150 -op close
*/

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "cmdLineParser.hpp"
#include "applyKernel.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	if(argc != 7)
	{
		cout << "Wrong input" << endl;
		cout << "Example:\n\t./executable in_image out_image -thres <val> -op <(erode|dilate|open|close)>" << endl;
		exit(EXIT_FAILURE);
	}

	CmdLineParser cml(argc, argv);
	if(cml["-thres"])
		float threshold = stof(cml("-thres"));

	if(cml["-op"])
		string operation = cml("-op");

	float threshold = stof(cml("-thres","255"));
	string operation = cml("-op", "erode");

	try
	{
		Mat image = imread(argv[1], IMREAD_GRAYSCALE);
		if(image.empty())
		{
			cout << "Error reading the image." << endl;
			exit(-1);
		}

		cout << "\n\tSelect kernel size:\n\t> ";
		int size;
		cin >> size;
		Mat kernel = Mat::ones(size, size, CV_32FC1);


		for(int y = 0 ; y < image.rows; y++)
		{
			for(int x = 0; x < image.cols; x++)
			{
				auto value = image.at<uchar> (y, x);
				if(value >= threshold)
					image.at<uchar> (y, x) = 255;
				else
					image.at<uchar> (y, x) = 0;
			}
		}

		Mat result = image.clone();

		if(operation == "erode")
			erode_grayscale(image, result, kernel);

		if(operation == "dilate")
			dilation_grayscale(image, result, kernel);

		if(operation == "open")
		{
			Mat erode = image.clone();
			erode_grayscale(image, erode, kernel);
			dilation_grayscale(erode, result, kernel);
		}

		if(operation == "close")
		{
			Mat dilation = image.clone();
			dilation_grayscale(image, dilation, kernel);
			erode_grayscale(dilation, result, kernel);
		}

		imwrite(argv[2], result);
	}catch(exception &ex)
	{
		cerr << ex.what() << endl;
	}
	return 0;
}