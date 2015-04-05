#include "common.h"

extern "C"{
#include <vl/generic.h>
#include <vl/stringop.h>
#include <vl/pgm.h>
#include <vl/sift.h>
#include <vl/getopt_long.h>
};

void main(int argc, char *argv[])
{
	VL_PRINT ("Hello world!\n") ;

	char * imfn = argv[1];
	
	Mat Image = imread(imfn, CV_LOAD_IMAGE_GRAYSCALE);
	
	int noctaves=4,nlevels=2,o_min=0;
	int height = Image.rows;
	int width = Image.cols;

	vl_sift_pix *ImageData=new vl_sift_pix[height*width];
	for (int i=0;i<height;i++)     //y
	{
		for (int j=0;j<width;j++)  //x
		{
			ImageData[i*width + j]= Image.at<uchar>(i,j);
		}
	}

	VlSiftFilt *SiftFilt=NULL;
	SiftFilt=vl_sift_new(width,height,noctaves,nlevels,o_min);
	
	int KeyPoint=0;
	int idx=0;
	if (vl_sift_process_first_octave(SiftFilt,ImageData)!=VL_ERR_EOF)
	{
		while (TRUE)
		{
			//计算每组中的关键点
			vl_sift_detect(SiftFilt);

			//遍历并绘制每个点
			KeyPoint += SiftFilt->nkeys;
			VlSiftKeypoint *pKeyPoint = SiftFilt->keys;
			for (int i=0;i<SiftFilt->nkeys;i++)
			{
				VlSiftKeypoint TemptKeyPoint=*pKeyPoint;
				pKeyPoint++;
				cv::circle(Image,cv::Point(TemptKeyPoint.x,TemptKeyPoint.y),TemptKeyPoint.sigma/2,CV_RGB(255,0,0));
				idx++;
				
				//计算并遍历每个点的方向
				double angles[4];
				int angleCount=vl_sift_calc_keypoint_orientations(SiftFilt,angles,&TemptKeyPoint);
				for (int j=0;j<angleCount;j++)
				{
					double TemptAngle=angles[j];
					//printf("%d: %f\n",j,TemptAngle);
				
					//计算每个方向的描述
					float *Descriptors=new float[128];
					//vl_sift_calc_keypoint_descriptor(SiftFilt,Descriptors,&TemptKeyPoint,TemptAngle);
					int k=0;
					while (k<128)
					{
						//printf("%d: %f",k,Descriptors[k]);
						//printf("; ");
						k++;
					}

					printf("\n");
					delete []Descriptors;
					Descriptors=NULL;
				}
			}
			//下一阶
			if (vl_sift_process_next_octave(SiftFilt)==VL_ERR_EOF)
			{
				break;
			}
			//free(pKeyPoint);
			KeyPoint=NULL;
		}
	}
	cout << "keypoint = " << KeyPoint << endl;
	cout << "idx = " << idx << endl;
	vl_sift_delete(SiftFilt);
	
	delete []ImageData;
	ImageData=NULL;
	
	cv::namedWindow("Source Image",1);
	cv::imshow("Source Image",Image);
	cv::waitKey(0);

	return;
}
