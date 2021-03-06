#include "stdafx.h"  

#include<iostream>  


#include "cv.h"  
#include "highgui.h"  
#include "math.h"  
#include "opencv2/core/core.hpp"  

#pragma comment(lib,"opencv_core2410d.lib")                
#pragma comment(lib,"opencv_highgui2410d.lib")                
#pragma comment(lib,"opencv_imgproc2410d.lib")     



#define DOWNRESIZE 0 // 缩小  
#define UPRESIZE   1 // 放大  
#define HORAO      2 // 水平外凹  
#define HORTU      3 // 水平外凸  
#define LADDER     4 // 梯形变形  
#define TRIANGLE   5 // 三角形变形  
#define SSHAPE     6 // S形变形  
#define WAVESHAPE  7 // 波浪形变形  
#define Concentrated 8 //集中形变形  
#define Scattered   9 // 散开形变形  

#define RANGE     100 // 水平外凹或外凸的幅度  
#define PI        3.1415926  

using namespace std;
using namespace cv;

void MaxFrame(IplImage* frame)
{
	uchar* old_data = (uchar*)frame->imageData;
	uchar* new_data = new uchar[frame->widthStep * frame->height];

	int center_X = frame->width / 2;
	int center_Y = frame->height / 2;
	int radius = 400;
	int newX = 0;
	int newY = 0;

	int real_radius = (int)(radius / 2.0);
	for (int i = 0; i < frame->width; i++)
	{
		for (int j = 0; j < frame->height; j++)
		{
			int tX = i - center_X;
			int tY = j - center_Y;

			int distance = (int)(tX * tX + tY * tY);
			if (distance < radius * radius)
			{
				newX = (int)((float)(tX) / 2.0);
				newY = (int)((float)(tY) / 2.0);

				newX = (int)(newX * (sqrt((double)distance) / real_radius));
				newX = (int)(newX * (sqrt((double)distance) / real_radius));

				newX = newX + center_X;
				newY = newY + center_Y;

				new_data[frame->widthStep * j + i * 3] = old_data[frame->widthStep * newY + newX * 3];
				new_data[frame->widthStep * j + i * 3 + 1] = old_data[frame->widthStep * newY + newX * 3 + 1];
				new_data[frame->widthStep * j + i * 3 + 2] = old_data[frame->widthStep * newY + newX * 3 + 2];
			}
			else
			{
				new_data[frame->widthStep * j + i * 3] = old_data[frame->widthStep * j + i * 3];
				new_data[frame->widthStep * j + i * 3 + 1] = old_data[frame->widthStep * j + i * 3 + 1];
				new_data[frame->widthStep * j + i * 3 + 2] = old_data[frame->widthStep * j + i * 3 + 2];
			}
		}
	}
	memcpy(old_data, new_data, sizeof(uchar) * frame->widthStep * frame->height);
	delete[] new_data;
}


void MinFrame(IplImage* frame)
{
	uchar* old_data = (uchar*)frame->imageData;
	uchar* new_data = new uchar[frame->widthStep * frame->height];

	int center_X = frame->width / 2;
	int center_Y = frame->height / 2;

	int radius = 0;
	double theta = 0;
	int newX = 0;
	int newY = 0;

	for (int i = 0; i < frame->width; i++)
	{
		for (int j = 0; j < frame->height; j++)
		{
			int tX = i - center_X;
			int tY = j - center_Y;

			theta = atan2((double)tY, (double)tX);
			radius = (int)sqrt((double)(tX * tX) + (double)(tY * tY));
			int newR = (int)(sqrt((double)radius) * 12);
			newX = center_X + (int)(newR * cos(theta));
			newY = center_Y + (int)(newR * sin(theta));

			if (!(newX > 0 && newX < frame->width))
			{
				newX = 0;
			}
			if (!(newY > 0 && newY < frame->height))
			{
				newY = 0;
			}

			new_data[frame->widthStep * j + i * 3] = old_data[frame->widthStep * newY + newX * 3];
			new_data[frame->widthStep * j + i * 3 + 1] = old_data[frame->widthStep * newY + newX * 3 + 1];
			new_data[frame->widthStep * j + i * 3 + 2] = old_data[frame->widthStep * newY + newX * 3 + 2];
		}
	}
	memcpy(old_data, new_data, sizeof(uchar) * frame->widthStep * frame->height);
	delete[] new_data;
}

