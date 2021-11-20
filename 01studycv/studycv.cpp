#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::cout; using std::cerr; using std::endl;

int main(int, char**)
{
    Mat img = imread("img.jpg");
    if(img.empty())
    {
        cout << "image dose not exist" << endl;
        return -1;
    }
    Mat temp;
    img(Range(90, 140), Range(210, 260)).copyTo(temp);

    Mat result;
    matchTemplate(img, temp, result, TM_CCOEFF_NORMED);

    double maxVal, minVal;
    Point maxLoc, minLoc;
    //寻找匹配结果最大值和最小值及坐标位置
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
    //绘制最佳匹配区域
    rectangle(img, Rect(maxLoc.x, maxLoc.y, temp.cols, temp.rows), Scalar(0, 255, 0), 2);
    //绘制最不佳匹配区域
    rectangle(img, Rect(minLoc.x, minLoc.y, temp.cols, temp.rows), Scalar(0, 0, 255), 2);

    imshow("img", img);
    imshow("temp", temp);
    imshow("result", result);

    waitKey(0);
    return 0;
}
