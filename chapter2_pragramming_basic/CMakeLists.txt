#required minimum version
cmake_minimum_required(VERSION 2.8)

#declare a project
project(HelloSLAM)

# 设置编译模式
set( CMAKE_BUILD_TYPE "Debug" )

#add excutable programm
# Sytax: add_excutable(programm_name source_file)
add_executable(helloSALM hello_world.cpp)

#add static lib which would generate a copy every time this function was called
add_library(hello libHelloSLAM.cpp)
# add shared lib
add_library(hello_shared SHARED libHelloSLAM.cpp)

add_executable(useHello useHello.cpp)
# 将库文件链接到可执行程序上
target_link_libraries(useHello hello_shared)

