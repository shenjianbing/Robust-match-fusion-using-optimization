#include "tools.h"
#include "fusion.h"
#include "readImg.h"
#include "connect.h"
#include "iterate.h"
#include "colorTransform.h"
#include "stdlib.h"
using namespace cv;
using namespace std;

const int PIXEL_SIZE = 7;
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "input arguments needed" << endl;
		return -1;
	}
	string folder = argv[1];
	char * img_filename_1 = "1.jpg";
	char * img_filename_2 = "2.jpg";
	char * img_filename_3 = "3.jpg";
	Mat m[3];
	ColorTransform ct23((folder + '/' + img_filename_2).c_str(), img_filename_3);
	ColorTransform ct21((folder + '/' + img_filename_2).c_str(), img_filename_1);
	//--------------------reading data----------------------------------
	cout << "read data : start" << endl;
	m[0] = imread_CV32FC3(folder + '/' + img_filename_1);
	m[1] = imread_CV32FC3(folder + '/' + img_filename_2);
	m[2] = imread_CV32FC3(folder + '/' + img_filename_3);
	
	Mat detail, gas;
	GaussianBlur(m[1], gas, Size(3, 3), -1);
	detail = m[1] - gas;

	Mat rel21 = readRel(m[0].cols, m[0].rows, (folder + '/' + img_filename_2).c_str(), img_filename_1);
	Mat rel23 = readRel(m[0].cols, m[0].rows, (folder + '/' + img_filename_2).c_str(), img_filename_3);
	cout << "read data : finish" << endl;
	
	//-------------------prepare----------------------------------------
	Mat mark = Mat::zeros(m[0].rows, m[0].cols, CV_8U);
	Mat dst[3];
	dst[0] = m[1].clone();
	dst[1] = m[1].clone();
	dst[2] = m[1].clone();
	int c = 0, count = 0;
	Mat * weight  = NULL;

	//-------------------iterate 1->0-----------------------------------
	cout << "process : start" << endl;
	do 
	{
		c = iterate(dst[0], m[1], m[0], rel21, PIXEL_SIZE, 0.85, mark, weight, 1, 0);
	} while (c >= 10 && ++count < 3);
	ct21.matTransformWithReplace(dst[0], mark);
	//-------------------iterate 1->2-----------------------------------
	mark = Mat::zeros(m[0].rows, m[0].cols, CV_8U);
	count = 0;
	do
	{
		c = iterate(dst[2],m[1], m[2], rel23, PIXEL_SIZE, 0.85, mark, weight, 1, 2);
	} while (c >= 10 && ++count < 3);
	ct23.matTransformWithReplace(dst[2], mark);
	cout << "process : finish" << endl;
	//-------------------show output-------------------------------------
	Mat result = fusion(dst, 3);
	Mat res2;
	GaussianBlur(result, res2, Size(3, 3), -1);
	imwrite((folder + '/' + "result" + ".jpg").c_str(), (res2 + detail) * 255);
	imshow("result", res2 + detail);
	system((string("del ") + folder + "\\*.rel").c_str());
	waitKey();
	return 0;
}
