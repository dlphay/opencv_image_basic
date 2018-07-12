
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


//***************************************************************************************************************************
// ���ƣ�hist_equalization_dlphay()
// ���ܣ��Ҷ�ͼ���ֱ��ͼ����(���԰汾)
// ������ �βΣ�Mat input_image    ����ͼ��
//      ����ֵ��Mat output_image   ���ͼ��      
//****************************************************************************************************************************
Mat hist_equalization_GRAY_dlphay_test(Mat input_image)
{
	const int grayMax = 255;
	vector<vector<int>> graylevel(grayMax + 1);

	cout << graylevel.size() << endl;
	Mat output_image;
	input_image.copyTo(output_image);

	if (!input_image.data)
	{
		return output_image;
	}
	for (int i = 0; i < input_image.rows - 1; i++)
	{
		uchar* ptr = input_image.ptr<uchar>(i);  // �����Ϊһ��һ�е�����  ������ptr
		for (int j = 0; j < input_image.cols - 1; j++)
		{
			int x = ptr[j];
			graylevel[x].push_back(0);//����ط�д�Ĳ��ã������ά����ֻ��Ϊ�˼�¼ÿһ���Ҷ�ֵ�����ظ���  
		}
	}
	for (int i = 0; i < output_image.rows - 1; i++)
	{
		uchar* imgptr = output_image.ptr<uchar>(i);
		uchar* imageptr = input_image.ptr<uchar>(i);
		for (int j = 0; j < output_image.cols - 1; j++)
		{
			int sumpiexl = 0;
			for (int k = 0; k < imageptr[j]; k++)
			{
				sumpiexl = graylevel[k].size() + sumpiexl;
			}
			imgptr[j] = (grayMax*sumpiexl / (input_image.rows*input_image.cols));
		}
	}
	return output_image;
}
//***************************************************************************************************************************
// ���ƣ�hist_equalization_dlphay()
// ���ܣ��Ҷ�ͼ���ֱ��ͼ����
// ������ �βΣ�Mat input_image    ����ͼ��
//      ����ֵ��Mat output_image   ���ͼ��      
//****************************************************************************************************************************
Mat hist_equalization_GRAY_dlphay(Mat& input)
{
	int gray_sum = 0;  //��������
	int gray[256] = { 0 };  //��¼ÿ���Ҷȼ����µ����ظ���
	double gray_prob[256] = { 0 };  //��¼�Ҷȷֲ��ܶ�
	double gray_distribution[256] = { 0 };  //��¼�ۼ��ܶ�
	int gray_equal[256] = { 0 };  //���⻯��ĻҶ�ֵ

	Mat output = input.clone();
	gray_sum = input.cols * input.rows;

	//ͳ��ÿ���Ҷ��µ����ظ���
	for (int i = 0; i < input.rows; i++)
	{
		uchar* p = input.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++)
		{
			int vaule = p[j];
			gray[vaule]++;
		}
	}
	//ͳ�ƻҶ�Ƶ��
	for (int i = 0; i < 256; i++)
	{
		gray_prob[i] = ((double)gray[i] / gray_sum);
	}

	//�����ۼ��ܶ�
	gray_distribution[0] = gray_prob[0];
	for (int i = 1; i < 256; i++)
	{
		gray_distribution[i] = gray_distribution[i - 1] + gray_prob[i];
	}

	//���¼�����⻯��ĻҶ�ֵ���������롣�ο���ʽ��(N-1)*T+0.5
	for (int i = 0; i < 256; i++)
	{
		gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
	}
	//ֱ��ͼ���⻯,����ԭͼÿ���������ֵ
	for (int i = 0; i < output.rows; i++)
	{
		uchar* p = output.ptr<uchar>(i);
		for (int j = 0; j < output.cols; j++)
		{
			p[j] = gray_equal[p[j]];
		}
	}
	return output;
}

