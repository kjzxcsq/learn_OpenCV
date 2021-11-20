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
    normalize(hist, hist, 255, 0, type, -1, Mat());
    // for(int i = 1; i <= hist.rows; i++)
    //     {
    //         rectangle(histImage, Point(width*(i - 1), hist_h - 1),
    //             Point(width*i - 1, hist_h - cvRound(15*hist_h*hist.at<float>(i - 1)) - 1),
    //             Scalar(255, 255, 255), -1);
    //     } 
    namedWindow(name, WINDOW_NORMAL);
    imshow(name, hist);
}
int main(int, char**)
{
    Mat img = imread("img.jpg");
    if(img.empty())
    {
        cout << "image dose not exicisted" << endl;
        return -1;
    }
    Mat sub_img, img_HSV, sub_HSV, hist, hist2;
    img(Range(250, 300), Range(150, 200)).copyTo(sub_img);
    imshow("img", img);
    imshow("sub_img", sub_img);

    cvtColor(img, img_HSV, COLOR_BGR2HSV);
    cvtColor(sub_img, sub_HSV, COLOR_BGR2HSV);
    int h_bins = 32, s_bins = 32;
    int histSize[] = {h_bins, s_bins};
    float h_ranges[] = {0, 180};        //H通道范围
    float s_ranges[] = {0, 256};        //S通道范围
    const float* ranges[] = {h_ranges, s_ranges};       //每个通道范围
    int channels[] = {0, 1};
    //绘制H-S二维直方图
    calcHist(&sub_HSV, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
    drawHist(hist, NORM_INF, "hist");
    Mat backproj;
    calcBackProject(&img_HSV, 1, channels, hist, backproj, ranges, 1.0);
    imshow("backproj", backproj);

    waitKey(0);
    return 0;
}
