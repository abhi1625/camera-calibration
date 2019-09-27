/**@file get_initial_calib.hpp
 * @brief Header file containing required headers and methods
 *        for computing initial intrinsic calibration matrix.
 *
 * Detailed description follows here.
 * @author     : Abhinav Modi, Kartik Madhira
 * 
 * Copyright 2019 MIT License
 */

#pragma once

#include <iostream>
#include <vector>
#include "Eigen/Dense"
#include "Eigen/Core"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace Eigen; //NOLINT
using std::vector;
using std::cout;


 /**
  * @brief get_corners outputs corners of the input checkerboard image.
  * @param board_image  - input checkerboard image.
  * @param pattern_size - tuple(num_rows, num_columns) in the checkerboard.
  * @param show_corners - flag to enable display of input image with corners. 
  * @return vector of corners points in the input checkerboard image.
  */
vector<cv::Point2f> get_corners(cv::Mat board_image, cv::Size pattern_size,
                                bool show_corners);

 /**
  * @brief get_V_matrix outputs V matrix. 
  * @param corner_vector - vector of corner points of the checkerboard image.  
  * @param square_size   - size of the square pattern in the checkerboard.
  * @param pattern_size  - tuple(num_rows, num_columns) in the checkerboard. 
  * @return 2D matrix of shape(12,2) containing V values 
  */
MatrixXf get_V_matrix(vector<cv::Point2f> corner_vector, float square_size,
                        cv::Size pattern_size);

/**
 * @brief get_homography computes homography matrix using image and world coords
 * @param world_corners - (N, 2) matrix of world coordinates  
 * @param square_size   - (N, 2) matrix of image coordinates
 * @return (3, 3) homography matrix  
 */
cv::Mat get_homography(cv::Mat world_corners, cv::Mat image_corners);

/**
 * @brief create_V_matrix computes V matrix using homography
 * @param world_corners - (N, 2) matrix of world coordinates  
 * @param square_size   - (N, 2) matrix of image coordinates
 * @return (3, 3) homography matrix  
 */
MatrixXf create_V_matrix(const cv::Mat& H);

/**
 * @brief get_vij_matrix is a helper function to compute vij matrix
 * @param vij - reference to the vij matrix for computing V matrix    
 * @param H   - const reference to the Homography matrix 
 * @param i   - index i for vij 
 * @param j   - index j for vij
 */
void get_vij_matrix(MatrixXf& vij, const cv::Mat& H, int i, int j); //NOLINT
