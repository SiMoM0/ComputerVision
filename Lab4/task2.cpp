//Lab 4 task 2
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat src = imread(argv[1]);
    //input image controls
    if(src.empty()) {
        printf("Could not open or find image");
        return -1;
    }

    Mat gray, smooth, sobel1, sobel2, result;

    //convert to grayscale
    cvtColor(src, gray, COLOR_BGR2GRAY);
    //smooth image using blur filter
    blur(gray, smooth, Size(5, 5));

    //sobel for 45 degree angles
    Mat kernel1 = (Mat_<float>(3, 3) <<
                -2, -1, 0,
                -1, 0, 1,
                0, 1, 2);

    //sobel for -45 degree angles
    Mat kernel2 = (Mat_<float>(3, 3) <<
                0, 1, 2,
                -1, 0, 1,
                -2, -1, 0);

    //apply kernels to smooth image and then threshold
    filter2D(smooth, sobel1, CV_32F, kernel1);
    threshold(sobel1, sobel1, 170, 255, THRESH_BINARY);
    filter2D(smooth, sobel2, CV_32F, kernel2);
    threshold(sobel2, sobel2, 170, 255, THRESH_BINARY);

    //sum the two images obtained with sobel (not great result compared to canny edge detector)
    result = sobel1 + sobel2;

    //show images
    namedWindow("Image");
    namedWindow("Edges");
    imshow("Image", src);
    imshow("Edges", result);
    waitKey(0);
    
    return 0;
}