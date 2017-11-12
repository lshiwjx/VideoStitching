#pragma once
#include <qmessagebox.h>
#include "qstring.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat\avformat.h"
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/avfiltergraph.h>
#include <libavutil/opt.h>
}
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/stitching.hpp"
#include "stitch/precomp.hpp"
#include "opencv2/ccalib.hpp"
#include "opencv2/xfeatures2d.hpp"
//#include "opencv2\stitching\detail\timelapsers.hpp"

using namespace std;
using namespace cv;
using namespace cv::detail;

class StitchTool
{
public:
	const bool try_cuda = false;
	
	const WaveCorrectKind wave_correct = detail::WAVE_CORRECT_HORIZ;
	const int expos_comp_type = ExposureCompensator::GAIN;  // _BLOCKS;	

	double feature_scale = 1, seam_scale = 1, compose_scale = 0.7;
	float conf_thresh = 0.5;//BA
	float blend_strength = 5;	//融合强度
	float match_conf = 0.35;     //特征点的相似程度

	string seam_find_type = "dp_colorgrad";  // "gc_color";
	string features_type = "surf";//surf orb surf快两倍，对大小不敏感
	string warp_type = "spherical";  //stereographic(立体),fisheye,plane,cylindrical(圆柱),spherical(球面)	
	string ba_cost_func = "reproj"; //reproj ray

	int num_images = 0;
	vector<int> cutX;
	vector<int> cutY;
	int cutZ = 20;
	int cutW = 0;
	vector<int> frame_delay{0,0,0,0};

	vector<Mat> image;
	vector<VideoCapture> video;

	Mat img_final_stitched;

	vector<CameraParams> cameras;
	Ptr<RotationWarper> warper;
	Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(expos_comp_type);
	vector<Point> corners;
	vector<Mat> mask_warped;
	Ptr<Blender> blender;
	vector<Size> sizes;
	vector<cv::Mat> map1, map2;


	bool readImage( QStringList );
	bool readVideo( QStringList );
	bool stitchImage();
	bool stitchVideo();
	bool sychronizeVideo(QStringList);
	StitchTool();
	~StitchTool();
};

