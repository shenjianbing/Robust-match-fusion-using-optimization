#include "readImg.h"
using namespace cv;
using namespace std;
//¶ÁÍ¼ img
int readimg(const char * pathname, Mat img[], Size newSize, bool isResize)
{
	int r = newSize.height;
	int c = newSize.width;
	int N = 0;
	char * path = new char[20];
	strcpy(path, pathname);
	if (pathname && strlen(pathname) > 0)
	{
		strcat(path, "/1.jpg");
	}
	else
	{
		strcpy(path, "1.jpg");
	}
	Mat temp;
	temp = imread(path);
	if (isResize)
	{
		resize(temp, img[N], Size(c, r));
	}
	else
	{
		img[N] = temp.clone();
	}
	while (img[N].data)
	{
		path[strlen(path) - 5]++;
		N++;
		temp = imread(path);
		if (!temp.data)
		{
			break;
		}
		if (isResize)
		{
			resize(temp, img[N], Size(c, r));
		}
		else
		{
			img[N] = temp.clone();
		}
	}
	for (int i = 0; i < N; i++)
	{
		img[i].convertTo(img[i], CV_32FC3, 1.0 / 255.0);
	}
	delete [] path;
	return N;
}