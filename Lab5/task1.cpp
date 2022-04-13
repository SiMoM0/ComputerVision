//Lab 5 task 1
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main(int argc, char** argv) {
    //check input
    if(argc != 2) {
        printf("Invalid sintax");
        return -1;
    }

    Mat src = imread(argv[1]);
    if(src.empty()) {
        printf("Could not open or find image");
        return -1;
    }

    Mat gray, smooth, otsu;

    //convert to grayscale
    cvtColor(src, gray, COLOR_BGR2GRAY);
    //apply blur filter
    blur(gray, smooth, Size(9, 9));

    //Otsu optimal threshold
    threshold(smooth, otsu, 0, 255, THRESH_OTSU);

    //show images
    namedWindow("Image");
    namedWindow("Otsu thresholding");
    imshow("Image", src);
    imshow("Otsu thresholding", otsu);
    waitKey(0);
    
    return 0;
}