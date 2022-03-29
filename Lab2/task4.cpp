//Lab 2 task 4
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main(int argc, char** argv) {
    Mat src = imread(argv[1]);
    Mat img;
    cvtColor(src, img, COLOR_BGR2GRAY);

    int hist_size = 256;
    float range[] = {0, 256};
    const float* hist_range[] = {range};
    bool uniform = true, accumulate = false;
    //calculate histogram using calcHist
    Mat hist;
    calcHist(&img, 1, 0, Mat(), hist, 1, &hist_size, hist_range, uniform, accumulate);
    //Create image to display histogram
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double) hist_w/hist_size);
    Mat hist_img (hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    //Normalize the histogram
    normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX, -1, Mat());
    
    for(int i=1; i<hist_size; ++i) {
        line(hist_img, Point(bin_w*(i-1), hist_h-cvRound(hist.at<float>(i-1))), Point(bin_w*(i), hist_h-cvRound(hist.at<float>(i))), Scalar(255, 255, 255), 2, 8, 0);
    }
    
    //save histogram
    //imwrite("histogram.jpg", hist_img);

    //Display histogram
    namedWindow("Histogram");
    imshow("Histogram", hist_img);
    waitKey(0);

    return 0;
}