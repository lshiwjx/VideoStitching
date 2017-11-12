#include<iostream>
#include<vector>
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
#include "StitchToolDll.h"
#define ANALISE_TIME 20
#define SAMPLE_TIEM 40
#define SAMPLE_RATE 44100
#define FILTER_RATE 8000
#define FPS 30
#define LENGTH 1920
#define HIGHT 1080
bool sychronize_video(std::vector<std::string> filename_list, int size);

int main() {

	CStitchToolDll stitch_tool;
	std::vector<std::string> name = { "../data/1.mp4","../data/2.mp4" ,"../data/3.mp4" ,"../data/4.mp4" };
	if (!stitch_tool.sychronize_video(name)) std::cout << "error" << std::endl;
	std::getchar();
	return 0;
}

bool sychronize_video(std::vector<std::string> filename_list, int size) {
	if (size != 4)return false;
	std::vector<std::int16_t> result[4];
	av_register_all();
	avfilter_register_all();

	for (int index = 0; index < 4; index++) {
		AVFormatContext *fmt_ctx = avformat_alloc_context();
		int audio_stream_index = -1;
		AVCodecContext *dec_ctx = NULL;
		AVFilterContext *buffersink_ctx;
		AVFilterContext *buffersrc_ctx;
		AVFilterGraph *filter_graph;
		AVPacket *packet = av_packet_alloc();
		AVFrame *filt_frame = av_frame_alloc();
		AVFrame *frame = av_frame_alloc();

		std::string filename = filename_list[index];
		int ret;
		AVCodec *dec;
		filter_graph = avfilter_graph_alloc();
		//------------------init codec-------------
		if ((ret = avformat_open_input(&fmt_ctx, filename.c_str(), NULL, NULL)) < 0) {
			std::cout<< "Cannot open input file" << std::endl;
			return ret;
		}
		if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
			std::cout << "Cannot find stream information" << std::endl;
			return ret;
		}
		if ((audio_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0)) < 0) {
			std::cout << "Cannot find an audio stream in the input file" << std::endl;
			return audio_stream_index;
		}
		dec_ctx = fmt_ctx->streams[audio_stream_index]->codec;
		if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
			std::cout << "Cannot open audio decoder" << std::endl;
			return ret;
		}
		//----------------init filter--------------
		char args[512];
		static const enum AVSampleFormat out_sample_fmts[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
		static const int64_t out_channel_layouts[] = { AV_CH_LAYOUT_MONO, -1 };
		static const int out_sample_rates[] = { FILTER_RATE, -1 };
		const AVFilterLink *outlink;
		AVRational time_base = fmt_ctx->streams[audio_stream_index]->time_base;
		AVFilter *abuffersrc = avfilter_get_by_name("abuffer");
		AVFilter *abuffersink = avfilter_get_by_name("abuffersink");
		AVFilterInOut *outputs = avfilter_inout_alloc();
		AVFilterInOut *inputs = avfilter_inout_alloc();
		if (!dec_ctx->channel_layout)
			dec_ctx->channel_layout = av_get_default_channel_layout(dec_ctx->channels);
		snprintf(args, sizeof(args),"time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=%d",time_base.num, time_base.den, dec_ctx->sample_rate,av_get_sample_fmt_name(dec_ctx->sample_fmt), dec_ctx->channel_layout);
		ret = avfilter_graph_create_filter(&buffersrc_ctx, abuffersrc, "in", args, NULL, filter_graph);
		ret = avfilter_graph_create_filter(&buffersink_ctx, abuffersink, "out", NULL, NULL, filter_graph);
		ret = av_opt_set_int_list(buffersink_ctx, "sample_fmts", out_sample_fmts, -1, AV_OPT_SEARCH_CHILDREN);
		ret = av_opt_set_int_list(buffersink_ctx, "channel_layouts", out_channel_layouts, -1, AV_OPT_SEARCH_CHILDREN);
		ret = av_opt_set_int_list(buffersink_ctx, "sample_rates", out_sample_rates, -1, AV_OPT_SEARCH_CHILDREN);
		outputs->name = av_strdup("in");
		outputs->filter_ctx = buffersrc_ctx;
		outputs->pad_idx = 0;
		outputs->next = NULL;
		inputs->name = av_strdup("out");
		inputs->filter_ctx = buffersink_ctx;
		inputs->pad_idx = 0;
		inputs->next = NULL;
		static const char *filter_descr = "aresample=8000,aformat=sample_fmts=s16:channel_layouts=mono";
		if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr,
			& inputs, &outputs, NULL)) < 0)
			        return false;
		if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
			return false;
		//---------------------------------------
		while (1) {
			if ((ret = av_read_frame(fmt_ctx, packet)) < 0) {
				std::cout << "Frame read failed" << std::endl;
				return ret;
			}
			if (result[index].size() > frame->sample_rate * SAMPLE_TIEM/SAMPLE_RATE*FILTER_RATE) break;  
			if (packet->stream_index == audio_stream_index) {
				int got_frame = 0;
				ret = avcodec_decode_audio4(dec_ctx, frame, &got_frame, packet);
				if (got_frame) {
					ret = av_buffersrc_add_frame_flags(buffersrc_ctx, frame, 0);
					ret = av_buffersink_get_frame(buffersink_ctx, filt_frame);
					int data_size = av_samples_get_buffer_size(NULL, dec_ctx->channels,
						filt_frame->nb_samples,
						dec_ctx->sample_fmt, 1);
					int16_t *p = (int16_t*)filt_frame->data[0];
					int16_t *p_end = p + filt_frame->nb_samples*av_get_channel_layout_nb_channels(av_frame_get_channel_layout(filt_frame));
					while (p < p_end) result[index].push_back(*p++);
				}
			}
		}
		av_packet_free(&packet);
		av_frame_free(&frame);
		avformat_close_input(&fmt_ctx);
		av_frame_free(&filt_frame);
		avfilter_graph_free(&filter_graph);
	}

	std::vector<int16_t> result_filtered[4] = { result[0],result[1],result[2],result[3] };
	/*for(int i = 0; i < 4; i++)
		for (int j = 2; j < result[i].size() - 2; j++) {
			std::vector<int16_t> sort_vec = { result[i][j - 2],result[i][j - 1],result[i][j],result[i][j + 1],result[i][j + 2] };
			std::sort(sort_vec.begin(), sort_vec.end());
			result_filtered[i][j - 2] = (sort_vec[1]+sort_vec[2]+sort_vec[3])/3;
			//result_filtered[i][j] = (result[i][j - 2] + result[i][j - 1] + result[i][j] + result[i][j + 1] + result[i][j + 2]) / 5;
		}*/




	std::ofstream out_file[4];
	//int dist[4] = { 0,0,0,0 };
	for (int i = 0; i < 4; i++) {
		char temp[4];
		std::string str = "audio";
		itoa(i, temp, 10);
		str += temp;
		str += ".txt";
		out_file[i].open(str, std::ios::out);
		copy(result_filtered[i].begin(), result_filtered[i].end(), std::ostream_iterator<std::int16_t>(out_file[i], "\n"));
		out_file[i].close();
		//biggest[i] = std::max_element(std::begin(result_filtered[i]), std::end(result_filtered[i]));
		//dist[i] = std::distance(std::begin(result_filtered[i]), biggest[i]);
	}
	//int mindist = std::min(dist[0], dist[3]);
	//for (int i = 0; i < 4; i++) {
	//	frame_delay[i] = int((dist[i] - mindist) * FPS / SAMPLE_RATE);
	//}
	//std::cout << frame_delay[0] << ' ' << frame_delay[1] << ' ' << frame_delay[2] << ' ' << frame_delay[3] << std::endl;
	std::vector<int> frame_delay{ 0 ,0, 0, 0 };
	std::vector<int>::iterator biggest[4];
	int dist_max[3];
	for (int i = 0; i < 3; i++) {
		std::vector<int> dist_tmp;
		for (int j = 0; j < 400; j++) {//只分析400帧
			dist_tmp.push_back(0);
			for (int k = 0; k < FILTER_RATE*ANALISE_TIME; k++) {//比较数据的长度
				int tmp = (result_filtered[i + 1][j*(FILTER_RATE / FPS) + k] * result_filtered[0][200 * FILTER_RATE / FPS + k]);
				dist_tmp[j] += tmp/100;
			}
		}
		biggest[i] = std::max_element(std::begin(dist_tmp), std::end(dist_tmp));
		std::cout << *biggest[i] << std::endl;
		dist_max[i] = std::distance(std::begin(dist_tmp), biggest[i]);
	}
	for (int i = 1; i < 4; i++) {
		frame_delay[i] = dist_max[i - 1] - 200;
	}
	int min = *std::min(frame_delay.begin(), frame_delay.end());
	for (int i = 1; i < 4; i++) {
		frame_delay[i] += min;
	}
	std::cout << frame_delay[0] << ' ' << frame_delay[1] << ' ' << frame_delay[2] << ' ' << frame_delay[3] << std::endl;
	std::getchar();
	return true;
}