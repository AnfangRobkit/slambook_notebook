cmake_minimum_required( VERSION 2.8 )
set(CMAKE_CXX_FLAGS "-std=c++11")

project(Imgae_SLAM)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(image_bascis imageBasics.cpp)
target_link_libraries(image_bascis ${OpenCV_LIBS})