// 哈哈镜制作  
int main(int argc, char** argv)
{
	IplImage* pImg; //声明IplImage指针  
	IplImage* pImg1; //声明IplImage指针  
	int i, j;
	int method = 0;
	CvSize size;
	double tmp;

	method = 8;
	//method = HORAO;  

	//载入图像  
	pImg = cvLoadImage("input.jpg", 1);
	cvNamedWindow("Image", 1);//创建窗口  
	cvShowImage("Image", pImg);//显示图像  
	printf("imageSize: %d height: %d, width: %d, nChannels: %d\n", pImg->imageSize, pImg->height, pImg->width, pImg->nChannels);

	//MaxFrame(pImg);  
	//MinFrame(pImg);  

	switch (method)
	{
		// 图像缩小  
	case DOWNRESIZE:
		size = cvGetSize(pImg);
		size.width = (size.width >> 3) << 2; // 在OpenCV里边，widthStep必须是4的倍数，从而实现字节对齐，有利于提高运算速度。  
		size.height = size.height >> 1;
		pImg1 = cvCreateImage(size, IPL_DEPTH_8U, 1);
		printf("imageSize: %d height: %d, width: %d, nChannels: %d\n", pImg1->imageSize, pImg1->height, pImg1->width, pImg1->nChannels);
		for (i = 0; i<pImg1->height; i++)
			for (j = 0; j<pImg1->width; j++)
			{
				pImg1->imageData[i*pImg1->width + j] = pImg->imageData[i * 2 * pImg->width + j * 2];
			}
		break;
		// 图像放大  
	case UPRESIZE:
		/* 添加代码 */
		break;
		// 水平外凹  
	case HORAO:
		pImg1 = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
		printf("imageSize: %d height: %d, width: %d, nChannels: %d\n", pImg1->imageSize, pImg1->height, pImg1->width, pImg1->nChannels);
		for (i = 0; i<pImg1->height; i++)
		{
			tmp = RANGE*sin(i*PI / pImg1->height);
			for (j = tmp; j<pImg1->width - tmp; j++)
			{
				pImg1->imageData[i*pImg1->width + j] = pImg->imageData[i*pImg->width + (int)((j - tmp)*(pImg->width) / (pImg->width - 2 * tmp))];
			}
		}
		break;
		// 水平外凸  
	case HORTU:
		/* 添加代码 */
		break;
		// 梯形变形  
	case LADDER:
		/* 添加代码 */
		break;
		// 三角形变形  
	case TRIANGLE:
		/* 添加代码 */
		break;
		// S形变形  
	case SSHAPE:
		/* 添加代码 */
		break;
		// 波浪形变形  
	case WAVESHAPE:
		/* 添加代码 */
		break;
	case Concentrated:
		MaxFrame(pImg);
		break;
	case Scattered:
		MinFrame(pImg);
		break;
	default:
		printf("no method support\n");
		break;
	}

	// 显示结果  
	cvNamedWindow("Image1", 1);//创建窗口  
	cvShowImage("Image1", pImg);//显示图像  

	cvWaitKey(0); //等待按键  

				  //销毁窗口 释放内存  
	cvDestroyWindow("Image");//销毁窗口  
	cvReleaseImage(&pImg); //释放图像  
	cvDestroyWindow("Image1");//销毁窗口  
	cvReleaseImage(&pImg1); //释放图像  

	return 0;

}
