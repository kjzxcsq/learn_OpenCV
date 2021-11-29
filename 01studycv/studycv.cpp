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

    int line_threshold = 150;           //累加器阈值
    int minlength = 30;                 //直线最小长度
    int maxgap = 10;                    //两点间最大距离
    namedWindow("find_lines", WINDOW_NORMAL);
    createTrackbar("line_threshold", "find_lines", &line_threshold, 500);
    createTrackbar("minlength", "find_lines", &minlength, 300);
    createTrackbar("maxgap", "find_lines", &maxgap, 100);

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        resize(frame, frame, Size(), 0.5, 0.5);

        Mat edge;
        //检测边缘并二值化
        Canny(frame, edge, 80, 180);
        threshold(edge, edge, 170, 255, THRESH_BINARY);
        imshow("edge", edge);

        //渐进概率式霍夫变换提取直线
        std::vector<Vec4i> linesP;
        HoughLinesP(edge, linesP, 1, CV_PI / 180, line_threshold, minlength, maxgap);

        //绘制直线
        for(size_t i = 0; i < linesP.size(); i++)
        {
            line(frame, Point(linesP[i][0], linesP[i][1]), Point(linesP[i][2], linesP[i][3]), Scalar(255), 3);
        }
        imshow("find_lines", frame);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
