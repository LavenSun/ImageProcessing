#include<opencv2/core.hpp>
#include <opencv2/highgui.hpp> 
#include<opencv2/imgproc.hpp>
using namespace cv;
int main(int argc, char* argv[])
{
	Mat img = imread("C:\\Users\\57035\\Pictures\\Saved Pictures\\苹果总部.jpg");//读取目标图片JPG
	//Mat rImg;
	//rotate(img, rImg, ROTATE_90_CLOCKWISE);
	imshow("原图", img);
	//imshow("旋转", rImg);
	waitKey(0);
	return 0;
}
	
