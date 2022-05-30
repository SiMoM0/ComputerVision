//Lab 8 task 2
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;

int main() {
    //path and vector for images
    String path = "data/checkerboard_images/*.png";
    String path_test = "data/test_image.png";
    vector<String> fn;
    vector<Mat> images;
    glob(path, fn, true);

    //load all the images
    for(int i=0; i<fn.size(); ++i) {
        //read images in grayscale
        Mat img = imread(fn[i], IMREAD_GRAYSCALE);
        //check correct image reading
        if(img.empty()) {
            printf("Error uploading image %s\n", fn[i]);
        } else {
            images.push_back(img);
        }
    }
    //upload test image
    Mat test_image = imread(path_test);

    //checkerboard dimensions
    int BOARD[2] {6, 5};
    Size patternsize (BOARD[0], BOARD[1]);
    //objects points and corners for each image
    vector<vector<Point3f>> objpoints;
    vector<vector<Point2f>> corners;

    //define coordinates for 3d points, for objpoints
    vector<Point3f> objp;
    for(int i=0; i<BOARD[1]; ++i) {
        for(int j=0; j<BOARD[0]; ++j) {
            objp.push_back(Point3f(j, i, 0));
        }
    }

    //findChessboardCorners task
    for(int i=0; i<images.size(); ++i) {
        //corners for each image
        vector<Point2f> c;
        bool patternfound = findChessboardCorners(images[i], patternsize, c,
                                                CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
        //check if the pattern has been found
        if(patternfound) {
            cornerSubPix(images[i], c, Size(11, 11), Size(-1, -1),
                        TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
            //save the corners
            corners.push_back(c);
        }
        //draw corners
        drawChessboardCorners(images[i], patternsize, Mat(corners[i]), patternfound);

        //save 3d coordinates for each image
        objpoints.push_back(objp);
    }

    //display all images with corners
    for(int i=0; i<images.size(); ++i) {
        imshow("Image", images[i]);
        waitKey(0);
    }

    //variables for calibration
    Mat camera_matrix, dist_coeffs, R, T;
    //perform calibrateCamera and get reprojection error
    //Other approach: use perViewError paramterer of overloaded method
    double reproj_error = calibrateCamera(objpoints, corners, Size(images[0].rows, images[0].cols), camera_matrix, dist_coeffs, R, T);
    printf("RMS re-projection error: %d", reproj_error);
    
    //variables for undistort and rectify
    Mat new_matrix, map1, map2;
    //sample image
    Mat img = images[10];
    //apply initUnistortRectifyMap
    initUndistortRectifyMap(camera_matrix, dist_coeffs, R, new_matrix, Size(img.cols, img.rows), CV_32FC1, map1, map2);
    //undistorted image
    Mat undistorted;
    //undistort(img, undistorted, new_matrix, dist_coeffs);

    //show images
    imshow("Normal Image", img);
    //imshow("Undistorted Image", undistorted);
    waitKey(0);

    return 0;
}