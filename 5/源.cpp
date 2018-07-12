
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

	//图片保存参数
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(9);

	cv::Mat src = cv::imread("123.jpg", 1);

	imshow("原图", src);
	Mat binary = Mat::zeros(src.size(), CV_8UC1);

	//色彩分割得到二值图像
	for (int ii = 0; ii<src.rows; ii++)
		for (int jj = 0; jj<src.cols; jj++) {
			int b = (int)(src.at<Vec3b>(ii, jj)[0]);
			int r = (int)(src.at<Vec3b>(ii, jj)[2]);
			if (r>150 & b<100) {
				binary.at<uchar>(ii, jj) = 255;
			}
			else binary.at<uchar>(ii, jj) = 0;
		}

	cv::imshow("阈值分割", binary);
	cv::imwrite("阈值分割.jpg", binary, compression_params);

	//对二值图像进行膨胀操作
	Mat element = cv::getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat dilate;
	cv::dilate(binary, dilate, element);
	imshow("区域图像分割", dilate);
	cv::imwrite("区域图像分割.jpg", dilate, compression_params);

	//膨胀之后的图像减去膨胀前的二值图像就是物体的轮廓
	Mat edge = dilate - binary;
	imshow("边缘", edge);
	cv::imwrite("边缘.jpg", edge, compression_params);

	cv::waitKey(0);


	return 0;
}