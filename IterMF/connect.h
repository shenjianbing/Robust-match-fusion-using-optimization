#pragma once
#include "opencv2/opencv.hpp"
#include <fstream>
/*	1	2	5	6	9	10
	3	4	7	8	11	12*/
cv::Mat readFromTxt(int cols, int rows, char * file_name_1, char * file_name_2, char * file_name_3);
cv::Mat readRel(int cols, int rows, const char * img1_name, const char * img2_name);