cmake_minimum_required(VERSION 2.8)
project(g2o_SLAM)

set(CMAKE_CXX_FLAGS "-std=c++11 -o3")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package( G2O REQUIRED )
include_directories( ${G2O_INCLUDE_DIRS} "/usr/include/eigen3")

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(g2o_curve g2o_curve_fitting.cpp)
target_link_libraries(g2o_curve ${OpenCV_LIBS} g2o_core g2o_stuff)
