#include "colorTransform.h"
using namespace cv;
using namespace std;


ColorTransform::ColorTransform(const char * imgName1, const char * imgName2)
{
	initFromRel(imgName1, imgName2);
}


float ColorTransform::transform(int color, float value)
{
	float r = data[color][(int)(value * (size - 1))];
	if (r < 0)
	{
		r = 0;
	}
	if (r > 1)
	{
		r = 1;
	}
	return r;
}

cv::Mat ColorTransform::matTransform(const cv::Mat & src)
{
	int c = src.cols;
	int r = src.rows;
	int t = src.type();
	Mat dst = Mat(r, c, t);
	Vec3f v;
	for (int j = 0; j < r; j++)
	{
		for (int i = 0; i < c; i++)
		{
			v = src.at<Vec3f>(j, i);
			
			v[0] = transform(BLUE, v[0]);
			v[1] = transform(GREEN, v[1]);
			v[2] = transform(RED, v[2]);

			dst.at<Vec3f>(j, i) = v;
		}
	}
	return dst;
}

void ColorTransform::initFromRel(const char * imgName1, const char * imgName2)
{
	char file_name[100];
	sprintf(file_name, "%s%s_ct.rel", imgName1, imgName2);
	fstream s(file_name);
	s >> size;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j <= size; j++)
		{
			s >> data[i][j];
		}
	}
	s.close();
}

void ColorTransform::showCurve(const char * name)
{
	Mat p = Mat::zeros(1000, 1000, CV_8UC3);
	for (int f = 0; f < 1000; f++)
	{
		if (f < 333)cout << f / 999.0 << " --> " << transform(BLUE, f / 1000.0) << endl;
		p.at<Vec3b>(Point(f, 999 - (int)(transform(RED, f / 1000.0) * 999)))[2] = 255;
		p.at<Vec3b>(Point(f, 999 - (int)(transform(GREEN, f / 1000.0) * 999)))[1] = 255;
		p.at<Vec3b>(Point(f, 999 - (int)(transform(BLUE, f / 1000.0) * 999)))[0] = 255;
	}
	imshow(name, p);
}

void ColorTransform::matTransformWithReplace(cv::Mat & src, const cv::Mat & mark)
{
	Vec3f v;
	int r = src.rows;
	int c = src.cols;
	for (int j = 0; j < r; j++)
	{
		for (int i = 0; i < c; i++)
		{
			if (mark.at<uchar>(j, i) == 0)
			{
				v = src.at<Vec3f>(j, i);
				v[0] = transform(BLUE, v[0]);
				v[1] = transform(GREEN, v[1]);
				v[2] = transform(RED, v[2]);
				src.at<Vec3f>(j, i) = v;
			}
		}
	}
}


