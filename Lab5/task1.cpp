//Lab 5 task 1
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
    
    //apply otsu threshold
    Mat otsu (src.rows, src.cols, CV_8UC3);
    otsuSegmentation(src, otsu, 9);
    //save otsu image with the name otsu_{asphalt number}.png in Images folder
    //string filename_otsu = "Images/otsu_" + string(argv[1]).substr(10);
    //imwrite(filename_otsu, otsu);

    //show images
    namedWindow("Image");
    imshow("Image", src);
    namedWindow("Otsu thresholding");
    imshow("Otsu thresholding", otsu);
    waitKey(0);

    //apply regionGrowing
    Mat region (src.rows, src.cols, CV_8UC1);
    regionGrowing(src, region, 5, 10);
    //save region growing image with the name rgrow_{asphalt number}.png in Images folder
    //string filename_rgrow = "Images/rgrow_" + string(argv[1]).substr(10);
    //imwrite(filename_rgrow, region);
    
    //show images
    namedWindow("Region Growing");
    imshow("Region Growing", region);
    waitKey(0);

    //apply watershed (doesn't seems to work properly)
    Mat wshed (src.rows, src.cols, CV_8UC3);
    watershedSegmentation(src, wshed);
    
    //show image
    namedWindow("Watershed");
    imshow("Watershed", wshed);
    waitKey(0);
    
    return 0;
}