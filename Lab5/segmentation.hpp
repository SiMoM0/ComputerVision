//Header for different types of segmentation techniques
#ifndef SEGMENTATION
#define SEGMENTATION

#include <opencv2/highgui.hpp>

/**
 * Otsu's optimal threshold method for segmentation.
 * @param input input color image of type cv::Mat
 * @param output output image of type cv::Mat
 * @param ksize kernel size of the blur filter
 */
void otsuSegmentation(const cv::Mat& input, cv::Mat& output, const int ksize);

/**
 * Region growing algorithm for segmentation.
 * @param input input color image of type cv::Mat
 * @param mask output mask for segmentation of type cv::Mat
 * @param ksize kernel size of the blur filter
 * @param similarity distance criteria between neighbour points, ucha value
 */
void regionGrowing(const cv::Mat& input, cv::Mat& mask, const int ksize, uchar similarity);

/**
 * Watershed algorithm.
 * @param input input color image of type cv::Mat
 * @param output output image of type cv::Mat
 */
void watershedSegmentation(const cv::Mat& input, cv::Mat& output);

/**
 * K-Means algorithm for segmentation.
 * @param input input color image of type cv::Mat
 * @param output output image of type cv::Mat
 * @param k number of clusters
 * @param color true if executing task3, default false
 */
void kmeansSegmentation(const cv::Mat& input, cv::Mat& output, const int k, const bool color=false);

#endif //SEGMENTATION