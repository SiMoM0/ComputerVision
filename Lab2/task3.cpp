//Lab 2 task 3
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main(int argc, char** argv) {
    Mat img = imread(argv[1]);
    cvtColor(img, img, COLOR_BGR2GRAY);
    
    Mat median, gaussian;
    int size = 7;
    int sigma = 5;
    
    //apply median filter
    medianBlur(img, median, size);
    //apply median filter
    GaussianBlur(img, gaussian, Size(size, size), sigma, sigma);

    //imwrite("median_blur.jpg", median);
    //imwrite("gaussian_blur.jpg", gaussian);

    //display image
    namedWindow("Median Filter");
    imshow("Median Filter", median);
    namedWindow("Gaussian Blur");
    imshow("Gaussian Blur", gaussian);
    waitKey(0);

    return 0;
}