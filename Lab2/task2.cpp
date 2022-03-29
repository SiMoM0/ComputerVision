//Lab 2 task 2
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filters.hpp"

using namespace cv;

int main(int argc, char** argv) {
    Mat img = imread(argv[1]);
    cvtColor(img, img, COLOR_BGR2GRAY);

    //apply filters
    int size = 7;
    Mat min_img = minFilter(img, size);
    Mat max_img = maxFilter(img, size);
    //imwrite("min_filter.jpg", min_img);
    //imwrite("max_filter.jpg", max_img);

    //display images
    namedWindow("Min Filter");
    imshow("Min Filter", min_img);
    namedWindow("Max Filter");
    imshow("Max Filter", max_img);
    waitKey(0);

    return 0;
}

//CONCLUSION
//With the max filter of size 7x7 the electric cables in the background seems to be removed without corrupting too much the image