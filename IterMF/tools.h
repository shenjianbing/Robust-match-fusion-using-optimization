#pragma once
#include "opencv2/opencv.hpp"
#include <cmath>
#include <limits>
using namespace std;
using namespace cv;

//返回值：[b, g, r], 要求r与i大小相同
cv::Vec3f similarity_measurement(const Mat& re/*ref*/, const Mat& im, 
	float ar = 1, float br = 1, float cr = 1, 
	float as = 1, float bs = 1, float cs = 1, float gamma = 2 /*= 2*/);
Mat imread_CV64FC3(const string& filename);
Vec3f inline mul(const Vec3f & a, const Vec3f & b);
cv::Vec3f mul(const Vec3f & a, const float f);
Vec3f inline div(const Vec3f & a, const Vec3f & b);
Vec3f inline sqrt(const Vec3f & a);
Vec3f inline abs(const Vec3f & a);

//根据角度和位移测量相似度 point:left top point in src
Vec3f calcSimilarity(const Mat &src, const Mat &ref, Point2i point, float theta, int tx, int ty, Size pixelSize = Size(7, 7));
cv::Vec3f calcSimilarity(const Mat &src, const Mat &refer, Point2i point, int tx, int ty, Size pixelSize /*= Size(7, 7)*/);
Mat drawWithBox(Mat mat, Point2i point, Size size, Scalar color = Scalar(0, 0, 255));
cv::Mat imread_CV32FC3(const string& filename);
void copyPatch(Mat & dst, const Mat & src, Rect dst_p, Rect src_p);
//int round(float r);