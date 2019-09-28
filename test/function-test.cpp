/**@file function-test.cpp
 * @brief Standard utilities header file
 * 
 * Detailed description follows here.
 * @author     : Abhinav Modi, Kartik Madhira
 * o
 * Copyright 2019 MIT License
 */

#include "gtest/gtest.h"
#include "../src/Util.h"
#include "../src/get_initial_calib.hpp"

TEST(get_images, greater_than_2) {
  EXPECT_GT(get_images_path("../checkerboard_images").size() , 2);
}  // should pass
