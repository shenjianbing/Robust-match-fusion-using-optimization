#ifndef HDR_H
#define HDR_H
#include "opencv2/opencv.hpp"
using namespace cv;
Mat addWeight(const Mat & src, const Mat & weight);
// laplacian滤波,获得边界信息
Mat laplacian(const Mat& src);

// 通过RGB标准差获得色彩鲜艳区域
Mat Saturation(const Mat& src);

// 获得曝光适中区域
Mat Wellexposedness(const Mat& src);

// 计算边框像素
int border(int rows, int cols, int n);

// 生成gaussian金字塔
void gaussianPyramid(const Mat & src, Mat pyr[], int nlev, int borderType);
// 从金字塔重建图片
cv::Mat reconstructLaplacianPyramid(cv::Mat pyr[], int nlev);	

// 生成laplacian金字塔
void laplacianPyramid(const Mat & src, Mat pyr[], int nlev, int borderType);
void sumTo1(cv::Mat src[], int num);
#endif // !HDR

