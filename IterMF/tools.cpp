#include "tools.h"
const float PI = 3.1415926;
#define MIN_FLOAT (numeric_limits<float>::min())
cv::Vec3f similarity_measurement( const Mat& re/*ref*/,const Mat& im, float ar, float br, float cr, float as, float bs, float cs, float gamma /*= 2*/ )
{
	if (re.size() != im.size())
	{
		cout << "similarity_measurement size error" << endl;
		exit(-1);
	}
	Mat vr(re.size(), re.type());
	Mat vs(re.size(), re.type());
	int R = vr.size().height;
	int C = vr.size().width;
	int count = R * C;
	Mat_<Vec3f>::const_iterator itre = re.begin<Vec3f>();
	Mat_<Vec3f>::const_iterator itim = im.begin<Vec3f>();
	for (Mat_<Vec3f>::iterator itvr = vr.begin<Vec3f>(), itvs = vs.begin<Vec3f>(); itvr != vr.end<Vec3f>(); ++itim, ++itre, ++itvr, ++itvs)
	{
		float tr = pow((*itre)[0] * (*itre)[1] * (*itre)[2], 1.0 / 3); 
		tr += MIN_FLOAT;
		float tr2 = ar / pow(ar * br * cr, 1.0 / 3);
		(*itvr)[0] = tr2 + gamma * log((*itre)[0] + MIN_FLOAT / tr);
		(*itvr)[1] = tr2 + gamma * log((*itre)[1] + MIN_FLOAT / tr);
		(*itvr)[2] = tr2 + gamma * log((*itre)[2] + MIN_FLOAT / tr);
		float ts = pow((*itim)[0] * (*itim)[1] * (*itim)[2], 1.0 / 3);
		ts += MIN_FLOAT;
		float ts2 = as / pow(as * bs * cs, 1.0 / 3);
		(*itvs)[0] = ts2 + gamma * log((*itim)[0] + MIN_FLOAT / ts);
		(*itvs)[1] = ts2 + gamma * log((*itim)[1] + MIN_FLOAT / ts);
		(*itvs)[2] = ts2 + gamma * log((*itim)[2] + MIN_FLOAT / ts);
	}
	Vec3f avgs = 0, avgr = 0, t1 = 0, t2 = 0, t3 = 0;
	for (Mat_<Vec3f>::iterator itvr = vr.begin<Vec3f>(), itvs = vs.begin<Vec3f>(); itvr != vr.end<Vec3f>(); ++itvr, ++itvs)
	{
		avgr += *itvr / (float)count;
		avgs += *itvs / (float)count;
	}
	//cout << vr << endl << vs;
	for (Mat_<Vec3f>::iterator itvr = vr.begin<Vec3f>(), itvs = vs.begin<Vec3f>(); itvr != vr.end<Vec3f>(); ++itvr, ++itvs)
	{
		//cout << *itvr - avgr << '*' << *itvs - avgs << '=' <<mul(*itvr - avgr, *itvs - avgs)<< endl;
		t1 += mul(*itvr - avgr, *itvs - avgs);
		t2 += mul(*itvr - avgr, *itvr - avgr);
		t3 += mul(*itvs - avgs, *itvs - avgs);
	}
	return div(t1, mul(sqrt(t2), sqrt(t3)));
}

/*int round(float r)
{
	return (int)((r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5));
}*/

cv::Mat imread_CV64FC3( const string& filename)
{
	Mat a = imread(filename);
	Mat b;
	a.convertTo(b, CV_64FC3, 1.0 / 255.0);
	return b;
}

cv::Mat imread_CV32FC3(const string& filename)
{
	Mat a = imread(filename);
	Mat b;
	a.convertTo(b, CV_32FC3, 1.0 / 255.0);
	return b;
}

cv::Vec3f mul( const Vec3f & a, const Vec3f & b )
{
	return Vec3f(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

cv::Vec3f mul(const Vec3f & a, const float f)
{
	return Vec3f(a[0] * f, a[1] * f, a[2] * f);
}

cv::Vec3f abs( const Vec3f & a )
{
	return Vec3f(abs(a[0]), abs(a[1]), abs(a[2]));
}

cv::Vec3f div( const Vec3f & a, const Vec3f & b )
{
	return Vec3f(a[0] / (b[0] + 0.0000001), a[1] / (b[1] + 0.0000001), a[2] / (b[2] + 0.0000001));
}

cv::Vec3f sqrt( const Vec3f & a )
{
	return Vec3f(sqrt(a[0]), sqrt(a[1]), sqrt(a[2]));
}

cv::Mat drawWithBox( Mat mat, Point2i point, Size size, Scalar color /*= Scalar(0, 0, 255)*/ )
{
	Mat dst = mat.clone();
	rectangle(dst, Rect(point, size), color);
	return dst;
}

cv::Vec3f calcSimilarity( const Mat &src, const Mat &refer, Point2i point, float theta, int tx, int ty, Size pixelSize /*= Size(7, 7)*/ )
{
	Mat s = Mat(src, Rect(point, pixelSize));
	Mat r = Mat::zeros(pixelSize, refer.type());
	float costh = cos(theta * PI / 180.0);
	float sinth = sin(theta * PI / 180.0);
	int x, y;
	int cx = point.x + pixelSize.width / 2;
	int cy = point.y + pixelSize.height / 2;
	for (int i = -pixelSize.height / 2; i < pixelSize.height - pixelSize.height / 2; ++i)
	{
		for (int j = -pixelSize.width / 2; j < pixelSize.width - pixelSize.width / 2; ++j)
		{
			x = cx + tx + round(costh * j - sinth * i);
			y = cy + ty + round(sinth * j + costh * i);
			if (x < 0 || x > src.size().width || y < 0 || y > src.size().height)
			{
				r.at<Vec3f>(i + pixelSize.height / 2, j + pixelSize.width / 2) = Vec3f(0, 0, 0);
			}
			else
			{
				r.at<Vec3f>(i + pixelSize.height / 2, j + pixelSize.width / 2) = refer.at<Vec3f>(y, x);
			}
		}
	}
	return similarity_measurement(s, r);
}

cv::Vec3f calcSimilarity(const Mat &src, const Mat &refer, Point2i point, int tx, int ty, Size pixelSize /*= Size(7, 7)*/)
{
	Mat s(src, Rect(point, pixelSize));
	Mat r(refer, Rect(point + Point2i(tx, ty), pixelSize));
	return similarity_measurement(s, r);
}

void copyPatch(Mat & dst, const Mat & src, Rect dst_p, Rect src_p)
{
	src(src_p).copyTo(dst(dst_p));
}
