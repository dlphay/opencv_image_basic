
// ͼ����ǿ��ֱ��ͼ���⻯

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

	//ͼƬ�������
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(9);

	cv::Mat src = cv::imread("123.jpg", 1);

	imshow("ԭͼ", src);
	Mat binary = Mat::zeros(src.size(), CV_8UC1);

	//ɫ�ʷָ�õ���ֵͼ��
	for (int ii = 0; ii<src.rows; ii++)
		for (int jj = 0; jj<src.cols; jj++) {
			int b = (int)(src.at<Vec3b>(ii, jj)[0]);
			int r = (int)(src.at<Vec3b>(ii, jj)[2]);
			if (r>150 & b<100) {
				binary.at<uchar>(ii, jj) = 255;
			}
			else binary.at<uchar>(ii, jj) = 0;
		}

	cv::imshow("��ֵ�ָ�", binary);
	cv::imwrite("��ֵ�ָ�.jpg", binary, compression_params);

	//�Զ�ֵͼ��������Ͳ���
	Mat element = cv::getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat dilate;
	cv::dilate(binary, dilate, element);
	imshow("����ͼ��ָ�", dilate);
	cv::imwrite("����ͼ��ָ�.jpg", dilate, compression_params);

	//����֮���ͼ���ȥ����ǰ�Ķ�ֵͼ��������������
	Mat edge = dilate - binary;
	imshow("��Ե", edge);
	cv::imwrite("��Ե.jpg", edge, compression_params);

	cv::waitKey(0);


	return 0;
}