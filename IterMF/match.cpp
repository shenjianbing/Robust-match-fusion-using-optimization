#include "match.h"
#include "tools.h"
using namespace cv;
const int MIN_POINT = 1;
const int MAX_POINT = 8;
int matchPixel(const Mat & img, const Mat & ref, Mat & rel, int x, int y, float threshold, int pixelSize)
{
	int _width = img.cols;
	int _height = img.rows;
	int pixelBorderSize = (pixelSize - 1) / 2;
	if (x - pixelBorderSize < 0 || x + pixelBorderSize >= _width)
	{
		printf("width out of range in matchPixel\n x = %d, y = %d",x, y);
		return -1;
	}
	if (y - pixelBorderSize < 0 || y + pixelBorderSize >= _height)
	{
		printf("height out of range in matchPixel\n x = %d, y = %d", x, y);
		return -1;
	}
	Vec3f p = img.at<Vec3f>(Point(x, y));

	//过曝，放弃像素
	if (p[0] > 0.9 && p[1] > 0.9 && p[2] > 0.9)
	{
		return -1;
	}

	//判断是否已经准确匹配
	Vec3f center = getCenter(rel);
	if (center[2] >= threshold)
	{
		return 0;
	}

	//计算周围点匹配程度
	Vector<Vec3f> list;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (rel.at<Vec3f>(i, j)[2] >= threshold)
			{
				list.push_back(rel.at<Vec3f>(i, j));
			}
		}
	}
	
	if (list.size() < MIN_POINT)
	{
		return -1;
	}
	
	//搜索匹配
	//list.push_back(Vec3f(x, y, 0));
	int length = (8 - list.size()) / 2 + 1;
	//length = sqrt(length);
	float tempV, MaxV = 0;
	int max_X = x, max_Y = y;
	Vec3f tempVec;
	static int * chackList = new int[_width * _height];
	memset(chackList, 0, _width * _height);
	for each (Vec3f vec in list)
	{
		if (chackList[(int)vec[0] + (int)(vec[1]) * _width] == 0)
		{
			chackList[(int)vec[0] + (int)(vec[1]) * _width] = 1;
		}
		else
		{
			continue;
		}

		//过曝，放弃匹配
		Vec3f pr = img.at<Vec3f>(Point(vec[0], vec[1]));
		if (pr[0] > 0.9 && pr[1] > 0.9 && pr[2] > 0.9)
		{
			continue;
		}

		for (int i = - length + vec[0] - x; i < length + vec[0] - x; i++)
		{
			if (i - pixelBorderSize + x < 0 || i + pixelBorderSize + x >= _width)
			{
				continue;
			}
			for (int j = - length + vec[1] - y; j < length + vec[1] - y; j++)
			{
				if (j - pixelBorderSize + y < 0 || j + pixelBorderSize + y >= _height)
				{
					continue;
				}
				tempVec = calcSimilarity(img, ref, Point2i(x - pixelBorderSize, y - pixelBorderSize), i, j, Size(pixelSize, pixelSize));
				tempV = (tempVec[0] + tempVec[1] + tempVec[2]) / 3.0;
				/*if (abs(tempVec[0]) + abs(tempVec[1]) + abs(tempVec[2]) > threshold && tempVec[0] + tempVec[1] + tempVec[2] < threshold)
				{
					imshow("1", img(Rect(x - pixelBorderSize, y - pixelBorderSize, pixelSize, pixelSize)));
					imshow("2", img(Rect(x - pixelBorderSize + i, y - pixelBorderSize + j, pixelSize, pixelSize)));
					waitKey(3000);
				}*/
				if (tempV > MaxV)
				{
					MaxV = tempV;
					max_X = x + i;
					max_Y = y + j;
				}
			}
		}
	}
	//delete[] chackList;
	getCenter(rel) = Vec3f(max_X, max_Y, MaxV);
	if (center[2] >= threshold)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

Vec3f & getCenter(Mat & rel)
{
	return rel.ptr<Vec3f>(1)[1];
}

