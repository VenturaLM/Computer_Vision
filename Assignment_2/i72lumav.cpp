/*
*	File:	i72lumav.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

//	Add a trackbar:	https://docs.opencv.org/3.4/da/d6a/tutorial_trackbar.html
//					https://www.youtube.com/watch?v=fM6ff3VEviI

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "whitePatch.hpp"
#include "cmdLineParser.hpp"

using namespace std;
using namespace cv;

Mat image, result;

const int alpha_slider_max = 100;
int percentage_slider = 0;

static void on_trackbar(int percentage, void *)
{
	float p = (float)percentage / (float)100;
	whitePatch(image, result, p);
	imshow("Image", result);
}

int main(int argc, char *argv[])
{
	CmdLineParser cml(argc, argv);
	if(cml["-p"])
	{
		float f = stof(cml("-p"));
		//int i = stoi(cml("-p"));
	}

	float f = stof(cml("-p","1"));

	try
	{
		image = imread(argv[1], CV_32FC1);

		if(image.rows == 0)
		{
			cerr << "Error reading image." << endl;
			return 0;
		}

		int option = 0;
		cout << "\n\tSelect your option:\n\t\t1. Single White-Patch.\n\t\t2. Percetage White-Patch.\n\t\t3. Interactive mode.\n\t\t4. Help.\n\t\t5. Exit.\n\t> ";
		cin >> option;

		bool active = true;
		while(active)
		{
			switch(option)
			{
		//===========================================================================================================
				//	Single
				case 1:
					whitePatch(image, result);

					imshow("Image", image);
					imshow("Result", result);
					
					active = false;
				break;
		//===========================================================================================================
				//	Percentage
				case 2:
					if(f > 1)
						f = 1;

					if(f < 0)
						f = 0;

					if(cml["-p"])
					{
						f = stof(cml("-p"));
						whitePatch(image, result, f);

						imshow("Image", image);
						imshow("Result", result);
					}else{
						cout << "./executable <in-image> [-p <float>]" << endl;
						exit(-1);
					}
					active = false;
				break;
		//===========================================================================================================
				//	Interactive mode
				case 3:
					if(cml["-i"])
					{
						namedWindow("Image", WINDOW_AUTOSIZE);
						//	on_trackbar: Every time the trackbar value changes, on_trackbar() is called.
						createTrackbar("Brightness", "Image", &percentage_slider, alpha_slider_max, on_trackbar);
					}else{
						cout << "./executable <in-image> [-i]" << endl;
						exit(-1);
					}
					active = false;
				break;
		//===========================================================================================================
				//	Help
				case 4:
					cout << "\n[Description]\n---------------\nThis program corrects color deficiencies in illumination that makes white light as a different colour The program finds the brightest image pixel and calculate the transform that makes it white. The rest of the image is also re-scaled according to the transformation.\n";
					cout << "\nAlso, you can use the second option to correct the colours using the average of the % brightest colours." << endl;
					cout << "\n[Usage]\n---------------\n1. ./executable <in-image>\n2. ./executable <in-image> [-p <float>]\n3. ./executable <in-image> [-i]\n";
					active = false;
				break;                
		//===========================================================================================================
				//	Exit
				case 5:
					cout << "\tGoodbye!" << endl;
					return 0;
				break;
		//===========================================================================================================
				default:
					cout << "\tOops... That's not even an option!" << endl;
					cout << "\tSelect one:\n\t\t1. Single White-Patch.\n\t\t2. Percetage White-Patch.\n\t\t3. Interactive mode.\n\t\t4. Help.\n\t\t5. Exit.\n\t> ";
					cin >> option;
		//===========================================================================================================    
			}
		}

		waitKey(0);
	}catch(exception &ex)
	{
		cerr << ex.what() << endl;
	}
	return 0;
}