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
        imshow("frame", frame);
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        imshow("gray", frame);
        threshold(frame, frame, 100, 255, THRESH_BINARY);
        imshow("thresh", frame);
        
        //5*5矩形结构元素
        Mat kernel = getStructuringElement(0, Size(5, 5));
        Mat open, close, gradient, tophat, blackhat, hitmiss;

        //开运算
        morphologyEx(frame, open, MORPH_OPEN, kernel);
        imshow("open", open);

        //闭运算
        morphologyEx(frame, close, MORPH_CLOSE, kernel);
        imshow("close", close);

        //形态学梯度运算
        morphologyEx(frame, gradient, MORPH_GRADIENT, kernel);
        imshow("gradient", gradient);

        //顶帽运算
        morphologyEx(frame, tophat, MORPH_TOPHAT, kernel);
        imshow("tophat", tophat);

        //黑帽运算
        morphologyEx(frame, blackhat, MORPH_BLACKHAT, kernel);
        imshow("blackhat", blackhat);

        //击中击不中变换
        morphologyEx(frame, hitmiss, MORPH_HITMISS, kernel);
        imshow("hitmiss", hitmiss);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
