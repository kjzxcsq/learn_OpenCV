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
        Mat mean_filter_3, mean_filter_9;
        blur(frame, mean_filter_3, Size(3, 3));
        blur(frame, mean_filter_9, Size(9, 9));
        
        imshow("Frame", frame);
        imshow("mean_filter_3", mean_filter_3);
        imshow("mean_filter_9", mean_filter_9);

        Mat bilateral1, bilateral2, bilateral3, bilateral4;
        bilateralFilter(frame, bilateral1, 5, 50, 25 / 2);
        bilateralFilter(frame, bilateral2, 25, 50, 25 / 2);
        bilateralFilter(frame, bilateral3, 5, 9, 9);
        bilateralFilter(frame, bilateral4, 5, 200, 200);

        imshow("bilateral1", bilateral1);
        imshow("bilateral2", bilateral2);
        imshow("bilateral3", bilateral3);
        imshow("bilateral4", bilateral4);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
