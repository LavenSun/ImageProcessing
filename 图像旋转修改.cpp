#include<opencv2/core.hpp>
#include <opencv2/highgui.hpp> 
#include<opencv2/imgproc.hpp>
using namespace cv;
int main(int argc, char* argv[])
{
	Mat img = imread("C:\\Users\\57035\\Pictures\\Saved Pictures\\ƻ���ܲ�.jpg");//��ȡĿ��ͼƬJPG
	//Mat rImg;
	//rotate(img, rImg, ROTATE_90_CLOCKWISE);
	imshow("ԭͼ", img);
	//imshow("��ת", rImg);
	waitKey(0);
	return 0;
}
	
