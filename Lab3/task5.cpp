//Lab 3 task 5
#include <opencv2/highgui.hpp>

using namespace cv;

//Callback function for mouse event
void onMouse(int event, int x, int y, int flags, void* userdata) {
    //Square is 9x9 so range from the center point is 4
    int dim = 9;
    int range = dim/2;
    //Input image
    Mat img = *((Mat*) userdata);
    int rows = img.rows;
    int cols = img.cols;

    //Left button click on mouse
    if(event == EVENT_LBUTTONDOWN) {
        //Sum of the B, G, R values
        int blue_sum = 0;
        int green_sum = 0;
        int red_sum = 0;

        //Loop to get the sum of the colors in the square
        for(int i=y-range; i<y+range; ++i) {
            for(int j=x-range; j<x+range; ++j) {
                //Get values of the current pixel
                Vec3b pixel = img.at<Vec3b>(i, j);
                //Update sums
                blue_sum += (int) pixel[0];
                green_sum += (int) pixel[1];
                red_sum += (int) pixel[2];
            }
        }

        //Average values of the three colors, since it is a square of 9x9, divide by 81
        int blue_avg = blue_sum/81;
        int green_avg = green_sum/81;
        int red_avg = red_sum/81;
        //Print BGR color for the selected pixel
        printf("Pixel: (%d, %d), average BGR color :(%d, %d, %d)\n", y, x, blue_avg, green_avg, red_avg);
        
        //New image mask
        Mat mask (rows, cols, CV_8UC3);
        //Threshold t to be set
        int t = 60;

        //Creation of the new image
        for(int i=0; i<rows; ++i) {
            for(int j=0; j<cols; ++j) {
                //Get values of the current pixel
                Vec3b pixel = img.at<Vec3b>(i, j);
                int blue = pixel[0];
                int green = pixel[1];
                int red = pixel[2];
                //Check the current pixel values of the input image
                if(blue>blue_avg-t && blue<blue_avg+t && green>green_avg-t && green<green_avg+t && red>red_avg-t && red<red_avg+t) {
                    mask.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
                } else {
                    mask.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
                }
            }
        }
        //Dispaly mask
        namedWindow("Mask");
        imshow("Mask", mask);

        //New image colored by the mask
        Mat new_img (rows, cols, CV_8UC3);
        //default new color
        Vec3b color (92, 37, 201);

        //Creation of the new image
        for(int i=0; i<rows; ++i) {
            for(int j=0; j<cols; ++j) {
                //Get values of the current pixel
                Vec3b pixel = mask.at<Vec3b>(i, j);
                //Black color
                Vec3b black (0, 0, 0);
                //Check the current pixel values of the input image
                if(pixel == black) {
                    new_img.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
                } else {
                    new_img.at<Vec3b>(i, j) = color;
                }
            }
        }
        //Dispaly the new image
        namedWindow("Painted Image");
        imshow("Painted Image", new_img);
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

//CONCLUSION
//Setting the threshold with different values it is possible to change the color of part or the majority of the t-shirt/ball