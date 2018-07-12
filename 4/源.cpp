
// 图像增强—直方图均衡化

//opencv
#include <highgui.h>  
#include<cv.h>  
#include <cvaux.h>  
#include <opencv\cxcore.hpp>  
#include <opencv.hpp>  
//#include <nonfree.hpp>  
#include <core/core.hpp>  


#include <iostream>  
#include <opencv2/opencv.hpp>  
#include <vector>  
using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
	//IplImage* img1 = cvLoadImage("input.jpg");

	Mat input_image = imread("input.jpg", 1);
	Mat dlphay_outpou;
	Mat opencv_ouput;


	//灰度图 
	cvtColor(input_image, input_image, CV_RGB2GRAY);
	threshold(input_image, input_image, 65, 255, THRESH_BINARY);
	namedWindow("原图");
	imshow("原图", input_image);


	// opencv闭运算  即先膨胀，再腐蚀
	Mat element(4, 4, CV_8U, Scalar(1));
	Mat closed;
	morphologyEx(input_image, closed, MORPH_CLOSE, element);
	namedWindow("闭运算");
	imshow("闭运算", closed);

	// opencv开运算  即先腐蚀，再膨胀
	Mat opened;
	morphologyEx(input_image, opened, MORPH_OPEN, element);
	namedWindow("开运算");
	imshow("开运算", opened);

	waitKey(0);

	return 0;
}