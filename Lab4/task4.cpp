//Lab 4 task 4
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat src = imread(argv[1]);
    Mat gray, smooth, hough;

    //convert to grayscale
    cvtColor(src, gray, COLOR_BGR2GRAY);
    //apply median blur
    medianBlur(gray, smooth, 5);
    //apply canny detector
    Canny(smooth, smooth, 200, 240, 3);
    //clone original to hough
    hough = src.clone();

    //Hough circle transform
    vector<Vec3f> circles;
    //set the two parameters and the radius between 6 and 8
    HoughCircles(smooth, circles, HOUGH_GRADIENT, 2, smooth.rows/2, 60, 30, 6, 8);
    //draw circles
    for(size_t i=0; i<circles.size(); ++i) {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        //circle center
        circle(hough, center, 1, Scalar(0, 255, 0), 1, LINE_AA);
        //circle outline
        int radius = c[2];
        circle(hough, center, radius, Scalar(0, 255, 0), FILLED, LINE_AA);
    }

    //display images
    namedWindow("Image");
    namedWindow("Hough Circles");
    imshow("Image", src);
    imshow("Hough Circles", hough);
    waitKey(0);

    return 0;
}