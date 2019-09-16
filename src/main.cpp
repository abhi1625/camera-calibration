// Copyright 2019

#include <opencv2/opencv.hpp>
#include "get_initial_calib.h" //NOLINT
#include "Util.h"

int main() {
  std::vector<std::string> paths = get_images_path(
    "../checkerboard_images");
  for (uint16_t i = 0; i < paths.size(); i++) {
    std::cout << paths[i] << "\n";
  }
  std::cout << "the image is " << paths[0] <<"\n";
  cv::Mat board_image = read_image(paths[0]);
  get_corners(board_image, cv::Size(9, 6), true);
}
