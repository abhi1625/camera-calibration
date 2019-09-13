// Copyright 2019

#include <opencv2/opencv.hpp>
#include "get_initial_calib.h" //NOLINT
#include "Util.h"

int main() {
  std::vector<std::string> paths = get_images_path(
    "/home/kartikmadhira/github/camera-calibration/checkerboard_images");
  for (uint16_t i = 0; i < paths.size(); i++) {
    std::cout << paths[i] << "\n";
  }
}
