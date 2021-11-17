#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
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

        circle(frame, Point(50, 50), 25, Scalar(255, 255, 255), -1);
        circle(frame, Point(100, 50), 30, Scalar(0, 0, 255), 1);
        line(frame, Point(100, 100), Point(200, 150), Scalar(255, 0, 0), 2, LINE_8, 0);
        rectangle(frame, Rect(400, 500, 70, 60), Scalar(0, 125, 125), 2);
        putText(frame, "Learn draw", Point(200, 300), 2, 1, Scalar(0, 255, 0));
        
        imshow("Frame", frame);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}

