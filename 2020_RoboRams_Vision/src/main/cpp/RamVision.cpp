#include "RamVision.h"


using namespace std;
RamVision::RamVision()
{
    x = 0;
    y = 0;
}

UCHAR RamVision::GetPixel(cv::Mat inMat, int row, int col)
{
	col = (col > 0) ? col - 1 : 0;
	unsigned char* pd = inMat.ptr(row, col);
	UCHAR pixel = 0;
	if (pd[0] > 0)
		pixel = pd[0];
	else if (pd[1] > 0)
		pixel = pd[1];
	else
		pixel = pd[2];

	return pixel;
}


bool RamVision::GetBoxCoordinates(Mat input, int& topLeftX, int& topLeftY, int& sizex, int& sizey, int rowOffset = 0)
{
	bool returnValue = false;

	//bool foundFirst = false;
	bool foundSecond = false;
	int firstCount = 0;
	const int MAX_FIRST_COUNT = 200;
	const int SIZEY_OFFSET = 60;
	for (int row = (input.rows-1) - rowOffset; row > 0; row--)
	{
		for (int col = 0; col < input.cols; col++)
		{
			unsigned char pixel = 0;
			pixel = GetPixel(input, row, col);
			// look for an 'on' pixel.
			if (pixel > 0 )
			{
				
				// is this our first detection?
				if (1)//!foundFirst)
				{
					// in case the tape isn'ta perfect flat line, we want to get at the meat of it.
					if (firstCount < MAX_FIRST_COUNT)
					{
						++firstCount;
						
						
					}
					else
					{
						// found our first detection, time to look for the second line..

						//foundFirst = true;
						int bottomLeftX = col;
						int bottomLeftY = row -130;
						// Skip ahead, we don't want to detect the same object.
						while ((pixel > 0) && col < input.cols - 1)
						{
							
							pixel = GetPixel(input, row , ++col );
						}
						topLeftX = bottomLeftX;
						topLeftY = bottomLeftY + SIZEY_OFFSET;
						sizex = col - bottomLeftX;
						sizey = SIZEY_OFFSET;
						//now we are done
						return true;
					}
				}
				
			}
		}

	}

	return returnValue;
}


void RamVision::UpdateFrame(const Mat frame) 
{
    //currentFrame_(frame);
    //processedFrame_(frame);
	frame.copyTo(currentFrame_);
	frame.copyTo(processedFrame_);
};


bool RamVision::ProcessCurrentFrame(int &x, int &y, Mat &frame)
{
	if (!currentFrame_.empty())
	{
		Mat hsv, hsv1, blur;
		cvtColor(currentFrame_, hsv, COLOR_BGR2HSV);
		inRange(hsv, Scalar(0, 164, 226), Scalar(180, 255, 255), hsv1);
		//GaussianBlur(hsv1, blur, Size(21, 21), 0, 0);
		//bilateralFilter(hsv1, blur, 13, 80, 80);
		int sizex = 0;
		int sizey = 0;
		//GetBoxCoordinates(hsv1, x, y, sizex, sizey);
		//x = sizex / 2;
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(hsv1, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		int largestContour = 0;
		int largestContourIdx = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			if (contours[i].size() > largestContour)
			{
				largestContourIdx = i;
				largestContour = contours[i].size();
			}
		}
		x = 0;
		y = 0;
		if ((largestContour > 0)&&(largestContour < 50))
		{
			// we have the index of the tape vector array.
			int smallestX = INT32_MAX;
			int smallestY = INT32_MAX;
			int largestX = 0;
			int largestY = 0;
			for (int i = 0 ; i < largestContour; ++i)
			{
				x = contours[largestContourIdx][i].x;
				y = contours[largestContourIdx][i].y;
				if (x < smallestX)
					smallestX = x;
				if (x > largestX)
					largestX = x;
				if (y < smallestY)
					smallestY = y;
				if (y > largestY)
					largestY = y;
				
			}
			x = largestX - smallestX;
			y = largestY - smallestY;
			x = static_cast<int>(static_cast<double>(x) / 2.0f);
			y = static_cast<int>(static_cast<double>(y) / 2.0f);
			Mat drawing = Mat::zeros(hsv1.size(), CV_8UC3);
			RNG rng(12345);
			Scalar color = Scalar(rng.uniform(0, 0), rng.uniform(0, 255), rng.uniform(0, 0));
			drawContours(drawing, contours, largestContourIdx, color, 2, 8, hierarchy, 0, Point());
			this->processedFrame_ = drawing;
			
				return true;
		}
		
	}
	
    return false;
}

/* void RamVision::DetermineRobotPivot(int circle_col, int img_width, int &delta_correction)
{
	int middle_width = static_cast<int>(static_cast<double>((img_width-1) / 2.0f));
	delta_correction = circle_col - middle_width;
}
*/




RamVision::~RamVision()
{
    currentFrame_.release();
    processedFrame_.release();
    
}