//***************************************************************************************************************************
// ���ƣ�hist_equalization_dlphay()
// ���ܣ���ɫͼ���ֱ��ͼ����
// ������ �βΣ�Mat input    ����ͼ��
//      ����ֵ��Mat output   ���ͼ��      
//****************************************************************************************************************************
Mat hist_equalization_BGR_dlphay(Mat input)
{
	Mat output;
	uchar *dataIn = (uchar *)input.ptr<uchar>(0);//input��ͷָ�룬ָ���0�е�0�����أ���Ϊ������
	uchar *dataOut = (uchar *)output.ptr<uchar>(0);
	const int ncols = input.cols;//��ʾ����ͼ���ж�����
	const int nrows = input.rows;//��ʾ����ͼ���ж�����
	int nchannel = input.channels();//ͨ������һ����3
	int pixnum = ncols*nrows;
	int vData[765] = { 0 };//����R+G+B��ʱ��255+255+255������Ϊ765�����ȼ�
	double vRate[765] = { 0 };
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			vData[dataIn[i*ncols*nchannel + j*nchannel + 0]
				+ dataIn[i*ncols*nchannel + j*nchannel + 1]
				+ dataIn[i*ncols*nchannel + j*nchannel + 2]]++;//��Ӧ�����ȼ�ͳ��
		}
	}
	for (int i = 0; i < 764; i++)
	{
		for (int j = 0; j < i; j++)
		{
			vRate[i] += (double)vData[j] / (double)pixnum;//���
		}
	}
	for (int i = 0; i < 764; i++)
	{
		vData[i] = (int)(vRate[i] * 764 + 0.5);//���й�һ������
	}
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			int amplification = vData[dataIn[i*ncols*nchannel + j*nchannel + 0]
				+ dataIn[i*ncols*nchannel + j*nchannel + 1]
				+ dataIn[i*ncols*nchannel + j*nchannel + 2]] -
				(dataIn[i*ncols*nchannel + j*nchannel + 0]
					+ dataIn[i*ncols*nchannel + j*nchannel + 1]
					+ dataIn[i*ncols*nchannel + j*nchannel + 2]);//�ñ任���ֵ��ȥԭֵ�ĵ����ȼ��Ĳ�ֵ����3�����ÿ��ͨ��Ӧ���仯��ֵ
			int b = dataIn[i*ncols*nchannel + j*nchannel + 0] + amplification / 3 + 0.5;
			int g = dataIn[i*ncols*nchannel + j*nchannel + 1] + amplification / 3 + 0.5;
			int r = dataIn[i*ncols*nchannel + j*nchannel + 2] + amplification / 3 + 0.5;
			if (b > 255) b = 255;//����Խλ�ж�
			if (g > 255) g = 255;
			if (r > 255) r = 255;
			if (r < 0) r = 0;//����Խλ�ж�
			if (g < 0) g = 0;
			if (b < 0) b = 0;
			dataOut[i*ncols*nchannel + j*nchannel + 0] = b;
			dataOut[i*ncols*nchannel + j*nchannel + 1] = g;
			dataOut[i*ncols*nchannel + j*nchannel + 2] = r;
		}
	}
	return output;
}


int main(int argc, char* argv[])
{
	//IplImage* img1 = cvLoadImage("input.jpg");

	Mat input_image = imread("input.jpg", 0);
	Mat dlphay_outpou;
	Mat opencv_ouput;



	/************************************************************************/
	/*******************  ʵ��2��ͼ����ǿ��ֱ��ͼ���⻯  ********************/
	/************************************************************************/
	equalizeHist(input_image, opencv_ouput);
	//dlphay_outpou = hist_equalization_GRAY_dlphay_test(input_image);
	dlphay_outpou = hist_equalization_GRAY_dlphay(input_image);
	//dlphay_outpou = hist_equalization_BGR_dlphay(input_image);


	/************************************************************************/
	/*******************  ʵ��3��ͼ����ǿ��ֱ��ͼ���⻯  ********************/
	/************************************************************************/


	imshow("����ͼ��", input_image);
	//imshow("ͼ����ǿ��ֱ��ͼ���⻯", opencv_ouput);
	imshow("ͼ����ǿ��ֱ��ͼ���⻯", dlphay_outpou);
	waitKey(0);

	return 0;
}