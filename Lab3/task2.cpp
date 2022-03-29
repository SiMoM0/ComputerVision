//Lab 3 task 2
#include <opencv2/highgui.hpp>

using namespace cv;

//Callback function for mouse event
void onMouse(int event, int x, int y, int flags, void* userdata) {
    //Input image
    Mat img = *((Mat*) userdata);
    //Left button click on mouse
    if(event == EVENT_LBUTTONDOWN) {
        //Get values of the selected pixel
        Vec3b pixel = img.at<Vec3b>(y, x);
        int blue = (int) pixel[0];
        int green = (int) pixel[1];
        int red = (int) pixel[2];
        //Print BGR color for the selected pixel
        printf("Pixel: (%d, %d), BGR color :(%d, %d, %d)\n", y, x, blue, green, red);
    }
}

int main(int argc, char** argv) {
    //Load image
    Mat src = imread(argv[1]);
    //Show image
    namedWindow("Image");
    //Set callback functions for mouse events
    setMouseCallback("Image", onMouse, &src);
    imshow("Image", src);
    waitKey(0);

    return 0;
}