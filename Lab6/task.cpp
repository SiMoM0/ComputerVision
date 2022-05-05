//Lab 6 task 1
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    //check inputs
    if(argc != 3) {
        printf("Invalid sintax");
        return -1;
    }
    //load the two input images and check them
    Mat src1 = imread(argv[1]);
    Mat src2 = imread(argv[2]);
    if(src1.empty() || src2.empty()) {
        printf("Could not open or find image");
        return -1;
    }

    //show input images
    namedWindow("Image 1");
    namedWindow("Image 2");
    imshow("Image 1", src1);
    imshow("Image 2", src2);
    waitKey(0);

    //detect keypoints using SIFT detector
    int min = 500;
    Ptr<SIFT> detector = SIFT::create(min);
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptor1, descriptor2;
    detector->detectAndCompute(src1, noArray(), keypoints1, descriptor1);
    detector->detectAndCompute(src2, noArray(), keypoints2, descriptor2);
    
    //draw keypoints
    Mat img1, img2;
    drawKeypoints(src1, keypoints1, img1, Scalar(0, 255, 0));
    drawKeypoints(src2, keypoints2, img2, Scalar(0, 255, 0));

    //show images with features
    imshow("Image 1", img1);
    imshow("Image 2", img2);
    waitKey(0);

    //matching descriptor vectors using FLANN
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    vector<vector<DMatch>> matches;
    matcher->knnMatch(descriptor1, descriptor2, matches, 2);
    
    //filter matches using Lowe ratio test
    const float thresh = 0.7f;
    vector<DMatch> good_matches;
    for(int i=0; i<matches.size(); ++i) {
        //check ratio
        if(matches[i][0].distance < thresh * matches[i][1].distance) {
            good_matches.push_back(matches[i][0]);
        }
    }
    
    //draw matches
    Mat img_matches;
    drawMatches(src1, keypoints1, src2, keypoints2, good_matches, img_matches, Scalar(0, 255, 0), Scalar(0, 255, 0));

    //info about matches
    printf("Number of total matches: %d\n", matches.size());
    printf("Number of good matches: %d\n", good_matches.size());

    //show matches
    imshow("Matches", img_matches);
    waitKey(0);

    return 0;
}