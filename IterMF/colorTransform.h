#pragma once
#include "opencv2/opencv.hpp"
#include <fstream>
class ColorTransform
{
private:
	float data[3][10001];
	int size;
public:
	const int RED = 0;
	const int GREEN = 1;
	const int BLUE = 2;
	void initFromRel(const char * imgName1, const char * imgName2);
	ColorTransform(const char * imgName1, const char * imgName2);
	float transform(int color, float value);
	cv::Mat matTransform(const cv::Mat & src);
	void showCurve(const char * name);
	void matTransformWithReplace(cv::Mat & src, const cv::Mat & mark);
};