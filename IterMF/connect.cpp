#include "connect.h"

using namespace cv;
using namespace std;
Mat readRel(int cols, int rows, const char * img1_name, const char * img2_name)
{
	char * str[3];
	str[0] = new char[100];
	str[1] = new char[100];
	str[2] = new char[100];
	for (int i = 1; i <= 3; i++)
	{
		sprintf_s(str[i - 1], 100, "%s%s_%d.rel", img1_name, img2_name, i);
	}
	return readFromTxt(cols, rows, str[0], str[1], str[2]);
}
Mat readFromTxt(int cols, int rows, char * file_name_1, char * file_name_2, char * file_name_3)
{
	Mat m[3];
	float * data1 = new float[cols * rows];
	float * data2 = new float[cols * rows];
	float * data3 = new float[cols * rows];
	fstream s;
	s.open(file_name_1, ios::in);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			s >> data1[i * cols + j];
		}
	}
	s.close();
	m[0] = Mat(rows, cols, CV_32F, data1);

	s.open(file_name_2, ios::in);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			s >> data2[i * cols + j];
		}
	}
	s.close();
	m[1] = Mat(rows, cols, CV_32F, data2);

	s.open(file_name_3, ios::in);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			s >> data3[i * cols + j];
		}
	}
	s.close();
	m[2] = Mat(rows, cols, CV_32F, data3);
	Mat dst;
	merge(m, 3, dst);
	delete[] data1;
	delete[] data2;
	delete[] data3;
	return dst;
}

