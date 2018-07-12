
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

//��ֵ�˲���(3*3)
void AverFiltering(const Mat &src, Mat &dst) 
{
	if (!src.data) return;
	//at�������ص�  
	for (int i = 1; i<src.rows; ++i)
		for (int j = 1; j < src.cols; ++j) {
			if ((i - 1 >= 0) && (j - 1) >= 0 && (i + 1)<src.rows && (j + 1)<src.cols) {//��Ե�����д���  
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
			else {//��Ե��ֵ  
				dst.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
			}
		}
}

//��Ÿ�������ֵ  
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
	for (int gap = 9 / 2; gap > 0; gap /= 2)//ϣ������  
		for (int i = gap; i < 9; ++i)
			for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
				swap(arr[j], arr[j + gap]);
	return arr[4];//������ֵ  
}
//��ֵ�˲�������3*3��
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
	_dst.copyTo(dst);//����  
}

//���뽷������  
void salt(Mat image, int n)
{
	int i, j;
	for (int k = 0; k<n; k++)
	{
		// rand()�������������  
		i = rand() % image.cols;
		j = rand() % image.rows;
		if (image.type() == CV_8UC1)
		{ // �Ҷ�ͼ��  
			image.at<uchar>(j, i) = 255;
		}
		else if (image.type() == CV_8UC3)
		{ // ��ɫͼ��  
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
	imshow("����ͼ��", input_image);
	//Mat dlphay_outpou;
	//Mat opencv_ouput;

	/************************************************************************/
	/*******************  ʵ��3��ͼ����ǿ��ֱ��ͼ���⻯  ********************/
	/************************************************************************/



	/*  �����˹����  */
	Mat img_output_goss(input_image.size(), input_image.type());
	Mat noise_goss(input_image.size(), input_image.type());  /**����һ����������*/
	RNG rng(time(NULL));
	rng.fill(noise_goss, RNG::NORMAL, 10, 36);  /**��˹�ֲ�*/
	cv::add(input_image, noise_goss, img_output_goss);

	/*  ���뽷������  */
	Mat img_output_salt(input_image.size(), input_image.type());
	img_output_salt = input_image;
	salt(img_output_salt, 5000);

	/*  �����������  */
	Mat img_output_add(input_image.size(), input_image.type());
	img_output_add = input_image;
	salt(img_output_add, 10000);


	// ��˹���� + ��ֵ�˲���
	Mat img_output_AverFilter_goss(input_image.size(), input_image.type());
	AverFiltering(img_output_goss, img_output_AverFilter_goss);

	// �������� + ��ֵ�˲���
	Mat img_output_AverFilter_salt(input_image.size(), input_image.type());
	AverFiltering(img_output_salt, img_output_AverFilter_salt);

	// ��˹���� + ��ֵ�˲���
	Mat img_output_MedianFliter_goss(input_image.size(), input_image.type());
	MedianFlitering(img_output_goss, img_output_MedianFliter_goss);

	// �������� + ��ֵ�˲���
	Mat img_output_MedianFliter_salt(input_image.size(), input_image.type());
	MedianFlitering(img_output_salt, img_output_MedianFliter_salt);


	//imshow("����ͼ��", noise_goss);
	imshow("���ϸ�˹�������ͼ��", img_output_goss);
	imshow("���Ͻ����������ͼ��", img_output_salt);
	imshow("���Ͻ����������ͼ��", img_output_add);

	imshow("��˹����������ֵ�˲�", img_output_AverFilter_goss);
	imshow("��������������ֵ�˲�", img_output_AverFilter_salt);

	imshow("��˹����������ֵ�˲�", img_output_MedianFliter_goss);
	imshow("��������������ֵ�˲�", img_output_MedianFliter_salt);

	waitKey(0);

	return 0;
}