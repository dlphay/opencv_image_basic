
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

//均值滤波器(3*3)
void AverFiltering(const Mat &src, Mat &dst) 
{
	if (!src.data) return;
	//at访问像素点  
	for (int i = 1; i<src.rows; ++i)
		for (int j = 1; j < src.cols; ++j) {
			if ((i - 1 >= 0) && (j - 1) >= 0 && (i + 1)<src.rows && (j + 1)<src.cols) {//边缘不进行处理  
				dst.at<Vec3b>(i, j)[0] = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i - 1, j - 1)[0] + src.at<Vec3b>(i - 1, j)[0] + src.at<Vec3b>(i, j - 1)[0] +
					src.at<Vec3b>(i - 1, j + 1)[0] + src.at<Vec3b>(i + 1, j - 1)[0] + src.at<Vec3b>(i + 1, j + 1)[0] + src.at<Vec3b>(i, j + 1)[0] +
					src.at<Vec3b>(i + 1, j)[0]) / 9;
				dst.at<Vec3b>(i, j)[1] = (src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i - 1, j - 1)[1] + src.at<Vec3b>(i - 1, j)[1] + src.at<Vec3b>(i, j - 1)[1] +
					src.at<Vec3b>(i - 1, j + 1)[1] + src.at<Vec3b>(i + 1, j - 1)[1] + src.at<Vec3b>(i + 1, j + 1)[1] + src.at<Vec3b>(i, j + 1)[1] +
					src.at<Vec3b>(i + 1, j)[1]) / 9;
				dst.at<Vec3b>(i, j)[2] = (src.at<Vec3b>(i, j)[2] + src.at<Vec3b>(i - 1, j - 1)[2] + src.at<Vec3b>(i - 1, j)[2] + src.at<Vec3b>(i, j - 1)[2] +
					src.at<Vec3b>(i - 1, j + 1)[2] + src.at<Vec3b>(i + 1, j - 1)[2] + src.at<Vec3b>(i + 1, j + 1)[2] + src.at<Vec3b>(i, j + 1)[2] +
					src.at<Vec3b>(i + 1, j)[2]) / 9;
			}
			else {//边缘赋值  
				dst.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
			}
		}
}

//求九个数的中值  
uchar Median(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5,
	uchar n6, uchar n7, uchar n8, uchar n9) {
	uchar arr[9];
	arr[0] = n1;
	arr[1] = n2;
	arr[2] = n3;
	arr[3] = n4;
	arr[4] = n5;
	arr[5] = n6;
	arr[6] = n7;
	arr[7] = n8;
	arr[8] = n9;
	for (int gap = 9 / 2; gap > 0; gap /= 2)//希尔排序  
		for (int i = gap; i < 9; ++i)
			for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
				swap(arr[j], arr[j + gap]);
	return arr[4];//返回中值  
}
//中值滤波函数（3*3）
void MedianFlitering(const Mat &src, Mat &dst) {
	if (!src.data)return;
	Mat _dst(src.size(), src.type());
	for (int i = 0; i<src.rows; ++i)
		for (int j = 0; j < src.cols; ++j) {
			if ((i - 1) > 0 && (i + 1) < src.rows && (j - 1) > 0 && (j + 1) < src.cols) {
				_dst.at<Vec3b>(i, j)[0] = Median(src.at<Vec3b>(i, j)[0], src.at<Vec3b>(i + 1, j + 1)[0],
					src.at<Vec3b>(i + 1, j)[0], src.at<Vec3b>(i, j + 1)[0], src.at<Vec3b>(i + 1, j - 1)[0],
					src.at<Vec3b>(i - 1, j + 1)[0], src.at<Vec3b>(i - 1, j)[0], src.at<Vec3b>(i, j - 1)[0],
					src.at<Vec3b>(i - 1, j - 1)[0]);
				_dst.at<Vec3b>(i, j)[1] = Median(src.at<Vec3b>(i, j)[1], src.at<Vec3b>(i + 1, j + 1)[1],
					src.at<Vec3b>(i + 1, j)[1], src.at<Vec3b>(i, j + 1)[1], src.at<Vec3b>(i + 1, j - 1)[1],
					src.at<Vec3b>(i - 1, j + 1)[1], src.at<Vec3b>(i - 1, j)[1], src.at<Vec3b>(i, j - 1)[1],
					src.at<Vec3b>(i - 1, j - 1)[1]);
				_dst.at<Vec3b>(i, j)[2] = Median(src.at<Vec3b>(i, j)[2], src.at<Vec3b>(i + 1, j + 1)[2],
					src.at<Vec3b>(i + 1, j)[2], src.at<Vec3b>(i, j + 1)[2], src.at<Vec3b>(i + 1, j - 1)[2],
					src.at<Vec3b>(i - 1, j + 1)[2], src.at<Vec3b>(i - 1, j)[2], src.at<Vec3b>(i, j - 1)[2],
					src.at<Vec3b>(i - 1, j - 1)[2]);
			}
			else
				_dst.at<Vec3b>(i, j) = src.at<Vec3b>(i, j);
		}
	_dst.copyTo(dst);//拷贝  
}

