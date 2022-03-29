//Lab 1 Task 5
//Create three images, each one corresponding to a channel of the initial image
#include <iostream>
#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
	cv::Mat img = cv::imread(argv[1]);
	//get number of rows and columns
	int row = img.rows;
	int col = img.cols;
	//print number of channels
	int channels = img.channels();
	std::cout << "Number of channels: " << channels << "\n";
	//create three new images
	cv::Mat img1 (row, col, CV_8U);
	cv::Mat img2 (row, col, CV_8U);
	cv::Mat img3 (row, col, CV_8U);
	//check on the number of channels
	if(channels == 3) {		
		//modify the three images
		for(int i=0; i<row; ++i) {
			for(int j=0; j<col; ++j) {
				//current pixel
				cv::Vec3b current_pixel = img.at<cv::Vec3b>(i, j);
				//img1 is the blue channel
				img1.at<unsigned char>(i, j) = current_pixel[0];
				//img2 is the green channel
				img2.at<unsigned char>(i, j) = current_pixel[1];
				//img3 is the red channel
				img3.at<unsigned char>(i, j) = current_pixel[2];
			}
		}
	}
	//show the images
	cv::namedWindow("Blue");
	cv::imshow("Blue", img1);
	cv::namedWindow("Green");
	cv::imshow("Green", img2);
	cv::namedWindow("Red");
	cv::imshow("Red", img3);
	//store and print key
	char key = cv::waitKey(0);
	std::cout << "Key: " << key << "\n";
	
	return 0;
}