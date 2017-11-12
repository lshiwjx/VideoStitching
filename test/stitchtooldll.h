// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 STITCHTOOLDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// STITCHTOOLDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef STITCHTOOLDLL_EXPORTS
#define STITCHTOOLDLL_API __declspec(dllexport)
#else
#define STITCHTOOLDLL_API __declspec(dllimport)
#endif
#pragma once

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
#include "opencv2/stitching.hpp"
#include "opencv2/ccalib.hpp"
#include "opencv2/xfeatures2d.hpp"

// 此类是从 StitchToolDll.dll 导出的
class STITCHTOOLDLL_API CStitchToolDll {
public:
	CStitchToolDll(void);
	// Functions
	bool read_image(std::vector<std::string>);
	bool read_video(std::vector<std::string>);
	bool stitch_image();
	bool stitch_video();
	bool sychronize_video(std::vector<std::string>);
	//是否使用CUDA
	bool try_cuda = false;
	//视频同步的参数
	int analyse_time = 20;
	int sample_time = 40;
	int sample_rate = 44100;
	int filter_rate = 8000;
	int fps = 30;
	std::vector<int> frame_delay{ 0,0,0,0 };
	//图像拼接的参数
	int num_images = 0;
	int length = 1920;
	int height = 1080;
	const cv::detail::WaveCorrectKind wave_correct = cv::detail::WAVE_CORRECT_HORIZ;
	const int expos_comp_type = cv::detail::ExposureCompensator::GAIN;  // _BLOCKS;	
	double feature_scale = 1, seam_scale = 1, compose_scale = 0.7;
	float conf_thresh = 0.5;//BA
	float blend_strength = 5;	//融合强度
	float match_conf = 0.35;     //特征点的相似程度
	std::string seam_find_type = "dp_colorgrad";  // "gc_color";
	std::string features_type = "surf";//surf orb surf快两倍，对大小不敏感
	std::string warp_type = "spherical";  //stereographic(立体),fisheye,plane,cylindrical(圆柱),spherical(球面)	
	std::string ba_cost_func = "reproj"; //reproj ray
	//图像切割的参数
	std::vector<int> cutX;
	std::vector<int> cutY;
	int cutZ = 20;
	int cutW = 0;
	//各种容器
	std::vector<cv::Mat> image;
	std::vector<cv::VideoCapture> video;
	cv::Mat img_final_stitched;
	std::vector<cv::detail::CameraParams> cameras;
	cv::Ptr<cv::detail::RotationWarper> warper;
	cv::Ptr<cv::detail::ExposureCompensator> compensator = cv::detail::ExposureCompensator::createDefault(expos_comp_type);
	std::vector<cv::Point> corners;
	std::vector<cv::Mat> mask_warped;
	cv::Ptr<cv::detail::Blender> blender;
	std::vector<cv::Size> sizes;
	std::vector<cv::Mat> map1, map2;
};

extern STITCHTOOLDLL_API int nStitchToolDll;

