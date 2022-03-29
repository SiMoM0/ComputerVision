//Header file for filters used in this lab
#include <opencv2/highgui.hpp>

/** @brief Apply min filter function of a selected size
 * @param image the input image, a cv::Mat object
 * @param size integer representing the size of the kernel
 * @return modified image, a cv::Mat object
 */
cv::Mat minFilter(cv::Mat image, int size);

/** @brief Apply max filter function of a selected size
 * @param image the input image, a cv::Mat object
 * @param size integer representing the size of the kernel
 * @return modified image, a cv::Mat object
 */
cv::Mat maxFilter(cv::Mat image, int size);