//Lab 4 task 3
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat src = imread(argv[1]);
    Mat gray, smooth, canny, hough;

    //convert to grayscale
    cvtColor(src, gray, COLOR_BGR2GRAY);
    //apply blur
    blur(gray, smooth, Size(3, 3));

    //apply canny detector
    Canny(smooth, canny, 50, 200, 3);

    //clone src to hough
    hough = src.clone();

    //standard Hough transform
    vector<Vec2f> lines;
    HoughLines(canny, lines, 1, CV_PI/180, 150, 0, 0);
    //draw lines
    for(size_t i=0; i<lines.size(); ++i) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        Point pt1, pt2;
        double a, b, x0, y0;

        //check for the two road lines
        //the first condition look for an angle of about 45
        //the second for an angle of about 135
        if(theta>CV_PI/180*40 && theta<CV_PI/180*60) {
            a = cos(theta);
            b = sin(theta);
            x0 = a*rho;
            y0 = b*rho;
            pt1.x = cvRound(x0 + 300*(-b));
            pt1.y = cvRound(y0 + 300*a);
            pt2.x = cvRound(x0 - 300*(-b));
            pt2.y = cvRound(y0 - 300*a);
            printf("Points: (%d, %d), (%d, %d)\n", pt1.x, pt1.y, pt2.x, pt2.y);
            line(hough, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
        }
        else if(theta>CV_PI/180*130 && theta<CV_PI/180*150) {
            a = cos(theta);
            b = sin(theta);
            x0 = a*rho;
            y0 = b*rho;
            pt1.x = cvRound(x0 + 400*(-b) + 705);
            pt1.y = cvRound(y0 + 400*a + 655);
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*a);
            printf("Points: (%d, %d), (%d, %d)\n", pt2.x, pt2.y, pt1.x, pt1.y);
            line(hough, pt2, pt1, Scalar(0, 0, 255), 3, LINE_AA);
        }
    }

    printf("Lines: %d\n", lines.size());

    //color below the red lines
    for(int i=0; i<hough.cols; ++i) {
        bool flag = false;
        for(int j=0; j<hough.rows; ++j) {
            //if the pixel found before was red color the next
            if(flag) {
                hough.at<Vec3b>(j, i) = Vec3b(0, 0, 255);
            }
            else if(hough.at<Vec3b>(j, i) == Vec3b(0, 0, 255)) {
                flag = true;
            }
        }
    }

    //display image
    namedWindow("Image");
    namedWindow("Canny Detector");
    namedWindow("Hough Transform");
    imshow("Image", src);
    imshow("Canny Detector", canny);
    imshow("Hough Transform", hough);

    waitKey(0);
    
    return 0;
}