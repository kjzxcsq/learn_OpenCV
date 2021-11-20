#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::cout; using std::cerr; using std::endl;

void drawHist(Mat &hist, int type, std::string name)         //归一化并绘制直方图
{
    int hist_w = 512;
    int hist_h = 400;
    int width = 2;
    Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);
    normalize(hist, hist, 1, 0, NORM_L1, -1, Mat());
    for(int i = 1; i <= hist.rows; i++)
        {
            rectangle(histImage, Point(width*(i - 1), hist_h - 1),
                Point(width*i - 1, hist_h - cvRound(15*hist_h*hist.at<float>(i - 1)) - 1),
                Scalar(255, 255, 255), -1);
        } 
    imshow(name, histImage);
}
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

    Mat img = imread("img.jpg");
    if(img.empty())
    {
        cout << "image dose not exicisted" << endl;
        return -1;
    }
    Mat gray, gray2, gray3, hist, hist2, hist3;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    resize(gray, gray2, Size(), 0.5, 0.5);
    gray3 = imread("img1.jpg", IMREAD_GRAYSCALE);
    const int channels[1] = {0};        //通道索引
    float inRanges[2] = {0, 255};
    const float* ranges[1] = {inRanges};//像素灰度值范围
    const int bins[1] = {256};          //直方图的维度，其实就是像素灰度值的最大值
    calcHist(&gray, 1, channels, Mat(), hist, 1, bins, ranges);
    calcHist(&gray2, 1, channels, Mat(), hist2, 1, bins, ranges);
    calcHist(&gray3, 1, channels, Mat(), hist3, 1, bins, ranges);
    drawHist(hist, NORM_INF, "hist");
    drawHist(hist2, NORM_INF, "hist2");
    drawHist(hist3, NORM_INF, "hist3");

    double hist_hist = compareHist(hist, hist, HISTCMP_CORREL);             //原图直方图与原图直方图的相关系数
    cout << "hist_hist=" << hist_hist << endl;
    double hist_hist2 = compareHist(hist, hist2, HISTCMP_CORREL);           //原图直方图与1/2原图直方图的相关系数
    cout << "hist_hist2=" << hist_hist2 << endl;
    double hist_hist3 = compareHist(hist, hist3, HISTCMP_CORREL);           //不同图像直方图的相关系数
    cout << "hist_hist3=" << hist_hist3 << endl;
    int key = waitKey(0);
    return 0;
}
