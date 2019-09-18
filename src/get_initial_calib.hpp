// Copyright 2019
// Getting the initial estimate of the K matrix.

#pragma once

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace Eigen; //NOLINT
using std::vector;
using std::cout;

vector<cv::Point2f> get_corners(cv::Mat board_image,
                                    cv::Size pattern_size,
                                    bool show_corners);

cv::Mat get_V_matrix(vector<cv::Point2f> corner_vector, float square_size,
                        cv::Size pattern_size);

cv::Mat get_homography(cv::Mat world_corners, cv::Mat image_corners);
