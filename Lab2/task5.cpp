//Lab 2 task 5
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main(int argc, char** argv) {
    Mat src = imread(argv[1]);
    cvtColor(src, src, COLOR_BGR2GRAY);

    //histogram equalization
    Mat img;
    equalizeHist(src, img);
    //imwrite("equalized_image.jpg", img);

    //create histograms of both images
    int hist_size = 256;
    float range[] = {0, 256};
    const float* hist_range[] = {range};
    bool uniform = true, accumulate = false;
    //calculate histograms using calcHist
    Mat hist1, hist2;
    calcHist(&src, 1, 0, Mat(), hist1, 1, &hist_size, hist_range, uniform, accumulate);
    calcHist(&img, 1, 0, Mat(), hist2, 1, &hist_size, hist_range, uniform, accumulate);
    //Create images to display histograms
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double) hist_w/hist_size);
    Mat hist_src (hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    Mat hist_img (hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    //Normalize the histograms
    normalize(hist1, hist1, 0, hist_src.rows, NORM_MINMAX, -1, Mat());
    normalize(hist2, hist2, 0, hist_img.rows, NORM_MINMAX, -1, Mat());
    
    for(int i=1; i<hist_size; ++i) {
        line(hist_src, Point(bin_w*(i-1), hist_h-cvRound(hist1.at<float>(i-1))), Point(bin_w*(i), hist_h-cvRound(hist1.at<float>(i))), Scalar(255, 255, 255), 2, 8, 0);
        line(hist_img, Point(bin_w*(i-1), hist_h-cvRound(hist2.at<float>(i-1))), Point(bin_w*(i), hist_h-cvRound(hist2.at<float>(i))), Scalar(255, 255, 255), 2, 8, 0);
    }

    //save equalized histogram
    //imwrite("equalize_hist.jpg", hist_img);

    //display image and histogram
    namedWindow("Original Image");
    imshow("Original Image", src);
    namedWindow("Original Histogram");
    imshow("Original Histogram", hist_src);
    namedWindow("Equalized Image");
    imshow("Equalized Image", img);
    namedWindow("Equalized Histogram");
    imshow("Equalized Histogram", hist_img);
    waitKey(0);

    return 0;
}