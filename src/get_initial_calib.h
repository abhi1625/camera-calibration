// Copyright 2019
// Getting the initial estimate of the K matrix.

#ifndef SRC_GET_INITIAL_CALIB_H_
#define SRC_GET_INITIAL_CALIB_H_
#endif  // SRC_GET_INITIAL_CALIB_H_

#include <eigen3/Eigen/Dense>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Pattern size set by the user to accomodate
// different checkerboard pattern
std::vector<cv::Point2f> get_corners(cv::Mat board_image,
                            cv::Size pattern_size, bool show_corners = false) {
    // Get the chessboard corners irrespective of the orientation.
    cv::Mat gray;
    std::vector<cv::Point2f> corners;
    cv::cvtColor(board_image, gray, CV_BGR2GRAY);
    bool pattern_bool = cv::findChessboardCorners(board_image, pattern_size,
                        corners, cv::CALIB_CB_ADAPTIVE_THRESH +
                        cv::CALIB_CB_NORMALIZE_IMAGE+ cv::CALIB_CB_FAST_CHECK);
    if (pattern_bool) {
        if (show_corners) {
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
            cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            cv::drawChessboardCorners(board_image, pattern_size,
                                cv::Mat(corners), pattern_bool);
            cv::namedWindow("Corners window", CV_WINDOW_NORMAL);
            cv::resizeWindow("Corners window", 800, 600);
            cv::imshow("Corners window", board_image);
            cv::waitKey(0);
        }
        return corners;
    }
    std::cout << "WARNING: Corner pattern recognition failed" << std::endl;
    return corners;
}
// Estimate Homography from the given image
// cv::Mat get_homographies(cv::)
