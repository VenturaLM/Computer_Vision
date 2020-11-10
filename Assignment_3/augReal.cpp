/*
*	File:	augReal.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*
*	References:
*		- rvec:						https://docs.opencv.org/4.5.0/dd/d99/classcv_1_1Affine3.html#a2b55e155790f606db5eb913bd350645b
*		- Size:						https://docs.opencv.org/3.4/d6/d50/classcv_1_1Size__.html
*		- FileStorage:				https://docs.opencv.org/4.5.0/da/d56/classcv_1_1FileStorage.html#a29bcb12ede10d87a0fcbf89a88b092af
*		- TermCriteria:				https://docs.opencv.org/4.5.0/d9/d5d/classcv_1_1TermCriteria.html#ae3130f54d419687a87349da94df1226b
*									https://stackoverflow.com/questions/18955760/how-does-cvtermcriteria-work-in-opencv
*		- findChessboardCorners:	https://docs.opencv.org/4.5.0/d9/d0c/group__calib3d.html#ga93efa9b0aa890de240ca32b11253dd4a
*		- cornerSubPix:				https://docs.opencv.org/4.5.0/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
*		- solvePnP:					https://docs.opencv.org/4.5.0/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d
*		- line:						https://docs.opencv.org/4.5.0/d6/d6e/group__imgproc__draw.html#ga7078a9fae8c7e7d13d24dac2520ae4a2
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types_c.h>

#include "cmdLineParser.hpp"
#include "overlay.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	CmdLineParser cml(argc, argv);

	bool arg_flag = false;
	if(argc < 11)
	{
		arg_flag = true;
	}

	//	- Options:
	if(cml["-h"] or cml["-help"] or arg_flag == true)
	{
		cout 	<< "1. -help -h\t\tprints this message.\n"
				<< "2. -rows -r \t\tnumber of rows of the chessboard.\n"
				<< "3. -cols -c \t\tnumber of columns of the chessboard.\n"
				<< "4. -size -s\t\tsquare size.\n"
				<< "5. -yaml -y\t\t.yml file obtained by calibration.\n"
				<< "6. -video -v\t\tchessboard video file.\n"
				<< "7. -image -i\t\timage/video file." << endl;
		return 0;
	}
	
	int rows, cols;
	double size;
	string video_name, yaml_file_name;

	if(cml["-r"])
		rows = stoi(cml("-r"));
	else if(cml["-rows"])
		rows = stoi(cml("-rows"));

	if(cml["-c"])
		cols = stoi(cml("-c"));
	else if(cml["-cols"])
		cols = stoi(cml("-cols"));

	if(cml["-s"])
		size = stod(cml("-s"));
	else if(cml["-size"])
		size = stod(cml("-size"));

	if(cml["-y"])
		yaml_file_name = cml("-y");
	else if(cml["-yaml"])
		yaml_file_name = cml("-yaml");

	if(cml["-v"])
		video_name = cml("-v");
	else if(cml["-video"])
		video_name = cml("-video");

	//	- Initialize videocapture.
	VideoCapture cap_input;
	VideoCapture cap(video_name);
	bool video_flag = false;

	Mat input_image;
	if(cml["-i"] or cml["-image"])
	{
		string input;
		if(cml["-i"])
			input = cml("-i");
		else if(cml["-image"])
			input = cml("-image");

		video_flag = false;

		input_image = imread(input, CV_32F);
		if(!input_image.data)
		{
			cap_input.open(input);
			video_flag = true;

			if(!cap_input.isOpened())
				return -1;
		}
	}

	vector<Point2f> centers;
	vector<Point3f> points3D;
	vector<Point3f> axis_points;

	//	- Rodrigues vector - A vector representing the upper left 3x3 rotation matrix of the current matrix.
	Mat rvec(3, 1, CV_32F);
	Mat tvec(3, 1, CV_32F);
	//	- Camera matrix coefficients from calibration.
	Mat cameraMatrix(3, 3, CV_32F);
	//	- Distorsion coefficients from calibration.
	Mat distCoeffs(5, 1, CV_32F);

	Mat frame, gray_frame, input_frame;
	Size colsxrows(cols, rows);

	FileStorage fs(yaml_file_name, FileStorage::READ);

	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeffs;

	//	- Size: Template class for specifying the size of an image or rectangle.
	Size winSize = Size(2 * cols + 1, 2 * rows + 1);
	Size zeroZone = Size(-1, -1);
	//	- https://stackoverflow.com/questions/18955760/how-does-cvtermcriteria-work-in-opencv
	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, size);

	if(!cap.isOpened())
		return -1;

	for(auto i = 0; i < rows; i++)
	{
		for(auto j = 0; j < cols; j++)
		{
			points3D.push_back(Point3f(j, i, 0));
		}
	}

	axis_points.push_back(Point3f(0, 0, 0));
	axis_points.push_back(Point3f(size, 0, 0));
	axis_points.push_back(Point3f(0, size, 0));
	axis_points.push_back(Point3f(0, 0, -size));

	for(;;)
	{
		cap >> frame;

		if(!cap.read(frame))
			break;

		cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
		//	- Finds the position of the internal corners of the chessboard.
		bool patternfound = findChessboardCorners(gray_frame, colsxrows, centers);

		//	- Mandatory:
		if(patternfound and !cml["-i"] and !cml["-image"])
		{ 	
			//	Refines the corner locations.
			cornerSubPix(gray_frame, centers, winSize, zeroZone, criteria);
			solvePnP(points3D, centers, cameraMatrix, distCoeffs, rvec, tvec);
			projectPoints(axis_points, rvec, tvec, cameraMatrix, distCoeffs, centers);

			line(frame, centers[0], centers[1], Scalar(0, 0, 255), 6);
			line(frame, centers[0], centers[2], Scalar(0, 255, 0), 6);
			line(frame, centers[0], centers[3], Scalar(255, 0, 0), 6);

			imshow("Video", frame);		
		}

		//	- Optional:
		if(cml["-i"] or cml["-image"])
		{
			if(!video_flag)
				overlay(input_image, centers,frame);

			else
			{
				cap_input >> input_frame;
				if(!cap_input.read(input_frame))
					break;

				overlay(input_frame, centers, frame);
			}
		}
		if(waitKey(30) >= 0) 
			break;
	}
	return 0;
}