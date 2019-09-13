cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Call cmake with -D TESTS=ON to set this flag to true.
option(TESTS "build tests" OFF)

project(camera_calibration CXX C)

# Core and main are split. This allows us to link core to main and tests.

# Core library. *.cpp should be added here.
add_library(core
  ./src/core.cpp
  ./src/get_initial_calib.h)

# Main entry point.
add_executable(main
  ./src/main.cpp)

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS system filesystem)

include_directories( ${OpenCV_INCLUDE_DIRS} $ENV{EIGEN3_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})

# Link core to main.
target_link_libraries(main
  core
  ${OpenCV_LIBS}
  ${Boost_LIBRARIES})

# Add flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

if(TESTS)

  enable_testing()

  # Process the CMakeLists.txt in third-party/googletest, sets project(gtest) and
  # project(gmock), gives access to ${gtest_SOURCE_DIR} and ${gmock_SOURCE_DIR}.
  add_subdirectory(third-party/googletest)

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

  # Tests. *-test.cpp should be added here.
  add_executable(tests
    ./test/main-test.cpp
    ./test/function-test.cpp
    ./test/class-test.cpp)

  # Link core, pthread and gtest to tests.
  target_link_libraries(tests
    pthread
    gtest
    gtest_main
    core)

endif()
