// Copyright 2019
// Standard utilities header file

#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include <opencv/highgui.h>
#include <glob.h>
#include <string>
#include <vector>
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

cv::Mat read_image(std::string path) {
}

#endif  // SRC_UTIL_H_