//加入椒盐噪声  
void salt(Mat image, int n)
{
	int i, j;
	for (int k = 0; k<n; k++)
	{
		// rand()是随机数生成器  
		i = rand() % image.cols;
		j = rand() % image.rows;
		if (image.type() == CV_8UC1)
		{ // 灰度图像  
			image.at<uchar>(j, i) = 255;
		}
		else if (image.type() == CV_8UC3)
		{ // 彩色图像  
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
}
int main(int argc, char* argv[])
{
	//IplImage* img1 = cvLoadImage("input.jpg");

	Mat input_image = imread("input.jpg", 1);
	imshow("输入图像", input_image);
	//Mat dlphay_outpou;
	//Mat opencv_ouput;

	/************************************************************************/
	/*******************  实验3：图像增强—直方图均衡化  ********************/
	/************************************************************************/



	/*  加入高斯噪声  */
	Mat img_output_goss(input_image.size(), input_image.type());
	Mat noise_goss(input_image.size(), input_image.type());  /**创建一个噪声矩阵*/
	RNG rng(time(NULL));
	rng.fill(noise_goss, RNG::NORMAL, 10, 36);  /**高斯分布*/
	cv::add(input_image, noise_goss, img_output_goss);

	/*  加入椒盐噪声  */
	Mat img_output_salt(input_image.size(), input_image.type());
	img_output_salt = input_image;
	salt(img_output_salt, 5000);

	/*  加入加性噪声  */
	Mat img_output_add(input_image.size(), input_image.type());
	img_output_add = input_image;
	salt(img_output_add, 10000);


	// 高斯噪声 + 均值滤波器
	Mat img_output_AverFilter_goss(input_image.size(), input_image.type());
	AverFiltering(img_output_goss, img_output_AverFilter_goss);

	// 椒盐噪声 + 均值滤波器
	Mat img_output_AverFilter_salt(input_image.size(), input_image.type());
	AverFiltering(img_output_salt, img_output_AverFilter_salt);

	// 高斯噪声 + 中值滤波器
	Mat img_output_MedianFliter_goss(input_image.size(), input_image.type());
	MedianFlitering(img_output_goss, img_output_MedianFliter_goss);

	// 椒盐噪声 + 中值滤波器
	Mat img_output_MedianFliter_salt(input_image.size(), input_image.type());
	MedianFlitering(img_output_salt, img_output_MedianFliter_salt);


	//imshow("噪声图像", noise_goss);
	imshow("加上高斯噪声后的图像", img_output_goss);
	imshow("加上椒盐噪声后的图像", img_output_salt);
	imshow("加上椒加性声后的图像", img_output_add);

	imshow("高斯噪声经过均值滤波", img_output_AverFilter_goss);
	imshow("椒盐噪声经过均值滤波", img_output_AverFilter_salt);

	imshow("高斯噪声经过中值滤波", img_output_MedianFliter_goss);
	imshow("椒盐噪声经过中值滤波", img_output_MedianFliter_salt);

	waitKey(0);

	return 0;
}