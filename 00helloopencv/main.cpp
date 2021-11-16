#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int agrc, char** agrv){
    Mat img=imread("ubuntu.jpg");
    imshow("hello OpenCV",img);
    waitKey(0);
    return 0;
}