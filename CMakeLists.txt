cmake_minimum_required(VERSION 3.10)
# set the project name
project(vslam)
find_package(OpenCV 4.3 REQUIRED)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# add the executable
add_executable(slam_camera main.cpp
                            camera.cpp filter.cpp match.cpp
                            videorw.cpp RobustMatcher.cpp)

target_link_libraries(slam_camera opencv_core
                                  opencv_videoio
                                  opencv_highgui
                                  opencv_imgproc
                                  opencv_features2d
                                  opencv_calib3d
                                  pthread)
