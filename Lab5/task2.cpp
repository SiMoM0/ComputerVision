//Lab 5 task 2
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "segmentation.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    //check input
    if(argc != 2) {
        printf("Invalid sintax");
        return -1;
    }
    //load input image and check it
    Mat src = imread(argv[1]);
    if(src.empty()) {
        printf("Could not open or find image");
        return -1;
    }

    //Apply watershed
    //Mat wshed (src.rows, src.cols, CV_8UC3);
    //watershedSegmentation(src, wshed);

    //Apply kmeans
    Mat clusters;
    kmeansSegmentation(src, clusters, 3);
    //save image
    //imwrite("Images/street_segmented.png", clusters);

    //show image
    namedWindow("Image");
    namedWindow("Cluster Segmentation");
    imshow("Image", src);
    imshow("Cluster Segmentation", clusters);
    waitKey(0);

    return 0;
}