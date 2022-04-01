//Lab 4 task 1
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

//Global variables
Mat gray, edges;
int low_threshold = 0;
int high_threshold = 0;
const int max_lowThreshold = 1000;
const int max_highThreshold = 1000;
const int gaussian_kernel = 3;
const int kernel_size = 3;
const char* window = "Canny";

//Trackbar callback for the min threshold
static void cannyMinThreshold(int, void*) {
    //blur filter
    blur(gray, edges, Size(gaussian_kernel, gaussian_kernel));
    //canny filter
    Canny(edges, edges, low_threshold, high_threshold, kernel_size);
    //show image
    imshow(window, edges);
}

//Trackbar callback for the max threshold
static void cannyMaxThreshold(int, void*) {
    //blur filter
    blur(gray, edges, Size(gaussian_kernel, gaussian_kernel));
    //canny filter
    Canny(edges, edges, low_threshold, high_threshold, kernel_size);
    //show image
    imshow(window, edges);
}

int main(int argc, char** argv) {
    Mat src = imread(argv[1]);

    //convert to grayscale
    cvtColor(src, gray, COLOR_BGR2GRAY);

    //display image
    namedWindow("Image");
    imshow("Image", src);
    namedWindow(window);
    //trackbars for canny filter
    createTrackbar("Min Threshold: ", window, &low_threshold, max_lowThreshold, cannyMinThreshold);
    createTrackbar("Max Threshold: ", window, &high_threshold, max_highThreshold, cannyMaxThreshold);
    waitKey(0);
    
    return 0;
}