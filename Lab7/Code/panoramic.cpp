//Class for panoramic object
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include "header/panoramic.hpp"
#include "header/panoramic_utils.h"

Panoramic::Panoramic(const std::string& path) {
    //set path of the dataset
    p_path = path;
    cv::String data_path (path);
    std::vector<cv::String> fn;
    cv::glob(data_path, fn, true);

    //get all images in the dataset and store in vector "images"
    for(int i=0; i<fn.size(); ++i) {
        cv::Mat img = cv::imread(fn[i]);
        if(img.empty())
            std::printf("Could not open images at %s", fn[i]);
        else
            images.push_back(img);
    }
}

void Panoramic::visualizeImages() {
    //create window and display all images one at time
    cv::namedWindow("Images");
    for(int i=0; i<images.size(); ++i) {
        cv::imshow("Images", images[i]);
        cv::waitKey(0);
    }
}

cv::Mat Panoramic::create(const int& angle, const float& ratio) {
    //output image
    cv::Mat output = cv::Mat::zeros(cv::Size(500, 500), CV_8U);
    //vector of projected images
    std::vector<cv::Mat> proj_images;
    //create projected images using cylindiricalProj()
    for(int i=0; i<images.size(); ++i) {
        cv::Mat proj = cylindricalProj(images[i], angle);
        proj_images.push_back(proj);
    }

    //extract features using SIFT
    std::vector<cv::Mat> feat_images;
    int value = 400;
    cv::Ptr<cv::SIFT> detector = cv::SIFT::create(value);
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptor1, descriptor2;

    //matches for all images pairs
    std::vector<cv::DMatch> good_matches;
    
    for(int i=0; i<proj_images.size()-1; ++i) {
        //detect keypoints on both images
        detector->detectAndCompute(proj_images[i], cv::noArray(), keypoints1, descriptor1);
        detector->detectAndCompute(proj_images[i+1], cv::noArray(), keypoints2, descriptor2);
        //draw keypoints on projected images
        cv::Mat feat1, feat2;
        cv::drawKeypoints(proj_images[i], keypoints1, feat1);
        cv::drawKeypoints(proj_images[i+1], keypoints2, feat2);
        //save image with features
        //feat_images.push_back(feat1);
        //feat_images.push_back(feat2);
        //show images with features detection
        cv::imshow("Feature 1", feat1);
        cv::imshow("Feature 2", feat2);
        cv::waitKey(0);

        //match features using BFMatcher
        cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2);
        std::vector<cv::DMatch> matches;
        matcher->match(descriptor1, descriptor2, matches);
        
        std::printf("Number of matches: %d\n", matches.size());

        //find minimun distance among all matches
        float min_distance = matches[0].distance;
        for(int j=1; j<matches.size(); ++j) {
            if(matches[j].distance < min_distance) {
                min_distance = matches[j].distance;
            }
        }
    
        //reference distance
        float distance = ratio * min_distance;
        std::printf("Min distance: %f, reference: %f\n", min_distance, distance);
        
        //select only matches with desired distance in vector good_matches
        for(int k=0; k<matches.size(); ++k) {
            if(matches[k].distance < distance) {
                //add match to vector good_matches
                good_matches.push_back(matches[k]);
            }
        }
        std::printf("Number of good matches: %d\n", good_matches.size());

        //draw 'correct' matches between a pair of images
        cv::Mat draw;
        cv::drawMatches(proj_images[i], keypoints1, proj_images[i+1], keypoints2, good_matches, draw, cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 0));
        imshow("Match", draw);
        cv::waitKey(0);

        //store good keypoints from good_matches
        std::vector<cv::Point2f> points1, points2;
        for(int j=0; j<good_matches.size(); ++j) {
            points1.push_back(keypoints1[good_matches[j].queryIdx].pt);
            points2.push_back(keypoints2[good_matches[j].trainIdx].pt);
        }
        //find homography
        cv::Mat H = cv::findHomography(points1, points2, cv::RANSAC);
        std::cout << "H = " << std::endl << H << std::endl;
    
        //TODO: warp the two images
        cv::Mat stitch;
        cv::warpPerspective(images[i+1], stitch, H, cv::Size(images[i+1].cols, images[i+1].rows));
        cv::Mat result (cv::Size(images[i].cols+images[i+1].cols, images[i+1].rows), CV_8UC3);
        cv::Mat half1 = result(cv::Rect(0, 0, images[i].cols, images[i].rows));
        images[i].copyTo(half1);
        cv::Mat half2 = result(cv::Rect(images[i].cols-static_cast<int>(H.at<uchar>(0, 2)), 0, images[i].cols, images[i].rows));
        stitch.copyTo(half2);
        cv::imshow("Stitch", result);
        cv::waitKey(0);
    }

    //return output image
    return output;
}