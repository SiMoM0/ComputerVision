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
 * @param seed seed points of type vector<int, int>
 * @param similarity distance criteria between neighbour points, ucha value
 * @return mask result for segmentation
 */
cv::Mat regionGrowing(const cv::Mat& input, const std::vector<std::pair<int, int>>& seed, uchar similarity);

#endif //SEGMENTATION