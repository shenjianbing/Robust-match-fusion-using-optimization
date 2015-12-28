#include "iterate.h"
using namespace cv;
using namespace std;

void extendMark(Mat & mark, Rect rect)
{
	Mat t = mark(rect);
	int c = t.cols;
	int r = t.rows;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			if (t.at<uchar>(j, i) == 0)
			{
				t.at<uchar>(j, i) = 255;
			}
		}
	}
}

int iterate(Mat & dst,const cv::Mat & img, const cv::Mat & ref, cv::Mat & rel, int pixelSize, float threshold, Mat & mark, Mat * weight/* = nullptr*/, int p1/* = -1*/, int p2/* = -1*/)
{
	//Mat imgCopy = img.clone();
	int count = 0;
	int W = img.cols;
	int H = img.rows;
	int padWidth = pixelSize / 2;

	for (int i = padWidth; i < W - padWidth ; i++)
	{
		for (int j = padWidth; j < H - padWidth; j++)
		{
			if (mark.at<uchar>(j, i) != 1)
			{
				int result = matchPixel(img, ref, rel(Rect(i - 1, j - 1, 3, 3)), i, j, threshold, pixelSize);
				switch (result)
				{
				//³É¹¦ÐÞ¸Ä
				case 0:
					if (rel.at<Vec3f>(j, i)[2] >= threshold)
					{
						Vec3f m = rel.at<Vec3f>(j, i);
						++count;
						Mat t;
						copyPatch(dst, ref, getRectFromCenter(i, j, pixelSize), getRectFromCenter(m[0], m[1], pixelSize));
						extendMark(mark, getRectFromCenter(i, j, pixelSize));
						mark.at<uchar>(j, i) = 1;
					}
					break;
				case 1:
					mark.at<uchar>(j, i) = 1;
					break;
				default:
					break;
				}
			}
		}
	}
	return count;
}

Rect getRectFromCenter(int centerX, int centerY, int pixelSize)
{
	int padWidth = pixelSize / 2;
	return Rect(centerX - padWidth, centerY - padWidth, pixelSize, pixelSize);
}

