/*
*	File:	i72lumav.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

//  Core routines:
#include <opencv2/core/core.hpp>
//  Methods:	imread, imshow, namedWindow, waitKey
#include <opencv2/highgui/highgui.hpp>
//	Methods:	COLOR_BGR2GRAY, COLOR_GRAY2BGR
#include <opencv2/tracking.hpp>
#include <iostream>

//	GUI functions: ​ 				https://docs.opencv.org/3.4.7/d7/dfc/group__highgui.html
//	Draw a rectangle:				https://docs.opencv.org/3.4.7/d6/d6e/group__imgproc__draw.html
//	Draw a polygon:					https://docs.opencv.org/3.4.7/d6/d6e/group__imgproc__draw.html#gaf30888828337aa4c6b56782b5dfbd4b7
//		Draw a polygon (example):	https://docs.opencv.org/master/db/d75/samples_2cpp_2create_mask_8cpp-example.html#a17
//	Conver ROI to grayscale:		https://stackoverflow.com/questions/33994930/how-to-convert-the-selected-rectangle-from-the-image-into-grayscale

using namespace std;
using namespace cv;

//	Gloval variables.
Mat image, aux_image, mask, final;
Point point;
vector<Point> pts;
int drag = 0, var = 0, flag = 0;

void mouseHandler(int event, int x, int y, int, void *)
{
	//	Creates the vertex of the polygon using a circle.
	if (event == EVENT_LBUTTONDOWN && !drag)
	{
		if (flag == 0)
		{
			if (var == 0)
			{
				aux_image = image.clone();
			}
			point = Point(x, y);
		//	circle(img, center, radius, color, thickness, lineType = LINE_8, shift = 0);
			circle(aux_image, point, 2, Scalar(0, 0, 255), -1, 8, 0);
			pts.push_back(point);
			var ++;
			drag  = 1;
			//	Due to the fact that var is a global variable, when it is > 1, we create one of the
			//	edges of the polygon. Every pair of point are joined by a line.
			if (var > 1)
			{
			//	line(img, pt1, pt2, color, thickness, lineType = LINE_8, shift = 0);
				line(aux_image, pts[var - 2], point, Scalar(0, 0, 255), 2, 8, 0);
			}
			imshow("Practice_1", aux_image);
		}
	}

	//	Resets the last conditional above.
	if (event == EVENT_LBUTTONUP && drag)
	{
		imshow("Practice_1", aux_image);
		drag = 0;
	}

	//	Joins all the points created and makes a polygon.
	if (event == EVENT_RBUTTONDOWN)
	{
		flag = 1;
		aux_image = image.clone();
		if (var != 0)
		{
		//	polylines(img, pts_array, isClose, color, thickness, lineType = LINE_8, shift = 0);
			polylines(aux_image, pts, 1, Scalar(0, 0 ,0), 2, 8, 0);
		}
		imshow("Practice_1", aux_image);
	}

	//	Pops the new image with the requirements. It is named "Practice_1_Result_Polygon", while the
	//	original one keeps intact, just with the previous selection.
	if (event == EVENT_RBUTTONUP)
	{
		flag = var;
		cvtColor(aux_image, final, COLOR_BGR2GRAY);
		cvtColor(final, final, COLOR_GRAY2BGR);
		mask = Mat::zeros(image.size(), CV_8UC1);
		vector<vector<Point> > vpts;
		vpts.push_back(pts);
		fillPoly(mask, vpts, Scalar(255, 255, 255), 8, 0);
		bitwise_and(image, image, final, mask);
		//imshow("Mask", mask);
		imshow("Practice_1_Result_Polygon", final);
		imshow("Practice_1", aux_image);
	}

	//	Resets the polygon selection.
	if (event == EVENT_MBUTTONDOWN)
	{
		pts.clear();
		var = 0;
		drag = 0;
		flag = 0;
		imshow("Practice_1", image);
	}
}

int main(int argc, char const *argv[])
{
	try
	{
		if(argc != 2)
		{
			cerr << "Usage: image." << endl;
			return 0;
		}

		//  Loads the image from a file.
		image = imread(argv[1]);

		//	Check if the image has pixels.
		if(image.rows == 0)
		{
			cerr << "Error reading image." << endl;
			return 0;
		}

		int figure_type = 0;
		cout << "\tSelect your type of edition:\n\t\t1. Rectangle.\n\t\t2. Polygon.\n\t\t3. Help.\n\t\t4. Exit.\n\t> ";
		cin >> figure_type;

		bool active = true;
		while(active)
		{
			switch(figure_type)
			{
		//===========================================================================================================
				//	Rectangle
				case 1:
				//	In order to protect the variables visible on case 1, we must wrap it (case 1), so that, we prevent "see" them in case 2, case 3, case n and they belong, exclusively, to case 1.
				{
					int n_selections;
					cout << "\tSelections to do: ";
					cin >> n_selections;

					//	Copy the original image to an auxiliar image, so that I keep the original one intact.
					aux_image = image;

					//	In order to avoid an assertion related to the chanels as input (the amount of channels do not match), we must change twice the color.
					//	https://stackoverflow.com/questions/26927126/opencv-error-assertion-failed-channels-cv-mat-cn-dtype
					cvtColor(aux_image, aux_image, COLOR_BGR2GRAY);
					cvtColor(aux_image, aux_image, COLOR_GRAY2BGR);

					//	Vector of rois.
					vector<Rect> rois;

					while(rois.size() < n_selections)
					{
						//	Generate rectangles from the image selected. Each rectangle is saved on the rois vector.
						rois.push_back(selectROI("Practice_1", image));
					}

					//  Create a window which name is given by the argument.
					namedWindow("Practice_1");

					//	This loop place each crop on the grayscaled image.
					for (int i = 0; i < rois.size(); ++i)
					{
						Mat crop = image(rois[i]).clone();      // Crop is color CV_8UC3
						crop.copyTo(aux_image(rois[i]));        // Copy colour image into color grayscale CV_8UC3 image
					}

					imshow("Practice_1_Result_Rectangle", aux_image);
					active = false;
				}
				break;
		//===========================================================================================================
				//	Polygon (Optional)
				case 2:
					cout << "\nLeft mouse button: Set a point to create mask shape.\n"
					"Right mouse button: Create mask from points.\n"
					"Middle mouse button: Reset.\n";

					namedWindow("Practice_1", WINDOW_AUTOSIZE);
					setMouseCallback("Practice_1", mouseHandler, NULL);
					imshow("Practice_1", image);
					active = false;
				break;
		//===========================================================================================================
				//	Help
				case 3:
					cout << "\n[Description]\n---------------\nThis program loads an RGB image as first argument. You can select a region of interest (ROI) with the mouse. When two points in the image are selected, the program will show another window where the inner region is in color while the rest is in greyscale.\n";
					cout << "\n[Usage]\n---------------\nYou can choose 2 different ways to run this program:\n1. Using a rectangle.\n2. Creating a poligon.\n";
					active = false;
				break;
		//===========================================================================================================
				//	Exit
				case 4:
					cout << "\tGoodbye!" << endl;
					return 0;
				break;
		//===========================================================================================================
				default:
					cout << "\tOops... That's not even an option!" << endl;
					cout << "\tSelect one:\n\t\t1. Rectangle.\n\t\t2. Polygon.\n\t\t3. Help.\n\t\t4. Exit.\n\t> ";
					cin >> figure_type;
		//===========================================================================================================    
			}
		}

		//  Wait for a key to be pressed.
		waitKey(0);
	}catch(exception &ex)
	{
		cout << ex.what() << endl;
	}
}