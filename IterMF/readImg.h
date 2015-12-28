#ifndef READ_IMG
#define READ_IMG
#include "opencv2/opencv.hpp"
	int readimg(const char * pathname, cv::Mat img[], cv::Size newSize = cv::Size(0, 0), bool isResize = false);
#endif // !READ_IMG
