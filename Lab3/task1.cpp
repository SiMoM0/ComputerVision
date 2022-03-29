//Lab 3 task 1
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int argc, char** argv) {
    //Load image
    Mat src = imread(argv[1]);
    //Show image
    namedWindow("Image");
    imshow("Image", src);
    waitKey(0);

    return 0;
}