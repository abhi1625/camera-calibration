/**@file get_initial_calib.cpp
 * @brief Source file for get_initial_calib.hpp. 
 *        Contains definitions for functions declared in the included .hpp file
 *
 * Detailed description follows here.
 * @author     : Abhinav Modi, Kartik Madhira
 * 
 * Copyright 2019 MIT License
 */

#include "get_initial_calib.hpp"

// Pattern size set by the user to accomodate
// different checkerboard pattern
vector<cv::Point2f> get_corners(cv::Mat board_image,
                            cv::Size pattern_size, bool show_corners = false) {
    // Get the chessboard corners irrespective of the orientation.
    cv::Mat gray;
    vector<cv::Point2f> corners;
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

cv::Mat get_homography(cv::Mat _world_corners, cv::Mat _image_corners) {
    cv::Mat H = cv::findHomography(_world_corners, _image_corners);
    return H;
}

MatrixXf get_V_matrix(vector<cv::Point2f> corner_vector, float square_size,
                        cv::Size pattern_size) {
    // Get the total indices in the pattern size
    int total_indices = pattern_size.height * pattern_size.width;
    // Prepare 3D and corressponding 2D points for Homography
    // estimation.
    cv::Mat world_points(4, 2, CV_64F), image_points(4, 2, CV_64F);
    // In order to conver cv::Point2f data to cv::Mat use reshape
    cv::Mat corner_matrix = cv::Mat(corner_vector).reshape(1);
    // Always use copyTo for safe conversions
    // https://stackoverflow.com/questions/24991307/opencv-vectorpoint3f-to-3-column-mat
    corner_matrix.row(0).copyTo(image_points.row(0));
    corner_matrix.row(pattern_size.height-1).copyTo(image_points.row(1));
    corner_matrix.row(total_indices-1).copyTo(image_points.row(2));
    corner_matrix.row(total_indices-1-pattern_size.height)
                 .copyTo(image_points.row(3));

    MatrixXf _world_points(4, 2);
    _world_points << square_size, square_size,
                    square_size*pattern_size.height, square_size,
                    square_size*pattern_size.height,
                    square_size*pattern_size.width,
                    square_size, square_size*pattern_size.width;

    cv::eigen2cv(_world_points, world_points);

    cv::Mat H = get_homography(world_points, image_points);
    if (H.empty()) {
        cout << "WARNING: H matrix is empty!\n";
    }
    // MatrixXf V = create_V_matrix(H);
    MatrixXf V;
    V.resize(2, 6);
    V = create_V_matrix(H);
    cout << "V matrix: " << V << std::endl;
    return V;
}

MatrixXf create_V_matrix(const cv::Mat& H) {
    // Gather V matrix
    MatrixXf v11(1, 6);
    get_vij_matrix(v11, H, 0, 0);
    MatrixXf v12(1, 6);
    get_vij_matrix(v12, H, 0, 1);
    MatrixXf v22(1, 6);
    get_vij_matrix(v22 , H, 1, 1);
    MatrixXf V(2, 6);
    V << v12,
        (v11-v22);
    return V;
}

void get_vij_matrix(MatrixXf& vij, const cv::Mat& H, int i, int j) { //NOLINT
    // findHomography returns data of type double, not float!
    vij <<  H.at<double>(i, 0)*H.at<double>(j, 0),
            H.at<double>(i, 0)*H.at<double>(j, 1) +
            H.at<double>(i, 1)*H.at<double>(j, 0),
            H.at<double>(i, 1)*H.at<double>(j, 1),
            H.at<double>(i, 2)*H.at<double>(j, 0) +
            H.at<double>(i, 0)*H.at<double>(j, 2),
            H.at<double>(i, 2)*H.at<double>(j, 1) +
            H.at<double>(i, 1)*H.at<double>(j, 2),
            H.at<double>(i, 2)*H.at<double>(j, 2);
}

MatrixXf get_initial_K(const MatrixXf& V) {
    JacobiSVD<MatrixXf> svd(V, ComputeThinU | ComputeFullV);
    //   cout << "The right singular values of V are " << std::endl;
    //   cout << svd.matrixV() << std::endl;
    MatrixXf rt_eigen_matrix(6, 6);
    rt_eigen_matrix = svd.matrixV();
    auto B = get_B_matrix(rt_eigen_matrix);
    auto K = compute_K(B);
    cout << "K is " << K << std::endl;
    return K;
}

MatrixXf get_B_matrix(const MatrixXf& rt_eigen_matrix) {
    // extract last column of the right eigen matrix
    VectorXf b(6);
    b = rt_eigen_matrix.col(5);

    // another way to define a (3, 3) matrix
    Matrix3f B;
    B << b(0), b(1), b(3),
         b(1), b(2), b(4),
         b(3), b(4), b(5);

    return B;
}

Matrix3f compute_K(const MatrixXf& B) {
    // initialize K matrix
    Matrix3f K;

    // calculate y coordinate of principal point
    double v0 = (B(0, 1) * B(0, 2) - B(0, 0) * B(1, 2))
                / (B(0, 0)*B(1, 1) - std::pow(B(0, 1), 2));

    // calculate lambda
    double lam = B(2, 2) - (std::pow(B(0, 2), 2)
                 + v0 * (B(0, 1) * B(0, 2) - B(0, 0) * B(1, 2))) / B(0, 0);

    // calculate focal length in x direction
    double fx =  (lam / B(0, 0));
    cout << "v0 is " << v0 << std::endl;

    // calculate focal length in y direction
    double fy = std::pow((lam *B(0, 0)/(B(0, 0) * B(1, 1) -
                                    std::pow(B(0, 1), 2))), 0.5);

    // calculate skew term gamma
    double gamma = - B(0, 1)*std::pow(fx, 2) * fy / lam;

    // calculate x coordinate of principal point
    double u0 = (gamma * v0 / fy) - (B(0, 2) * std::pow(fx, 2) / lam);

    K << fx, gamma, u0,
          0,    fy, v0,
          0,     0,  1;

    return K;
}
