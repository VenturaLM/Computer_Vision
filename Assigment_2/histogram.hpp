/*
*   File:   histogram.hpp
*   Author: Ventura Lucena Martínez
*   Email:  i72lumav@uco.es
*/

#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP
#include <vector>

using namespace std;
using namespace cv;

void histogram(const Mat &in, vector<int> &histogram);
void cumulativeHistogram(const Mat &in, vector<int> &histogram, vector<int> &cumulative_histogram);
void applyHistogram(const Mat &in, Mat &out, vector<int> &cumulative_histogram, vector<int> &factors, double &factor);
void printHistogram(const vector<int> &histogram);

#endif