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

    namedWindow("find_circles", WINDOW_NORMAL);
    std::vector<Vec3f> circles;
    double dp = 2;
    int mindist = 10;        //两圆心最小距离
    int param1 = 100;        //Canny检测的较大阈值
    int param2 = 100;        //累加器阈值
    int min_radius = 20;        //半径最小值
    int max_radius =  100;      //半径最大值
    createTrackbar("mindist", "find_circles", &mindist, 300);
    createTrackbar("param2", "find_circles", &param2, 300);
    createTrackbar("min_radius", "find_circles", &min_radius, 300);
    createTrackbar("max_radius", "find_circles", &max_radius, 600);

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        resize(frame, frame, Size(), 0.5, 0.5);

        cvtColor(frame, frame, COLOR_BGR2GRAY);

        HoughCircles(frame, circles, HOUGH_GRADIENT, dp, mindist, param1, param2, min_radius, max_radius);
        for(size_t i = 0; i < circles.size(); i++)
        {
            //圆心      cvRound 浮点数四舍五入取整
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            //半径
            int radius = cvRound(circles[i][2]);
            //绘制
            circle(frame, center, 3, Scalar(0, 255, 0), -1);
            circle(frame, center, radius, Scalar(255, 0, 0), 3);
        }
        imshow("find_circles", frame);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
