cmake_minimum_required(VERSION 2.8)
project(vo1_2d2d)

set(CMAKE_BUILD_TYPE "Realse")
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")

find_package( OpenCV 4.0.0 REQUIRED )

include_directories( 
    ${OpenCV_INCLUDE_DIRS} )


add_executable( pose_estimation_2d2d pose_estimation_2d2d.cpp )
target_link_libraries( pose_estimation_2d2d ${OpenCV_LIBS} )
