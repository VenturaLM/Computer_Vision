/*
*   File:   whitePatch.hpp
*   Author: Ventura Lucena Martínez
*   Email:  i72lumav@uco.es
*/

#ifndef WHITEPATCH_HPP
#define WHITEPATCH_HPP

using namespace cv;

void whitePatch(const Mat &in, Mat &out);
void whitePatch(const Mat &in, Mat &out, float percentage);
static void on_trackbar(int percentage, void *);

#endif