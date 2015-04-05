#include "basic.h"

float idot(const cv::Vec3f a, const cv::Vec3f b)
{
	float innerp = a.val[0] * b.val[0] + a.val[1] * b.val[1] + a.val[2] * b.val[2];	
	return innerp;
}

float idot(const cv::Vec4f a, const cv::Vec4f b)
{
	float innerp = a.val[0] * b.val[0] + a.val[1] * b.val[1] + a.val[2] * b.val[2] + a.val[3] * b.val[3];
	return innerp;
}

void mul(const cv::Mat mtx, const cv::Vec4f a, cv::Vec4f& ret)
{
	if (mtx.cols != 4)
	{
		std::cerr << "can not multiply the mtx and the vector. mtx rows != 4" << std::endl;
		exit(1);
	}

	int rows = mtx.rows;
	int cols = mtx.cols;

	for (int i = 0; i < rows; ++ i)
	{
		cv::Vec4f tmpvec; 
		for(int j = 0; j < cols; ++ j)
			tmpvec.val[j] = mtx.at<float>(i, j);			

		ret.val[i] = idot(tmpvec, a);
	}
}

Scalar randomColor(RNG& rng)
{
	int icolor = (unsigned) rng;
	return Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

//enum { CV_8U=0, CV_8S=1, CV_16U=2, CV_16S=3, CV_32S=4, CV_32F=5, CV_64F=6 };
void saveMat(const Mat& m, string fn)
{
	fstream fout(fn, ios::out);
	if (fout.is_open() == false)
	{
		cout << "failed to open " << fn << endl;
		return ;
	}

	int h = m.rows;
	int w = m.cols;
	int ch = m.channels();

	//只对通道为1的矩阵进行保存
	assert(ch == 1);  
	
	fout << w << ' ' << h << endl;
	for (int y = 0; y < h; ++ y)
	{
		for (int x = 0; x < w; ++ x)
		{
			switch(m.type())
			{
			case 0: fout << m.at<  uchar >(y,x) << ' '; break;      //CV_8U
			case 1: fout << m.at<  char  >(y,x) << ' '; break;      //CV_8S
			case 2: fout << m.at< ushort >(y,x) << ' '; break;      //CV_16U
			case 3: fout << m.at<  short >(y,x) << ' '; break;      //CV_16S
			case 4: fout << m.at<   int  >(y,x) << ' '; break;      //CV_32S
			case 5: fout << m.at<  float >(y,x) << ' '; break;      //CV_32F
			case 6: fout << m.at< double >(y,x) << ' '; break;      //CV_64F
			default: break;
			}
		}
		fout << endl;
	}
	fout.close();

}