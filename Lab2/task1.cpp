//Lab 2 task 1
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main(int argc, char** argv) {
    Mat img = imread(argv[1]);
    Mat gray;

    //convert to grayscale and save
    cvtColor(img, gray, COLOR_BGR2GRAY);
    imwrite("image_grayscale.jpg", gray);

    //display images
    namedWindow("Color Image");
    imshow("Color Image", img);
    namedWindow("Grayscale Image");
    imshow("Grayscale Image", gray);
    waitKey(0);

    return 0;
}