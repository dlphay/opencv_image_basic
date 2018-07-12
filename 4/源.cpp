
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


	//�Ҷ�ͼ 
	cvtColor(input_image, input_image, CV_RGB2GRAY);
	threshold(input_image, input_image, 65, 255, THRESH_BINARY);
	namedWindow("ԭͼ");
	imshow("ԭͼ", input_image);


	// opencv������  �������ͣ��ٸ�ʴ
	Mat element(4, 4, CV_8U, Scalar(1));
	Mat closed;
	morphologyEx(input_image, closed, MORPH_CLOSE, element);
	namedWindow("������");
	imshow("������", closed);

	// opencv������  ���ȸ�ʴ��������
	Mat opened;
	morphologyEx(input_image, opened, MORPH_OPEN, element);
	namedWindow("������");
	imshow("������", opened);

	waitKey(0);

	return 0;
}