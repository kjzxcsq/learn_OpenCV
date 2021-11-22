#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::cout; using std::cerr; using std::endl;

int main(int, char**)
{
    Mat frame;
    cout << "Opening camera..." << endl;
    VideoCapture capture(0); // open the first camera
    if (!capture.isOpened())
    {
        cerr << "ERROR: Can't initialize camera capture" << endl;
        return 1;
    }

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        resize(frame, frame, Size(), 0.5, 0.5);
        Mat kernel1 = (Mat_<float>(1, 2) << 1, -1);         //X方向边缘检测滤波器
        Mat kernel2 = (Mat_<float>(1, 3) << 1, 0, -1);      //X方向边缘检测滤波器
        Mat kernel3 = (Mat_<float>(3, 1) << 1, 0, -1);      //Y方向边缘检测滤波器
        Mat kernelXY = (Mat_<float>(2, 2) << 1, 0, 0, -1);  //左上到右下边缘检测滤波器
        Mat kernelYX = (Mat_<float>(2, 2) << 0, -1, 1, 0);  //右上到左下边缘检测滤波器
        Mat result1, result2, result3, result4, result5, result6;

        //以[1 -1]检测水平方向
        filter2D(frame, result1, CV_16S, kernel1);
        convertScaleAbs(result1, result1);

        //以[1 0 -1]检测水平方向
        filter2D(frame, result2, CV_16S, kernel2);
        convertScaleAbs(result2, result2);

        //以[1 0 -1]检测垂直方向
        filter2D(frame, result3, CV_16S, kernel3);
        convertScaleAbs(result3, result3);

        result6 = result2 + result3;        //整幅图像的边缘

        //检测从左上到右下的边缘
        filter2D(frame, result4, CV_16S, kernelXY);
        convertScaleAbs(result4, result4);

        //检测从右上到左下的边缘
        filter2D(frame, result5, CV_16S, kernelYX);
        convertScaleAbs(result5, result5);

        imshow("result1", result1);
        imshow("result2", result2);
        imshow("result3", result3);
        imshow("result4", result4);
        imshow("result5", result5);
        imshow("result6", result6);
        
        imshow("frame", frame);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
