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

#endif //SEGMENTATION