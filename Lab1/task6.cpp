//Lab 1 Task 6
//Rotate and affine transform as in the docs
//In order to use rotation, add the include imgproc
//Complie adding -lopencv_imgproc
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv) {
	cv::Mat img = cv::imread(argv[1]);
	//rotated image
	cv::Mat rotated;
	//center point for rotation
	cv::Point2f center (img.cols/2, img.rows/2);
	cv::Mat r = cv::getRotationMatrix2D(center, -45, 0.6);
	cv::warpAffine(img, rotated, r, img.size());
	
	//affine transformation with 3 points, as explained in the documentation
	cv::Point2f srcTri[3];
	srcTri[0] = cv::Point2f(0.f, 0.f);
	srcTri[1] = cv::Point2f(img.cols - 1.f, 0.f);
	srcTri[2] = cv::Point2f(0.f, img.rows - 1.f);
	cv::Point2f dstTri[3];
	dstTri[0] = cv::Point2f(0.f, img.rows*0.33f);
	dstTri[1] = cv::Point2f(img.cols*0.85f, img.rows*0.25f);
	dstTri[2] = cv::Point2f(img.cols*0.15f, img.rows*0.7f);
	cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
	//the new image
	cv::Mat warp_dst = cv::Mat::zeros( img.rows, img.cols, img.type());
	cv::warpAffine( img, warp_dst, warp_mat, warp_dst.size());
	
	//show the images
	cv::namedWindow("Rotated");
	cv::imshow("Rotated", rotated);
	cv::namedWindow("3 points");
	cv::imshow("3 points", warp_dst);
	cv::waitKey(0);
	
	return 0;
}