//Implementation of the functions in the header 'filters.hpp'
#include "filters.hpp"
#include <iostream>

//minFilter function

cv::Mat minFilter(cv::Mat image, int size) {
    cv::Mat min_img (image.rows-size+1, image.cols-size+1, CV_8U);
    double min;
    double max;
    //check if the size is odd or even
    if(size%2 == 0) {
        return image;
    } else {
        std::cout << "Applying the min filter\n";
        for(int i=0; i<min_img.rows-size+1; ++i) {
            for(int j=0; j<min_img.cols-size+1; ++j) {
                //get the submatrix of size*size
                cv::Mat submatrix = image(cv::Range(i, i+size-1), cv::Range(j, j+size-1));
                //get the minimum value in the submatrix
                cv::minMaxLoc(submatrix, &min, &max);
                int m = (int) min;
                //apply the value to the new image
                min_img.at<unsigned char>(i, j) = (unsigned char) m;
            }
        }
    }
    
    return min_img;
}

//maxFilter function

cv::Mat maxFilter(cv::Mat image, int size) {
    cv::Mat max_img (image.rows-size+1, image.cols-size+1, CV_8U);
    double min;
    double max;
    //check if the size is odd or even
    if(size%2 == 0) {
        return image;
    } else {
        std::cout << "Applying the max filter\n";
        for(int i=0; i<max_img.rows-size+1; ++i) {
            for(int j=0; j<max_img.cols-size+1; ++j) {
                cv::Mat submatrix = image(cv::Range(i, i+size-1), cv::Range(j, j+size-1));
                cv::minMaxLoc(submatrix, &min, &max);
                int m = (int) max;
                max_img.at<unsigned char>(i, j) = (unsigned char) m;
            }
        }
    }
    
    return max_img;
}