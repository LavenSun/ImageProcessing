#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<stdio.h>
using namespace std;
using namespace cv;
void TouShenBianHuan()
{
    Mat src = imread("//home//laven//CodePractice//0925_2.jpg");
    //imshow("test", src);
    int img_height = src.rows;
	int img_width = src.cols;
    vector<Point2f> corners(4);
    //左上 右上 左下 右下
	corners[0] = Point2f(27.5, 25);
	corners[1] = Point2f(1892, 217);
	corners[2] = Point2f(13.5, 1369);
	corners[3] = Point2f(1858, 900);
	vector<Point2f> corners_trans(4);
	corners_trans[0] = Point2f(0, 500);
	corners_trans[1] = Point2f(900, 500);
	corners_trans[2] = Point2f(0, 1000);
	corners_trans[3] = Point2f(900, 1000);
	Mat transform = getPerspectiveTransform(corners, corners_trans);
	cout << transform << endl;
	vector<Point2f> ponits, points_trans;
	for(int i = 0; i < img_height; i++)
    {
		for(int j = 0; j < img_width; j++)
		{
			ponits.push_back(Point2f(j,i));
		}
	}
	perspectiveTransform(ponits, points_trans, transform);
	Mat img_trans = Mat::zeros(img_height,img_width,CV_8UC3);
	int count = 0;
	for(int i = 0; i < img_height; i++)
    {
        uchar* p = src.ptr<uchar>(i);
		for(int j = 0; j < img_width; j++)
		{
			int y = points_trans[count].y;
			int x = points_trans[count].x;
			uchar* t = img_trans.ptr<uchar>(y);
			t[x*3]  = p[j*3];
			t[x*3+1]  = p[j*3+1];
			t[x*3+2]  = p[j*3+2];
			count++;
		}
	}
	imshow("test", img_trans);
	//imwrite("boy_trans.png",img_trans);
	waitKey();
}
void ZhiFangTuJunHengHua()
{
    Mat img = imread("//home//laven//CodePractice//0925_2.jpg");
    cvtColor(img, img, CV_BGR2GRAY);
    imshow("test",img);
    Mat res;
    equalizeHist(img, res); //直方图均衡化
    imshow("res", res);
    waitKey();
}
void TouShenBianHuan1()
{
    Mat src = imread("//home//laven//CodePractice//0925_2.jpg");
    imshow("test1", src);
	int img_height = src.rows;
	int img_width = src.cols;
	vector<Point2f>src_corners(4);
	src_corners[0] = Point2f(27.5, 25);
	src_corners[1] = Point2f(1892, 217);
	src_corners[2] = Point2f(13.5, 1369);
	src_corners[3] = Point2f(1858, 900);
	circle(src, src_corners[0], 3, Scalar(0, 0, 255), 3, 8);
	circle(src, src_corners[1], 3, Scalar(0, 0, 255), 3, 8);
	circle(src, src_corners[2], 3, Scalar(0, 0, 255), 3, 8);
	circle(src, src_corners[3], 3, Scalar(0, 0, 255), 3, 8);
	vector<Point2f>dst_coners(4);
    dst_coners[0] = Point2f(0, 500);
	dst_coners[1] = Point2f(900, 500);
	dst_coners[2] = Point2f(0, 1000);
	dst_coners[3] = Point2f(900, 1000);
	Mat warpMatrix = getPerspectiveTransform(src_corners, dst_coners);
	Mat dst;
	warpPerspective(src, dst, warpMatrix, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
	imshow("test2", dst);
	waitKey();
}
int main()
{
    //ZhiFangTuJunHengHua();
    //TouShenBianHuan();
    TouShenBianHuan1();
    return 0;
}
