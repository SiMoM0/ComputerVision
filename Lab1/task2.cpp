//Lab 1 Task 2
//Print number of channels and output of waitKey()
#include <iostream>
#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
	cv::Mat img = cv::imread(argv[1]);
	//print number of channels
	std::cout << "Number of channels: " << img.channels() << "\n";
	cv::namedWindow("Example 1");
	cv::imshow("Example 1", img);
	//store and print key
	char key = cv::waitKey(0);
	std::cout << "Key: " << key << "\n";
	
	return 0;
}
