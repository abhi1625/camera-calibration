// Copyright 2019
#include "get_initial_calib.hpp" //NOLINT
#include "Util.h"

int main() {
  std::vector<std::string> paths = get_images_path(
    "/home/kartikmadhira/github/camera-calibration/checkerboard_images");
  std::cout << "the image is " << paths[0] << "\n";
  cv::Mat board_image = read_image(paths[0]);
  std::vector<cv::Point2f> corners = get_corners(board_image,
                                      cv::Size(9, 6), false);
  // auto corners_matrix = get_corner_matrix(corners);
  auto V = get_V_matrix(corners, 21.5, cv::Size(9, 6));
  cout << V;
  // for (auto &point : corners) {
  //   std::cout << point.x << "----" << point.y << "\n";
  // }
}
