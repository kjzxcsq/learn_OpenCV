cmake_minimum_required(VERSION 2.6)

project(studycv)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenCV 4.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(studycv studycv.cpp)
target_link_libraries(studycv ${OpenCV_LIBS})
