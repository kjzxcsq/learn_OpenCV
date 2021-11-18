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
        
        Mat frame_copy, frame_deepcopy, frame_deepcopy2;
        frame_copy = frame;
        frame.copyTo(frame_deepcopy);
        copyTo(frame, frame_deepcopy2, frame);

        Rect rect(200, 200, 400, 400);
        Mat ROI1, ROI2, ROI_deepcopy;
        ROI1 = frame(rect);
        ROI2 = frame(Range(100, 500), Range(200, 400));
        frame(Range(100, 500), Range(200, 400)).copyTo(ROI_deepcopy);
        circle(frame, Point(300, 300), 200, Scalar(0, 0, 255), -1);

        imshow("frame", frame);
        imshow("frame_copy", frame_copy);
        imshow("frame_deepcopy", frame_deepcopy);
        imshow("frame_deepcopy2", frame_deepcopy2);
        imshow("ROI1", ROI1);
        imshow("ROI2", ROI2);
        imshow("ROI_deepcopy", ROI_deepcopy);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
