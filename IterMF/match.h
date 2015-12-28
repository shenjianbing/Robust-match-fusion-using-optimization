#pragma once
#include "opencv2/opencv.hpp"


//************************************
// Method:    matchPixel
// FullName:  matchPixel
// Access:    public 
// Returns:   int 0:成功修改 1:无需操作 -1:失败
// Qualifier:
// Parameter: const cv::Mat & img 待匹配图像
// Parameter: const cv::Mat & ref 参考图像
// Parameter: cv::Mat & rel 周围关系矩阵，3*3
// Parameter: int x 
// Parameter: int y
// Parameter: int threshold 阈值
// Parameter: int pixelSize odd
// Introduce: 寻找ref中与img(x, y)的最佳匹配，结果修改rel中心点
//************************************
int matchPixel(const cv::Mat & img, const cv::Mat & ref, cv::Mat & rel, int x, int y, float threshold, int pixelSize = 7);
//************************************
// Method:    getCenter
// FullName:  getCenter
// Access:    public 
// Returns:   cv::Vec3f
// Qualifier:
// Parameter: cv::Mat rel 周围关系矩阵，3*3
// Introduce: 获取关系矩阵中心点
//************************************
cv::Vec3f & getCenter(cv::Mat & rel);