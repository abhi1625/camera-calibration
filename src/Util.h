// Copyright 2019
// Standard utilities header file

#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_
#endif  // SRC_UTIL_H_

#include <glob.h>
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>


using namespace boost::filesystem; //NOLINT

// TO DO : Allow relative oaths

std::vector<std::string> get_images_path(std::string images_path) {
    std::vector<std::string> path_string;
    path p{images_path};
    directory_iterator it(p);
    // https://theboostcpplibraries.com/boost.filesystem-iterators
    while (it != directory_iterator{}) {
        // path_string.emplace_back(it->path().string());
        path_string.emplace_back(it->path().string());
        it++;
    }
    return path_string;
}

cv::Mat read_image(std::string path, bool show_image = false) {
    cv::Mat image;
    cv::imread(path, CV_LOAD_IMAGE_COLOR);
    if (!image.data) {
        std::cout << "Could not read image" << std::endl;
    }
    if (show_image) {
        cv::namedWindow("Debug display", CV_WINDOW_AUTOSIZE);
        cv::imshow("Debug display", image);
        cv::waitKey(0);
    }
    return image;
}


