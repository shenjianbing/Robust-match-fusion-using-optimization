#ifndef FUSION_H
#define FUSION_H
#include "opencv2/opencv.hpp"
#include "hdr.h"
#include "tools.h"
Mat fusion(const Mat& aim, const Mat& ref, Rect aimPocision, Rect refPocision);
Mat fusion(Mat img[], int num);
Mat * getWeight(Mat img[], int num, bool isSumTo1 = false);
#endif // !FUSION_H

