cmake_minimum_required(VERSION 2.6)

project(helloopencv)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenCV 4.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(helloopencv main.cpp)
target_link_libraries(helloopencv ${OpenCV_LIBS})
