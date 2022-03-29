//Lab 1 Task 1
//Implement the openCV Hello World

#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
	cv::Mat img = cv::imread(argv[1]);
	cv::namedWindow("Example 1");
	cv::imshow("Example 1", img);
	cv::waitKey(0);
	
	return 0;
}
