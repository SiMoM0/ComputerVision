//Lab 5 task 1
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "segmentation.hpp"

using namespace cv;
using namespace std;

Mat src;

//Mouse callback for region growing
void mouseCallback(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        vector<pair<int, int>>* ppoints = static_cast<vector<pair<int, int>>*>(userdata);
        //Get values of the selected pixel
        Vec3b pixel = src.at<Vec3b>(y, x);
        int blue = (int) pixel[0];
        int green = (int) pixel[1];
        int red = (int) pixel[2];
        //Print BGR color for the selected pixel
        printf("Pixel: (%d, %d), BGR color :(%d, %d, %d)\n", y, x, blue, green, red);
        ppoints->push_back(pair<int, int>(y, x));
    }
}

int main(int argc, char** argv) {
    //check input
    if(argc != 2) {
        printf("Invalid sintax");
        return -1;
    }
    //load input image and check it
    src = imread(argv[1]);
    if(src.empty()) {
        printf("Could not open or find image");
        return -1;
    }
    
    //apply otsu threshold
    Mat otsu (src.rows, src.cols, CV_8UC3);
    otsuSegmentation(src, otsu, 7);

    vector<pair<int, int>> points;

    //show images
    namedWindow("Image");
    setMouseCallback("Image", mouseCallback, &points); // Register the callback function
    imshow("Image", src);
    namedWindow("Otsu thresholding");
    imshow("Otsu thresholding", otsu);
    waitKey(0);


    //apply regionGrowing and show image
    Mat region = regionGrowing(src, points, 20);
    namedWindow("Region Growing");
    imshow("Region Growing", region);
    waitKey(0);
    
    return 0;
}