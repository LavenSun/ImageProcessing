#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <stdio.h>
#include "opencv2/calib3d/calib3d.hpp"
const int MAX_CORNERS = 500;
#define LOG printf
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("usage: pic_reg.exe [file.avi]\n");
		return 0;
	}
	char* filename = argv[1];
	CvCapture* pCapture = NULL;
	if (!(pCapture = cvCaptureFromAVI(filename)))
	{
		LOG("error load file");
		return 0;
	}
	const char* SZ_WINDOW_ORIG = "original";
	const char* SZ_WINDOW_FIRST_FRAME = "firstframe";
	const char* SZ_WINDOW_REG = "transformed";
	cvNamedWindow(SZ_WINDOW_ORIG, 1);
	cvNamedWindow(SZ_WINDOW_FIRST_FRAME, 1);
	cvNamedWindow(SZ_WINDOW_REG, 1);
	IplImage* pCurFrame;
	int iFrame = 0;
	IplImage* imgA;
	IplImage* imgB;
	IplImage* imgTmp;
	IplImage* eig_image;
	IplImage* tmp_image;
	IplImage* imgC, *imgXform;
	CvMat* M = cvCreateMat(3, 3, CV_32FC1);
	CvPoint2D32f cornersA[MAX_CORNERS];
	CvPoint2D32f cornersB[MAX_CORNERS];
	CvPoint2D32f matchCornersA[MAX_CORNERS];
	CvPoint2D32f matchCornersB[MAX_CORNERS];
	bool bRun = true;
	while ((pCurFrame = cvQueryFrame(pCapture)) != NULL && bRun)
	{
		//initialize,load two images from the file system, and allocate the images and other   structures we will need for results		
		if (iFrame == 0)
		{
			// store first image, we will adjust other frames according to this one			
			// if you want to adjust frame according to last adjacent one, you should change the code			
			imgA = cvCreateImage(cvGetSize(pCurFrame), pCurFrame->depth, 1);
			imgC = cvCreateImage(cvGetSize(pCurFrame), pCurFrame->depth, 1);
			imgB = cvCreateImage(cvGetSize(pCurFrame), pCurFrame->depth, 1);
			imgXform = cvCreateImage(cvGetSize(pCurFrame), pCurFrame->depth, 1);
			//imgB = pCurFrame;			
			cvCvtColor(pCurFrame, imgA, CV_BGR2GRAY);
			eig_image = cvCreateImage(cvGetSize(pCurFrame), IPL_DEPTH_32F, 1);
			tmp_image = cvCreateImage(cvGetSize(pCurFrame), IPL_DEPTH_32F, 1);
		}
		else
		{
			CvSize img_sz = cvGetSize(imgA);
			int win_size = 10;
			cvCvtColor(pCurFrame, imgB, CV_BGR2GRAY);
			cvCopy(imgB, imgC);
			//the first thing we need to do is get the features we want to track			
			// you can move this block in iframe == 0 block above, to reduce the calculation			
			int corner_count = MAX_CORNERS;
			cvGoodFeaturesToTrack(imgA, eig_image, tmp_image, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04);
			cvFindCornerSubPix(imgA, cornersA, corner_count, cvSize(win_size, win_size), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
			//call Lucas Kanade algorithm			
			char feature_found[MAX_CORNERS];
			float feature_errors[MAX_CORNERS];
			CvSize pyr_sz = cvSize(imgA->width + 8, imgB->height / 3);
			IplImage* pyrA = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
			IplImage* pyrB = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
			cvCalcOpticalFlowPyrLK(imgA, imgB, pyrA, pyrB, cornersA, cornersB, corner_count, cvSize(win_size, win_size), 5, feature_found, feature_errors, cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3), 0);
			//now make some image of what we are looking at			
			int matchCornersNum = 0;
			for (int i = 0; i < corner_count; i++)
			{
				if (feature_found[i] == 0 || feature_errors[i] > 550)
				{
					printf("Error is %f\n", feature_errors[i]);
					continue;
				}
				// reserve only matched points				
				matchCornersA[matchCornersNum] = cornersA[i];
				matchCornersB[matchCornersNum] = cornersB[i];
				matchCornersNum++;
				CvPoint p0 = cvPoint(cvRound(cornersA[i].x), cvRound(cornersA[i].y));
				CvPoint p1 = cvPoint(cvRound(cornersB[i].x), cvRound(cornersB[i].y));
				cvLine(imgB, p0, p1, CV_RGB(255, 0, 0), 2);
				cvCircle(imgB, p0, 5, CV_RGB(0, 255, 0), 1);
			}
			// prepare data to find homography			
			CvMat* srcpoint = cvCreateMat(matchCornersNum, 2, CV_32FC1);
			CvMat* dstpoint = cvCreateMat(matchCornersNum, 2, CV_32FC1);
			cvInitMatHeader(srcpoint, matchCornersNum, 2, CV_32FC1, matchCornersB);
			cvInitMatHeader(dstpoint, matchCornersNum, 2, CV_32FC1, matchCornersA);
			LOG("[%f,%f,%f\n", CV_MAT_ELEM(*M, float, 0, 0), CV_MAT_ELEM(*M, float, 0, 1), CV_MAT_ELEM(*M, float, 0, 2));
			LOG(" %f,%f,%f\n", CV_MAT_ELEM(*M, float, 1, 0), CV_MAT_ELEM(*M, float, 1, 1), CV_MAT_ELEM(*M, float, 1, 2));
			LOG(" %f,%f,%f]\n", CV_MAT_ELEM(*M, float, 2, 0), CV_MAT_ELEM(*M, float, 2, 1), CV_MAT_ELEM(*M, float, 2, 2));
			// find homography, if you have opencv > 1.0, you can use Ransac to do a better calculation			
			cvFindHomography(srcpoint, dstpoint, M);
			// transform the original image			
			cvWarpPerspective(imgC, imgXform, M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0)); 			// show them
			// show them
			cvShowImage(SZ_WINDOW_FIRST_FRAME, imgA);
			cvShowImage(SZ_WINDOW_ORIG, imgB);
			cvShowImage(SZ_WINDOW_REG, imgXform);
			// release
			cvReleaseMat(&srcpoint);
			cvReleaseMat(&dstpoint);
			cvReleaseImage(&pyrA);
			cvReleaseImage(&pyrB);
		}
		iFrame++;
		int k = cvWaitKey(100);
		switch ((char)k)
		{
		case 'q':
			bRun = false;
			break;
		}
	}
	cvReleaseMat(&M);
	cvReleaseImage(&imgA);
	cvReleaseImage(&imgB);
	cvReleaseImage(&imgXform);
	cvReleaseImage(&imgC);
	cvReleaseImage(&eig_image);
	cvReleaseImage(&tmp_image);
	return 0;
}
