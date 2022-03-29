//Lab 1 Task 3
//Create two images of 256x256, one channel in order to create vertical and horizontal channel
//Create two images of 300x300, chessboard with squares of 20x20 and 50x50
#include <opencv2/highgui.hpp>

int main(void) {
	cv::Mat img1 (256, 256, CV_8U);
	cv::Mat img2 (256, 256, CV_8U);
	for(int i=0; i<256; ++i) {
		for(int j=0; j<256; ++j) {
			img1.at<unsigned char>(i, j) = i;
			img2.at<unsigned char>(i, j) = j;
		}
	}
	
	cv::Mat img3 (300, 300, CV_8U);
	cv::Mat img4 (300, 300, CV_8U);
	for(int i=0; i<300; ++i) {
		for(int j=0; j<300; ++j) {
			if(((i/20)%2 + (j/20)%2)%2 == 0)
				img3.at<unsigned char>(i, j) = 255;
			else
				img3.at<unsigned char>(i, j) = 0;
			
			if(((i/50)%2 + (j/50)%2)%2 == 0)
				img4.at<unsigned char>(i, j) = 255;
			else
				img4.at<unsigned char>(i, j) = 0;
		}
	}
	
	cv::namedWindow("Vertical Gradient");
	cv::imshow("Vertical Gradient", img1);
	cv::namedWindow("Horizontal Gradient");
	cv::imshow("Horizontal Gradient", img2);
	
	cv::namedWindow("Chess 20");
	cv::imshow("Chess 20", img3);
	cv::namedWindow("Chess 50");
	cv::imshow("Chess 50", img4);
	
	cv::waitKey(0);
	
	return 0;
}
