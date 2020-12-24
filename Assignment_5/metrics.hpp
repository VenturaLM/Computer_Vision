#ifndef FSIV_METRICS_HPP
#define FSIV_METRICS_HPP

#include <vector>
#include <opencv2/core.hpp>


cv::Mat compute_confusion_matrix(const cv::Mat& true_labels,
                         const cv::Mat& predicted_labels);

float compute_recognition_rate(const cv::Mat& cmat, int category);

float compute_mean_recognition_rate(const cv::Mat& cmat,
                                    std::vector<int> const& categories=std::vector<int>());
float compute_accuracy(const cv::Mat& cmat,
                       std::vector<int> const& categories=std::vector<int>());

/**
 * @brief Compute chi2 distance between two histograms.
 * Note: Code from scratch. Do not use cv::compareHist().
 * @param[in] h1 the first descriptor.
 * @param[in] h2 the second descriptor.
 * @return the chi2 distance.
 * @pre h1.rows==1 and h2.rows==1
 * @pre h1.cols == h2.cols
 * @posrt !isnan(ret_v) && (ret_v >= 0.0)
 */
float fsiv_chisquared_dist(const cv::Mat & h1, const cv::Mat & h2);

#endif // FSIV_METRICS_HPP
