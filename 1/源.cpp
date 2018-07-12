
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


//***************************************************************************************************************************
// 名称：hist_equalization_dlphay()
// 功能：灰度图像的直方图均衡(测试版本)
// 参数： 形参：Mat input_image    输入图像
//      返回值：Mat output_image   输出图像      
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
		uchar* ptr = input_image.ptr<uchar>(i);  // 处理成为一行一行的数据  储存在ptr
		for (int j = 0; j < input_image.cols - 1; j++)
		{
			int x = ptr[j];
			graylevel[x].push_back(0);//这个地方写的不好，引入二维数组只是为了记录每一个灰度值的像素个数  
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
// 名称：hist_equalization_dlphay()
// 功能：灰度图像的直方图均衡
// 参数： 形参：Mat input_image    输入图像
//      返回值：Mat output_image   输出图像      
//****************************************************************************************************************************
Mat hist_equalization_GRAY_dlphay(Mat& input)
{
	int gray_sum = 0;  //像素总数
	int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
	double gray_prob[256] = { 0 };  //记录灰度分布密度
	double gray_distribution[256] = { 0 };  //记录累计密度
	int gray_equal[256] = { 0 };  //均衡化后的灰度值

	Mat output = input.clone();
	gray_sum = input.cols * input.rows;

	//统计每个灰度下的像素个数
	for (int i = 0; i < input.rows; i++)
	{
		uchar* p = input.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++)
		{
			int vaule = p[j];
			gray[vaule]++;
		}
	}
	//统计灰度频率
	for (int i = 0; i < 256; i++)
	{
		gray_prob[i] = ((double)gray[i] / gray_sum);
	}

	//计算累计密度
	gray_distribution[0] = gray_prob[0];
	for (int i = 1; i < 256; i++)
	{
		gray_distribution[i] = gray_distribution[i - 1] + gray_prob[i];
	}

	//重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
	for (int i = 0; i < 256; i++)
	{
		gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
	}
	//直方图均衡化,更新原图每个点的像素值
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
// 名称：hist_equalization_dlphay()
// 功能：彩色图像的直方图均衡
// 参数： 形参：Mat input    输入图像
//      返回值：Mat output   输出图像      
//****************************************************************************************************************************
Mat hist_equalization_BGR_dlphay(Mat input)
{
	Mat output;
	uchar *dataIn = (uchar *)input.ptr<uchar>(0);//input的头指针，指向第0行第0个像素，且为行优先
	uchar *dataOut = (uchar *)output.ptr<uchar>(0);
	const int ncols = input.cols;//表示输入图像有多少行
	const int nrows = input.rows;//表示输入图像有多少列
	int nchannel = input.channels();//通道数，一般是3
	int pixnum = ncols*nrows;
	int vData[765] = { 0 };//由于R+G+B最时是255+255+255，所以为765个亮度级
	double vRate[765] = { 0 };
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			vData[dataIn[i*ncols*nchannel + j*nchannel + 0]
				+ dataIn[i*ncols*nchannel + j*nchannel + 1]
				+ dataIn[i*ncols*nchannel + j*nchannel + 2]]++;//对应的亮度级统计
		}
	}
	for (int i = 0; i < 764; i++)
	{
		for (int j = 0; j < i; j++)
		{
			vRate[i] += (double)vData[j] / (double)pixnum;//求出
		}
	}
	for (int i = 0; i < 764; i++)
	{
		vData[i] = (int)(vRate[i] * 764 + 0.5);//进行归一化处理
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
					+ dataIn[i*ncols*nchannel + j*nchannel + 2]);//用变换后的值减去原值的到亮度级的差值，除3后就是每个通道应当变化的值
			int b = dataIn[i*ncols*nchannel + j*nchannel + 0] + amplification / 3 + 0.5;
			int g = dataIn[i*ncols*nchannel + j*nchannel + 1] + amplification / 3 + 0.5;
			int r = dataIn[i*ncols*nchannel + j*nchannel + 2] + amplification / 3 + 0.5;
			if (b > 255) b = 255;//上溢越位判断
			if (g > 255) g = 255;
			if (r > 255) r = 255;
			if (r < 0) r = 0;//下溢越位判断
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
	/*******************  实验2：图像增强—直方图均衡化  ********************/
	/************************************************************************/
	equalizeHist(input_image, opencv_ouput);
	//dlphay_outpou = hist_equalization_GRAY_dlphay_test(input_image);
	dlphay_outpou = hist_equalization_GRAY_dlphay(input_image);
	//dlphay_outpou = hist_equalization_BGR_dlphay(input_image);


	/************************************************************************/
	/*******************  实验3：图像增强—直方图均衡化  ********************/
	/************************************************************************/


	imshow("输入图像", input_image);
	//imshow("图像增强—直方图均衡化", opencv_ouput);
	imshow("图像增强—直方图均衡化", dlphay_outpou);
	waitKey(0);

	return 0;
}