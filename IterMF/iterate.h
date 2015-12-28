#pragma once
#include "opencv2/opencv.hpp"
#include "match.h"
#include "fusion.h"
int iterate(Mat & dst, const cv::Mat & img, const cv::Mat & ref, cv::Mat & rel, int pixelSize, float threshold, Mat & mark, Mat * weight/* = nullptr*/, int p1/* = -1*/, int p2/* = -1*/);
Rect getRectFromCenter(int centerX, int centerY, int pixelSize);