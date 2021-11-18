#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

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
        std::vector<Mat> Gauss, Lap;
        int level = 3;
        Gauss.push_back(frame);

        //构建高斯金字塔
        for(int i = 0; i < level; i++)
        {
            Mat gauss;
            pyrDown(Gauss[i], gauss);
            Gauss.push_back(gauss);        
        }
        //构建拉普拉斯金字塔
        for(int i = Gauss.size() - 1; i > 0; i--)
        {
            Mat lap, upGauss;
            if(i == Gauss.size() - 1)       //如果是高斯金字塔最上层的图像
            {
                Mat down;
                pyrDown(Gauss[i], down);
                pyrUp(down, upGauss);
                lap = Gauss[i] - upGauss;
                Lap.push_back(lap);
            }
            pyrUp(Gauss[i], upGauss);
            lap = Gauss[i - 1] - upGauss;
            Lap.push_back(lap);
        }

        for(int i = 0; i < Gauss.size(); i++)
        {
            std::string name = std::to_string(i);
            imshow("G" + name, Gauss[i]);
            imshow("L" + name, Lap[i]);
        }

        imshow("Frame", frame);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    return 0;
}

