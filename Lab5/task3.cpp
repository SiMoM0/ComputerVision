//Lab 5 task 3
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

    //show images
    namedWindow("Image");
    imshow("Image", src);
    waitKey(0);

    //apply otsu threshold
    //Mat otsu (src.rows, src.cols, CV_8UC3);
    //otsuSegmentation(src, otsu, 5);
    //namedWindow("Otsu");
    //imshow("Otsu", otsu);
    //waitKey(0);

    //apply region growing on the t-shirts
    //Mat region;
    //regionGrowing(src, region, 5, 10);
    //namedWindow("Region Growing");
    //imshow("Region Growing", region);
    //waitKey(0);

    //apply watershed segmentation
    //Mat water (src.rows, src.cols, CV_8UC3);
    //watershedSegmentation(src, water);
    //namedWindow("Watershed");
    //imshow("Watershed", water);
    //waitKey(0);

    //apply kmeans segmentation with 5 cluster to get the t-shirts clusters
    Mat tshirt;
    kmeansSegmentation(src, tshirt, 5, true);
    //save image
    //imwrite("Images/tshirts.png", tshirt);

    namedWindow("Segmentation");
    imshow("Segmentation", tshirt);
    waitKey(0);
    
    return 0;
}