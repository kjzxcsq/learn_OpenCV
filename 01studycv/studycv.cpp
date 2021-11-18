#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::cout; using std::cerr; using std::endl;

int main(int, char**)
{
    std::vector<double> positiveData = {2.0, 8.0, 10.0};
    std::vector<double> normalized_L1, normalized_L2, normalized_Inf, normalized_L2SQR;     //不同归一化方法
    normalize(positiveData, normalized_L1, 1.0, 0.0, NORM_L1);              //绝对值求和归一化
    cout << "normalized_L1=[" << normalized_L1[0] << ", " << normalized_L1[1] << ", " << normalized_L1[2] << "]" << endl;
    normalize(positiveData, normalized_L2, 1.0, 0.0, NORM_L2);              //模长归一化
    cout << "normalized_L2=[" << normalized_L2[0] << ", " << normalized_L2[1] << ", " << normalized_L2[2] << "]" << endl;
    normalize(positiveData, normalized_Inf, 1.0, 0.0, NORM_INF);            //最大值归一化
    cout << "normalized_Inf=[" << normalized_Inf[0] << ", " << normalized_Inf[1] << ", " << normalized_Inf[2] << "]" << endl;
    normalize(positiveData, normalized_L2SQR, 1.0, 0.0, NORM_MINMAX);
    cout << "normalized_MINMAX=[" << normalized_L2SQR[0] << ", " << normalized_L2SQR[1] << ", " << normalized_L2SQR[2] << "]" << endl;

    Mat frame, gray;
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
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        resize(gray, gray, Size(640, 480), 0, 0);

        Mat hist;                           //存放直方图计算结果
        const int channels[1] = {0};        //通道索引
        float inRanges[2] = {0, 255};
        const float* ranges[1] = {inRanges};//像素灰度值范围
        const int bins[1] = {256};          //直方图的维度，其实就是像素灰度值的最大值
        calcHist(&gray, 1, channels, Mat(), hist, 1, bins, ranges);

        //绘制直方图
        int hist_w = 512;
        int hist_h = 400;
        int width = 2;
        Mat histImage_L1 = Mat::zeros(hist_h, hist_w, CV_8UC3);
        Mat histImage_Inf = Mat::zeros(hist_h, hist_w, CV_8UC3);
        Mat hist_L1, hist_Inf;
        normalize(hist, hist_L1, 1, 0, NORM_L1, -1, Mat());
        for(int i = 1; i <= hist_L1.rows; i++)
        {
            rectangle(histImage_L1, Point(width*(i - 1), hist_h - 1),
                Point(width*i - 1, hist_h - cvRound(15*hist_h*hist_L1.at<float>(i - 1)) - 1),
                Scalar(255, 255, 255), -1);
        } 
        normalize(hist, hist_Inf, 1, 0, NORM_INF, -1, Mat());
        for(int i = 1; i <= hist_Inf.rows; i++)
        {
            rectangle(histImage_Inf, Point(width*(i - 1), hist_h - 1),
                Point(width*i - 1, hist_h - cvRound(15*hist_h*hist_L1.at<float>(i - 1)) - 1),
                Scalar(255, 255, 255), -1);
        }
        imshow("histImage_L1", histImage_L1);   
        imshow("histImage_Inf", histImage_Inf);  
        imshow("gray", gray);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
