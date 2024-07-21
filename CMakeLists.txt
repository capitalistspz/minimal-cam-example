cmake_minimum_required(VERSION 3.28)
project(minimal_camera_example)

set(CMAKE_CXX_STANDARD 20)

add_executable(minimal_camera_example main.cpp
        altered.h)

wut_create_rpx(minimal_camera_example)