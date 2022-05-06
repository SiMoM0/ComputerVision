//Lab 7 task 1
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "Code/header/panoramic.hpp"
#include "Code/header/panoramic_utils.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    //define path for the images
    string path = "Datasets/dolomites/*.png";
    
    //create object Panoramic
    Panoramic pan (path);
    //print number of images in the dataset
    printf("Number of images: %d\n", pan.getSize());

    //visualize all the images
    pan.visualizeImages();

    //create panoramic image
    int angle = 27;
    Mat panoramic = pan.create(angle, 1.8);

    //show panoramic image
    namedWindow("Panoramic");
    imshow("Panoramic", panoramic);
    waitKey(0);

    return 0;
}