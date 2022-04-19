//Definition of functions in the header file "segmentation.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void otsuSegmentation(const cv::Mat& input, cv::Mat& output, const int ksize) {
    cv::Mat gray, temp, mask;
    //convert input image to grayscale
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    //apply blur filter
    cv::blur(gray, temp, cv::Size(ksize, ksize));
    //Otsu optimal threshold to output image
    cv::threshold(temp, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    //---segment input image with the original colors---
    //since there are only two segment, store two values
    //find the average values of both areas, comparing with otsu mask
    long sum1[3] = {0, 0, 0}, sum2[3] = {0, 0, 0};
    int count1 = 0, count2 = 0;

    //calculate sum of pixel's value in both areas
    for(int i=0; i<input.rows; ++i) {
        for(int j=0; j<input.cols; ++j) {
            //white value in otsu threshold
            if(mask.at<unsigned char>(i, j) > 128) {
                sum1[0] += input.at<cv::Vec3b>(i, j)[0];
                sum1[1] += input.at<cv::Vec3b>(i, j)[1];
                sum1[2] += input.at<cv::Vec3b>(i, j)[2];
                count1++;
            } else {
                sum2[0] += input.at<cv::Vec3b>(i, j)[0];
                sum2[1] += input.at<cv::Vec3b>(i, j)[1];
                sum2[2] += input.at<cv::Vec3b>(i, j)[2];
                count2++;
            }
        }
    }
    //calculate average
    uchar avg1[3] = {0, 0, 0};
    uchar avg2[3] = {0, 0, 0};
    for(int i=0; i<3; ++i) {
        avg1[i] = sum1[i] / count1;
        avg2[i] = sum2[i] / count2;
    }
    //printf("%d, %d, %d\n", sum2[0], sum2[1], sum2[2]);
    //printf("%d\n", count1);
    //printf("%d\n", count2);
    //printf("%d, %d, %d\n", avg2[0], avg2[1], avg2[2]);
    
    //color the two areas
    for(int i=0; i<output.rows; ++i) {
        for(int j=0; j<output.cols; ++j) {
            //white value in otsu threshold
            if(mask.at<unsigned char>(i, j) > 128) {
                output.at<cv::Vec3b>(i, j) = cv::Vec3b(avg1[0], avg1[1], avg1[2]);
            } else {
                output.at<cv::Vec3b>(i, j) = cv::Vec3b(avg2[0], avg2[1], avg2[2]);
            }
        }
    }
}

void regionGrowing(const cv::Mat& input, cv::Mat& mask, const int ksize, uchar similarity) {
    //number of rows and columns of input and output image
    int rows = input.rows;
    int cols = input.cols;
    //predicate Q for controlling growing, 0 if not visited yet, 255 otherwise
    cv::Mat Q = cv::Mat::zeros(rows, cols, CV_8UC1);

    //convert to grayscale, apply blur and threshold (inverse to obtain white for cracks)
    cv::Mat gray, img;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::blur(gray, img, cv::Size(ksize, ksize));
    cv::threshold(img, img, 50, 255, cv::THRESH_BINARY_INV);
    cv::imshow("Threshold", img);

    //loop threshold image to erode pixel groups in a single one (there may be better methods (?))
    for(int i=0; i<rows; ++i) {
        for(int j=0; j<cols; ++j) {
            //if the current pixel is black pass this iteration
            if (img.at<uchar>(i, j) == 0)
                continue;
            //flag for controls on neighbors
            bool flag = false;
            //check right, down, left and up pixel, in this order
            if(j < cols-1 && img.at<uchar>(i, j+1) == 255) {
                flag = true;
            } else if(i < rows-1 && img.at<uchar>(i+1, j) == 255) {
                flag = true;
            } else if(j > 0 && img.at<uchar>(i, j-1) == 255) {
                flag = true;
            } else if(i > 0 && img.at<uchar>(i-1, j) == 255) {
                flag = true;
            }

            //change color if flag is true after checking all neighbors
            if(flag)
                img.at<uchar>(i, j) = 0;
        }
    }

    cv::imshow("Erosion", img);
    cv::waitKey(0);

    //point to be visited
    std::vector<std::pair<int, int>> points;

    int p = 0;
    //add points of the skeleton image into the vector
    for(int i=0; i<img.rows; ++i) {
        for(int j=0; j<img.cols; ++j) {
            if(img.at<uchar>(i, j) == 255) {
                //add to points vector
                //NOTE: not all the points of the skeleton image may be added, since they could be too much
                points.push_back(std::pair<int, int>(i, j));
                //std::printf("White point at (%d, %d)\n", i, j);
                //update point counter
                p++;
            }
        }
    }
    std::printf("Points: %d", p);
    while(!points.empty()) {
        //pop a single point
        std::pair<int, int> p = points.back();
        points.pop_back();

        //get color value of the point
        uchar color = gray.at<uchar>(p.first, p.second);
        //set the current pixel visited
        Q.at<uchar>(p.first, p.second) = 255;

        //loop for each neighbour
        for(int i=p.first-1; i<=p.first+1; ++i) {
            for(int j=p.second-1; j<=p.second+1; ++j) {
                //get neighbour pixel value
                uchar neigh = gray.at<uchar>(i, j);
                //check if it has been visited
                uchar visited = Q.at<uchar>(i, j);

                //check if the neighbour pixel is similar
                if(!visited && std::abs(neigh-color) <= similarity) {
                    points.push_back(std::pair<int, int>(i, j));
                }
            }
        }
    }
    //copy Q into mask
    mask = Q.clone();
}