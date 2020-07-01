#include <opencv2/opencv.hpp>
#include <opencv2/videostab.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;
using namespace cv::videostab;
void processing(Ptr < IFrameSource>, string outputPath);
int main(int argc, const char ** argv)
{
	//1           准备输入视频路径                 
	//2           建立一个运动估计器                 
	//3           建立一个稳定器                 
	//4           使用创建的稳定器来稳定视频
	Ptr<IFrameSource > stabilizedFrames;
	try
	{
		//1-准备输入视频进行检查
		string inputPath;
		string outputPath;
		if (argc > 1)
		{
			inputPath = argv[1];
		}
		else
		{
			inputPath = ".\\cube4.avi";
		}
		if (argc > 2)                                                
			outputPath = argv[2];                                 
		else                                                
			outputPath = ".\\cube4_stabilized.avi";
		Ptr<VideoFileSource > source = makePtr<VideoFileSource>(inputPath);                                
		cout << "frame cout (rough):" << source->count() << endl;
		//准备运动估计器
		//首先，准备运动估计生成器
		double min_inlier_ratio = 0.1;                                 
		Ptr<MotionEstimatorRansacL2> est = makePtr<MotionEstimatorRansacL2>(MM_AFFINE);                                 
		RansacParams ransac = est->ransacParams();
		ransac.size = 3;
		ransac.thresh = 5;
		ransac.eps = 0.5;
		est->setRansacParams(ransac);
		est->setMinInlierRatio(min_inlier_ratio);
		//2  创建一个特征检查器
		int nkps = 1000;                                 
		Ptr<GoodFeaturesToTrackDetector > feature_detector = makePtr<GoodFeaturesToTrackDetector >(nkps);
		//3           创建运动估计器
		Ptr<KeypointBasedMotionEstimator> motionEstBuilder = makePtr<KeypointBasedMotionEstimator>(est);
		motionEstBuilder->setDetector(feature_detector);
		Ptr<IOutlinerRejector>();
		motionEstBuilder->setOutlinerRejector(outlinerRejector);
		//准备稳定器

	}
}
