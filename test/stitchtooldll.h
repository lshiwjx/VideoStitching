// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� STITCHTOOLDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// STITCHTOOLDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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

// �����Ǵ� StitchToolDll.dll ������
class STITCHTOOLDLL_API CStitchToolDll {
public:
	CStitchToolDll(void);
	// Functions
	bool read_image(std::vector<std::string>);
	bool read_video(std::vector<std::string>);
	bool stitch_image();
	bool stitch_video();
	bool sychronize_video(std::vector<std::string>);
	//�Ƿ�ʹ��CUDA
	bool try_cuda = false;
	//��Ƶͬ���Ĳ���
	int analyse_time = 20;
	int sample_time = 40;
	int sample_rate = 44100;
	int filter_rate = 8000;
	int fps = 30;
	std::vector<int> frame_delay{ 0,0,0,0 };
	//ͼ��ƴ�ӵĲ���
	int num_images = 0;
	int length = 1920;
	int height = 1080;
	const cv::detail::WaveCorrectKind wave_correct = cv::detail::WAVE_CORRECT_HORIZ;
	const int expos_comp_type = cv::detail::ExposureCompensator::GAIN;  // _BLOCKS;	
	double feature_scale = 1, seam_scale = 1, compose_scale = 0.7;
	float conf_thresh = 0.5;//BA
	float blend_strength = 5;	//�ں�ǿ��
	float match_conf = 0.35;     //����������Ƴ̶�
	std::string seam_find_type = "dp_colorgrad";  // "gc_color";
	std::string features_type = "surf";//surf orb surf���������Դ�С������
	std::string warp_type = "spherical";  //stereographic(����),fisheye,plane,cylindrical(Բ��),spherical(����)	
	std::string ba_cost_func = "reproj"; //reproj ray
	//ͼ���и�Ĳ���
	std::vector<int> cutX;
	std::vector<int> cutY;
	int cutZ = 20;
	int cutW = 0;
	//��������
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

