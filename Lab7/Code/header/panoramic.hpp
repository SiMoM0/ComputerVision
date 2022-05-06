//Header file for panoramic class
#ifndef PANORAMIC_H
#define PANORAMIC_H
#include <opencv2/core.hpp>

class Panoramic {
    private:
        std::string p_path;
        std::vector<cv::Mat> images;

    public:
        /**
         * Create Panoramic object and store all the images
         * @param path the system path for the image dataset
         */
        Panoramic(const std::string& path);
        /**
         * Visualize all the images uploaded one at time
         */
        void visualizeImages();

        /**
         * Create the panoramic image
         * @param angle angle for the cylindrical projection
         * @return panoramic image of type cv::Mat
         */
        cv::Mat create(const int& angle, const float& ratio);

        /**
         * Return the path string of the dataset
         * @return path std::string
         */
        std::string getPath() {return p_path;}

        /**
         * Return all the images
         * @return a std::vector containing all the images
         */
        std::vector<cv::Mat> getImages() {return images;}

        /**
         * Return the number of images uploaded
         * @return number of images
         */
        int getSize() {return images.size();}
};

#endif