cmake_minimum_required( VERSION 2.8 )


set(CMAKE_CXX_FLAGS "-std=c++11")
set( CMAKE_BUILD_TYPE "Dedug" )

project(PCL_SLAM)


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# eigen
include_directories("/usr/include/eigen3")

#pcl
find_package(PCL REQUIRED COMPONENT common io)
include_directories(${PCL_INCLUDE_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(joinMap joinMap.cpp)
target_link_libraries(joinMap ${OpenCV_LIBS} ${PCL_LIBRARIES})
