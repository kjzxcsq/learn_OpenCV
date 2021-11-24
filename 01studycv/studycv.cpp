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
        imshow("frame", frame);

        //转成二值图像，用于统计连通域
        Mat imgG, imgBW, imgE, imgD;
        cvtColor(frame, imgG, COLOR_BGR2GRAY);
        threshold(imgG, imgBW, 150, 255, THRESH_BINARY);

        Mat struct1 = getStructuringElement(0, Size(3, 3));         //矩形结构元素
        Mat struct2 = getStructuringElement(1, Size(3, 3));         //十字结构元素
        //腐蚀
        erode(imgBW, imgE, struct1);
        //膨胀
        dilate(imgBW, imgD, struct2);

        //生成随机颜色， 区分不同连通域
        RNG rng(142857);
        Mat out, stats, centroids;
        //统计连通域数目
        int number = connectedComponentsWithStats(imgE, out, stats, centroids, 8, CV_16U);
        std::vector<Vec3b> colors;
        for(int i = 0; i < number; i++)
        {
            //使用均匀分布的随机数确定颜色
            Vec3b vec3 = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            colors.push_back(vec3);
        }

        //用不同颜色标记连通域
        Mat result = Mat::zeros(imgG.size(), imgG.type());
        int w = result.cols;
        int h = result.rows;
        for(int i = 1; i < number; i++)
        {
            //中心位置
            int center_x = centroids.at<double>(i, 0);
            int center_y = centroids.at<double>(i, 1);
            //矩形边框
            int x = stats.at<int>(i, CC_STAT_LEFT);
            int y = stats.at<int>(i, CC_STAT_TOP);
            int w = stats.at<int>(i, CC_STAT_WIDTH);
            int h = stats.at<int>(i, CC_STAT_HEIGHT);
            int area = stats.at<int>(i, CC_STAT_AREA);

            //中心位置绘制
            circle(frame, Point(center_x, center_y), 2, Scalar(0, 255, 0), 2);
            //外切矩形
            Rect rect(x, y, w, h);
            rectangle(frame, rect, colors[i], 1);

            putText(frame, format("%d", i), Point(center_x, center_y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
            cout << "number:" << i << ", area:" << area << endl;
        }
        imshow("imgBW", imgBW);
        imshow("imgE", imgE);
        imshow("imgD", imgD);
        imshow("result", frame);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}
