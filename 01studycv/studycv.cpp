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

    int value = 100;
    namedWindow("edge_canny", WINDOW_NORMAL);
    createTrackbar("threshold", "edge_canny", &value, 500);

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        // resize(frame, frame, Size(), 0.5, 0.5);

        Mat resultG, edge_canny;
        GaussianBlur(frame, resultG, Size(3, 3), 5);
        Canny(resultG, edge_canny, value, value*2, 3);
        
        imshow("frame", frame);
        imshow("edge_canny", edge_canny);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
