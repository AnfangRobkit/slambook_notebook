#required minimum version
cmake_minimum_required(VERSION 2.8)

#declare a project
project(Motion_SLAM)

include_directories("/usr/include/eigen3")

add_executable(eigen_trial eigenMatrix.cpp)

