#include "common_code.hpp"
#include "lbp.hpp"

using namespace std;
using namespace cv::ml;

static uchar uniform[256] =
{
0,1,2,3,4,58,5,6,7,58,58,58,8,58,9,10,11,58,58,58,58,58,58,58,12,58,58,58,13,58,
14,15,16,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,17,58,58,58,58,58,58,58,18,
58,58,58,19,58,20,21,22,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,
58,58,58,58,58,58,58,58,58,58,58,58,23,58,58,58,58,58,58,58,58,58,58,58,58,58,
58,58,24,58,58,58,58,58,58,58,25,58,58,58,26,58,27,28,29,30,58,31,58,58,58,32,58,
58,58,58,58,58,58,33,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,34,58,58,58,58,
58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,
58,35,36,37,58,38,58,58,58,39,58,58,58,58,58,58,58,40,58,58,58,58,58,58,58,58,58,
58,58,58,58,58,58,41,42,43,58,44,58,58,58,45,58,58,58,58,58,58,58,46,47,48,58,49,
58,58,58,50,51,52,58,53,54,55,56,57
}; /**< Look-up table to convert 256 lbp codes to 59 u-lbp codes. **/

void 
fsiv_lbp(const cv::Mat & img, cv::Mat & lbp, const bool uLBP)
{    
	assert(!img.empty() && img.type()==CV_32FC1);
	lbp = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);

	//TODO: compute the lbp descriptor.
	//Hint: first compute the 256 lbp code, then if the uLBP is required,
	//use the uniform lookup table to get the correspoing uLBP code.
	//Remember: the codes for the points that not can be computed will be zeros.

	//	Reference:	https://www.bytefish.de/blog/local_binary_patterns.html
	for (auto i = 1; i < img.rows - 1; i++)
	{
		for (auto j = 1; j < img.cols - 1; j++)
		{
			float center = img.at<float>(i, j);
			uchar code = 0;

			code |= ( img.at<float>(i, j - 1) > center ) << 7;
			code |= ( img.at<float>(i + 1, j - 1) > center ) << 6;
			code |= ( img.at<float>(i + 1, j) > center ) << 5;
			code |= ( img.at<float>(i + 1, j + 1) > center ) << 4;
			code |= ( img.at<float>(i, j + 1) > center ) << 3;
			code |= ( img.at<float>(i - 1, j + 1) > center ) << 2;
			code |= ( img.at<float>(i - 1, j) > center ) << 1;
			code |= ( img.at<float>(i - 1, j - 1) > center ) << 0;

			lbp.at<uchar>(i, j - 1) = code;
		}
	}

	//	Reference:	https://answers.opencv.org/question/33988/does-any-one-has-a-uniform-lbp-c-code-with-its-spatial-histogram/
	//				https://docs.opencv.org/4.5.0/d2/de8/group__core__array.html#gab55b8d062b7f5587720ede032d34156f
	if(uLBP)
	{
		cv::Mat ulbp;
		cv::Mat uniform_table(1, 256, CV_8UC1, uniform);

		cv::LUT(lbp, uniform_table, ulbp);

		lbp = ulbp.clone();
	}

	//
	assert(lbp.type()==CV_8UC1);
	assert(lbp.rows==img.rows && lbp.cols==img.cols);
#ifndef NDEBUG
	double max_code, min_code;
	cv::minMaxLoc(lbp, &min_code, &max_code, nullptr, nullptr);
	assert(!uLBP || ((min_code>=0.0) && (max_code<59)));
	assert(uLBP  || ((min_code>=0.0) && (max_code<256)));
#endif
}

