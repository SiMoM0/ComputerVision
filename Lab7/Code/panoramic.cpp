//Class for panoramic object
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "header/panoramic.hpp"
#include "header/panoramic_utils.h"

Panoramic::Panoramic(const std::string& path) {
    p_path = path;
    cv::String data_path (path);
    std::vector<cv::String> fn;
    cv::glob(data_path, fn, true);

    for(int i=0; i<fn.size(); ++i) {
        cv::Mat img = cv::imread(fn[i]);
        if(img.empty())
            std::printf("Could not open images at %s", fn[i]);
        else
            images.push_back(img);
    }
}

void Panoramic::visualizeImages() {
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
    //create projected images
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
    std::vector<std::vector<cv::DMatch>> good_matches {images.size()-1};
    
    for(int i=0; i<proj_images.size()-1; ++i) {
        //detect keypoints on both images
        detector->detectAndCompute(proj_images[i], cv::noArray(), keypoints1, descriptor1);
        detector->detectAndCompute(proj_images[i+1], cv::noArray(), keypoints2, descriptor2);
        //draw keypoints on projected images
        cv::Mat feat1, feat2;
        cv::drawKeypoints(proj_images[i], keypoints1, feat1);
        cv::drawKeypoints(proj_images[i+1], keypoints2, feat2);
        feat_images.push_back(feat1);
        feat_images.push_back(feat2);
        cv::imshow("Feature", feat1);
        cv::waitKey(0);

        //match features using BFMatcher
        cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2);
        std::vector<cv::DMatch> matches;
        matcher->match(descriptor1, descriptor2, matches);
        
        std::printf("Number of matches: %d\n", matches.size());

        float min_distance = matches[0].distance;
        for(int j=1; j<matches.size(); ++j) {
            if(matches[j].distance < min_distance) {
                min_distance = matches[j].distance;
            }
        }
    
        //reference distance
        float distance = ratio * min_distance;
        std::printf("Min distance: %f, reference: %f\n", min_distance, distance);
        
        for(int k=0; k<matches.size(); ++k) {
            if(matches[k].distance < distance) {
                //add match to vector good_matches
                good_matches[i].push_back(matches[k]);
            }
        }
        //insert the remaining matches into the vector good_matches
        //good_matches.push_back(matches);
        std::printf("Number of good matches: %d\n", good_matches[i].size());

        cv::Mat result;
        cv::drawMatches(images[i], keypoints1, images[i+1], keypoints2, good_matches[i], result, cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 0));
        imshow("Match", result);
        cv::waitKey(0);
    }

    //return output image
    return output;
}