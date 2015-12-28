#include "fusion.h"
using namespace cv;
int borderType = BORDER_REPLICATE;
Mat fusion(const Mat& aim, const Mat& ref, Rect aimPocision, Rect refPocision)
{
	Mat arr[2] = { Mat(aim, aimPocision), Mat(ref, refPocision) };
	return fusion(arr, 2);
}

Mat fusion(Mat img[], int num)
{
	Mat img_gray[20];
	for (int i = 0; i < num; i++)
	{
		cvtColor(img[i], img_gray[i], COLOR_BGR2GRAY);
	}
	Mat weightImg[20];
	int R = img[0].rows;
	int C = img[0].cols;
	int nlev = (int)floor(log(min(R, C)) / log(2));
	//计算权重
	for (int i = 0; i < num; i++)
	{
		weightImg[i] = Mat::ones(R, C, CV_32F);
		weightImg[i] = weightImg[i].mul(laplacian(img_gray[i]));
		weightImg[i] = weightImg[i].mul(Saturation(img[i]));
		weightImg[i] = weightImg[i].mul(Wellexposedness(img[i]));
	}
	sumTo1(weightImg, num);
	
	//生成金字塔
	Mat pyrW[100], pyrI[100], pyr[100];
	//创建目标金字塔
	gaussianPyramid(Mat::zeros(R, C, CV_32FC3), pyr, nlev, borderType);
	for (int i = 0; i < num; i++)
	{
		//转换权重图
		gaussianPyramid(weightImg[i], pyrW, nlev, borderType);
		//转换原图，并且使用浮点数类型
		laplacianPyramid(img[i], pyrI, nlev, borderType);
		for (int j = 1; j <= nlev; j++)
		{
			//将原图加权后加到目标金字塔中
			pyr[j] += addWeight(pyrI[j], pyrW[j]);
		}
	}

	//合成金字塔
	//从金字塔重构带边图片
	Mat dstWithBorder = reconstructLaplacianPyramid(pyr, nlev);
	//消去边框
	Mat dst(dstWithBorder, Rect(border(R, C, 3), border(R, C, 1), C, R));
	
	return dst;
}

Mat * getWeight(Mat img[], int num, bool isSumTo1/* = false*/)
{
	Mat img_gray[20];
	for (int i = 0; i < num; i++)
	{
		cvtColor(img[i], img_gray[i], COLOR_BGR2GRAY);
	}
	Mat * weightImg = new Mat[num];
	int R = img[0].rows;
	int C = img[0].cols;
	//计算权重
	for (int i = 0; i < num; i++)
	{
		weightImg[i] = Mat::ones(R, C, CV_32F);
		weightImg[i] = weightImg[i].mul(laplacian(img_gray[i]));
		weightImg[i] = weightImg[i].mul(Saturation(img[i]));
		weightImg[i] = weightImg[i].mul(Wellexposedness(img[i]));
	}
	if (isSumTo1 == true)
	{
		sumTo1(weightImg, num);
	}
	return weightImg;
}

