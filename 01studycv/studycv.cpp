#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::cout; using std::cerr; using std::endl;

//为了变量在回调函数中使用，声明为全局
int value;
void callback(int, void*);
Mat frame, img;
int main(int, char**)
{
    static Mat frame;
    cout << "Opening camera..." << endl;
    VideoCapture capture(2); // open the first camera
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
        resize(frame, frame, Size(1080, 720), 0, 0, INTER_AREA);
        namedWindow("camera", WINDOW_NORMAL);
        value = 100;
        createTrackbar("亮度百分百", "camera", &value, 500, callback, 0);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}

void callback(int, void*)
{
    float a = value / 100;
    Mat img = frame * a;
    imshow("camera", img);
    cout << "cb" << endl;
    return;
}
