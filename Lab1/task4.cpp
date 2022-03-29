//Lab 1 Task 4
//Set a channel of the image to 0
#include <iostream>
#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
	cv::Mat img = cv::imread(argv[1]);
	//print number of channels
	int channels = img.channels();
	std::cout << "Number of channels: " << channels << "\n";
	//check on the number of channels
	if(channels == 3) {
		for(int i=0; i<img.rows; ++i) {
			for(int j=0; j<img.cols; ++j) {
				//set to 0 the blue channel
				//img.at<cv::Vec3b>(i, j)[0] = 0;
				//set to 0 the green channel
				img.at<cv::Vec3b>(i, j)[1] = 0;
				//set to 0 the red channel
				//img.at<cv::Vec3b>(i, j)[2] = 0;
			}
		}
	}
	
	cv::namedWindow("Example 1");
	cv::imshow("Example 1", img);
	//store and print key
	char key = cv::waitKey(0);
	std::cout << "Key: " << key << "\n";
	
	return 0;
}

//CONCLUSION
//OpenCV seems to use the folllowing representation of the colors: 'BGR'