/*
*	File:	applyKernel.hpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#ifndef APPLYKERNEL_H
#define APPLYKERNEL_H

void medianBlur_grayscale(const cv::Mat &in, cv::Mat &out, const cv::Mat &kernel);
void medianBlur_colour(const cv::Mat &in, cv::Mat &out, const cv::Mat &kernel);

void applyKernel_grayscale(const cv::Mat &in, cv::Mat &out, const cv::Mat &kernel);
void applyKernel_colour(const cv::Mat &in, cv::Mat &out, const cv::Mat &kernel);

void erode_grayscale(const cv::Mat &in, cv::Mat &out, const cv::Mat &kernel);
void dilation_grayscale(const cv::Mat &in, cv::Mat &out, const cv::Mat &kernel);

#endif