void
fsiv_lbp_hist(const cv::Mat & lbp, cv::Mat & lbp_hist,
			  const bool normalize, const bool uLBP)
{	    
	assert(!lbp.empty() && lbp.type()==CV_8UC1);

	// Establishing the number of bins
	int histSize = uLBP ? 59 : 256;

	//TODO.
	//Hint: use "cv::calcHist" if you want, but remember we want row vectors so
	//the histogram must be reshaped. You can reshape a cv::Mat using the
	//cv::Mat::reshape() method.
	//
	//Also remember to normalize the histogram if it is required (sum(h)==1.0).

	float hranges[] = {0, 256};
	const float* phranges = hranges;

	//	Reference:	https://docs.opencv.org/4.5.0/d6/dc7/group__imgproc__hist.html#ga4b2b5fd75503ff9e6844cc4dcdaed35d
	//				https://docs.opencv.org/4.5.0/d6/d7f/samples_2cpp_2camshiftdemo_8cpp-example.html#a40
	calcHist(&lbp, 1, 0, cv::Mat(), lbp_hist, 1, &histSize, &phranges, true, false);
   
	//	Reference:	https://docs.opencv.org/4.5.0/d2/de8/group__core__array.html#ga46630ed6c0ea6254a35f447289bd7404
	cv::transpose(lbp_hist, lbp_hist);

	if (normalize)
	{
		//	Reference:	https://docs.opencv.org/4.5.0/d2/de8/group__core__array.html#ga7bcf47a1df78cf575162e0aed44960cb
		cv::normalize(lbp_hist, lbp_hist, 1.0, 0.0, cv::NORM_L1);
	}

	lbp_hist.convertTo(lbp_hist, CV_32FC1);
	//
	assert(lbp_hist.type()==CV_32FC1);
	assert(lbp_hist.rows==1 && (uLBP ? (lbp_hist.cols==59) : (lbp_hist.cols==256)));
	assert(!normalize || (std::abs(cv::sum(lbp_hist)[0]-1.0)<1.0e6));
}

void
fsiv_lbp_desc(const cv::Mat & lbp, cv::Mat & lbp_desc,
			  const int *ncells, bool normalize, bool uLBP)
{
	assert(!lbp.empty() && lbp.type()==CV_8UC1);
	assert(ncells!=nullptr && (ncells[0]*ncells[1] > 0));

	const int cell_h = cvFloor(double(lbp.rows) / ncells[0]);
	const int cell_w = cvFloor(double(lbp.cols) / ncells[1]);
	const int hist_size = uLBP ? 59 : 256;

	//TODO: Use fsiv_lbp_hist() for each cell and concatenate the histograms
	// to get the image descriptor.
	//Hint: you can use cv::hconcat() or you can create an initial descriptor
	//with enough space and use cv::Mat::copyTo on it using a roi.

	//	Reference:	https://answers.opencv.org/question/173852/how-to-split-image-into-small-blocks-process-on-them-and-then-join-all-the-blocks-together-again/
	std::vector<cv::Mat> my_nCells;
	std::vector<cv::Mat> histograms(ncells[0] * ncells[1]);

	for (auto i = 0; i <= lbp.rows - cell_h; i += cell_h)
	{
		for (auto j = 0; j <= lbp.cols - cell_w; j += cell_w)
			my_nCells.push_back( lbp(cv::Rect(j, i, cell_w, cell_h)).clone() );
	}

	for (auto i = 0; i < my_nCells.size(); i++)
		fsiv_lbp_hist(my_nCells[i], histograms[i], normalize, uLBP);

	//	Reference:	https://docs.opencv.org/4.5.0/d2/de8/group__core__array.html#gaf9771c991763233866bf76b5b5d1776f
	cv::hconcat(histograms, lbp_desc);
	//
	assert(!lbp_desc.empty());
	assert(lbp_desc.type()==CV_32FC1);
	assert(lbp_desc.rows==1); // !!We want row vectors //
	assert(uLBP || lbp_desc.cols==(256*ncells[0]*ncells[1]));
	assert(!uLBP || lbp_desc.cols==(59*ncells[0]*ncells[1]));
}

void 
fsivlbp_disp(const cv::Mat &lbpmat, const std::string &winname)
{
	imshow(winname, lbpmat);
	cv::waitKey(0);
}