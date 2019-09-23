// Copyright 2019

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
    V.resize(12, 2);
    V = create_V_matrix(H);
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
    cout << H << "\n";
    V << v12,
        (v11-v12);
    return V;
}

void get_vij_matrix(MatrixXf& vij, const cv::Mat& H, int i, int j) { //NOLINT
    // vij << H.at<float>(0,0);
    // cout << "value here is!!!!!" << H.at<float>(0,1);
    cout << H.at<float>(i, 0)*H.at<float>(j, 0) << "---------\n";
    vij <<  H.at<float>(i, 0)*H.at<float>(j, 0),
            H.at<float>(i, 0)*H.at<float>(j, 1) +
            H.at<float>(i, 1)*H.at<float>(j, 0),
            H.at<float>(i, 1)*H.at<float>(j, 1),
            H.at<float>(i, 2)*H.at<float>(j, 0) +
            H.at<float>(i, 0)*H.at<float>(j, 2),
            H.at<float>(i, 2)*H.at<float>(j, 1) +
            H.at<float>(i, 1)*H.at<float>(j, 2),
            H.at<float>(i, 2)*H.at<float>(j, 2);
}

cv::Mat get_initial_K(cv::Mat homography) {
}
