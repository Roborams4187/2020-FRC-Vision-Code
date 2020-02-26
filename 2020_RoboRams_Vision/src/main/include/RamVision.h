#ifndef RAMVISION_H 
#define RAMVISION_H 1
#include <opencv2/opencv.hpp>
#include <vector>
#include "cameraserver/CameraServer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <string>

using namespace cv;
typedef unsigned char UCHAR;
class RamVision
{
    public:
    RamVision();
    static UCHAR GetPixel(cv::Mat inMat, int row, int col);
    bool GetBoxCoordinates(Mat input, int& topLeftX, int& topLeftY, int& sizex, int& sizey, int rowOffset );
    void UpdateFrame(const Mat frame);
    bool ProcessCurrentFrame(int &x, int &y, Mat &frame);
    //static void VisionThread();
    //void RamVision::DetermineRobotPivot(int circle_col, int img_width, int &delta_correction);
    ~RamVision();
    //private:
    Mat currentFrame_,processedFrame_;
    int x,y;
    
};






#endif