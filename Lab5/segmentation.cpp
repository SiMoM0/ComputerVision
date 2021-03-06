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
    double value = cv::threshold(temp, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    std::printf("Otsu threshold: %f\n", value);

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
    //cv::imshow("Threshold", img);

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

    //cv::imshow("Erosion", img);
    //cv::waitKey(0);

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
    std::printf("Points: %d\n", p);
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
                //check if pixel coordinates exist
                if(i>=0 && i<rows && j>=0 && j<cols) {
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
    }
    //copy Q into mask
    mask = Q.clone();
}

void watershedSegmentation(const cv::Mat& input, cv::Mat& output) {
    //implementation of watershed algorithm as described in the documentation (NOT GOOD RESULT)
    cv::Mat bw, dist;

    //convert to grayscale, smooth and use threshold
    cv::cvtColor(input, bw, cv::COLOR_BGR2GRAY);
    cv::blur(bw, bw, cv::Size(5, 5));
    cv::threshold(bw, bw, 60, 255, cv::THRESH_BINARY_INV);
    //cv::imshow("Binary image", bw);
    //cv::waitKey(0);

    //perform the distance transofrm algorithm
    cv::distanceTransform(bw, dist, cv::DIST_L2, 3);
    //normalize distance image
    cv::normalize(dist, dist, 0, 1.0, cv::NORM_MINMAX);
    //cv::imshow("Distance", dist);
    //cv::waitKey(0);

    //threshold to obtain peaks, markers for cracks
    cv::threshold(dist, dist, 0.5, 1.0, cv::THRESH_BINARY);
    //dilate the dist image
    //cv::dilate(dist, dist, cv::Mat::ones(3, 3, CV_8U));
    //cv::imshow("Dilate", dist);
    //cv::waitKey(0);

    //from each blob create a seed for watershed algorithm
    cv::Mat dist8u, markers8u;
    cv::Mat markers = cv::Mat::zeros(dist.size(), CV_32SC1);
    dist.convertTo(dist8u, CV_8U);
    //find total markers
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(dist8u, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    //number of contours
    int ncomp = static_cast<int>(contours.size());
    std::printf("Contours: %d\n", ncomp);

    //draw foreground markers
    for(int i=0; i<ncomp; ++i) {
        cv::drawContours(markers, contours, i, cv::Scalar(i+1), -1);
    }
    //draw background markers
    cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(255), -1);
    markers.convertTo(markers8u, CV_8U, 10);
    //cv::imshow("Markers", markers8u);
    //cv::waitKey(0);

    //apply the watershed algorithm
    cv::Mat result = input.clone();
    cv::watershed(result, markers);
    cv::Mat mark;
    markers.convertTo(mark, CV_8U);
    cv::bitwise_not(mark, mark);

    //generate random colors
    cv::RNG rng (12345);
    std::vector<cv::Vec3b> colors;
    for(int i=0; i<ncomp; ++i) {
        uchar b = static_cast<uchar>(rng.uniform(0, 255));
        uchar g = static_cast<uchar>(rng.uniform(0, 255));
        uchar r = static_cast<uchar>(rng.uniform(0, 255));
        //insert new color
        colors.push_back(cv::Vec3b(b, g, r));
    }

    //create output image
    for(int i=0; i<markers.rows; ++i) {
        for(int j=0; j<markers.cols; ++j) {
            int index = markers.at<int>(i, j);
            if(index > 0 && index <= ncomp) {
                output.at<cv::Vec3b>(i, j) = colors[index-1];
            }
        }
    }
}

void kmeansSegmentation(const cv::Mat& input, cv::Mat& output, const int k, const bool color=false) {
    //data array for kmeans function, input image need to be converted to array like
    cv::Mat data = input.reshape(1, input.rows * input.cols);
    //convert to 32 float
    data.convertTo(data, CV_32F);
    
    //structures for kmeans function
    std::vector<int> labels;
    cv::Mat1f centers;
    //apply kmeans
    double compactness = cv::kmeans(data, k, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 0.1), 10, cv::KMEANS_PP_CENTERS, centers);
    std::printf("Compactness: %f\n", compactness);

    //update data array with clusters colors
    for(int i=0; i<data.rows; ++i) {
        data.at<float>(i, 0) = centers(labels[i], 0);
        data.at<float>(i, 1) = centers(labels[i], 1);
        data.at<float>(i, 2) = centers(labels[i], 2);
    }

    //reshape into output image
    output = data.reshape(3, input.rows);
    output.convertTo(output, CV_8UC3);

    //for task 3, segment only the t-shirts (parameter color set to true)
    if(color) {
        //center number for the t-shirts is the fifth
        int index = 4;
        cv::Mat mask;
        //get the cluster color and apply inRange function
        cv::Scalar color (centers(index, 0), centers(index, 1), centers(index, 2));
        cv::inRange(output, color, color, mask);
        //cv::imshow("Mask", mask);
        //cv::waitKey(0);

        //apply mask using copyTo function and temp Mat
        cv::Mat tmp;
        output.copyTo(tmp, mask);
        //cv::imshow("Final", fin);
        //cv::waitKey(0);

        //clone to output image
        output = tmp.clone();
    }
}