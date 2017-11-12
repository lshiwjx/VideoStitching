#include "stitchtool.h"
#define ANALISE_TIME 20
#define SAMPLE_TIEM 40
#define SAMPLE_RATE 44100
#define FILTER_RATE 8000
#define FPS 30
#define LENGTH 1920
#define HIGHT 1080


void MessageBox(QString msg)
{
	QMessageBox msgbox;
	msgbox.setText(msg);
	msgbox.exec();
}


bool StitchTool::readImage(QStringList filename_list)
{
	Mat img_temp;
	image.clear();
	num_images = filename_list.size();
	for (int i = 0; i < filename_list.size(); i++)
	{
		img_temp = imread(filename_list[i].toStdString());
		if (img_temp.empty()) {
			MessageBox("Read image failed");
			return false;
		}
		image.push_back(img_temp);
		cv::resize(img_temp, img_temp, cv::Size(480, 270));
		cv::imshow(QString::number(i).toStdString(), img_temp);
	}
	
	if (image.empty()) return false;
	else return true;
}


bool StitchTool::readVideo(QStringList filename_list)
{

	cv::Mat img_temp;
	video.clear();
	image.clear();
	num_images = filename_list.size();
	cv::VideoCapture videoCapture;

	for (int i = 0; i < num_images; i++)
	{
		if (!videoCapture.open(filename_list[i].toStdString())) {
			MessageBox("Read video failed");
			return false;
		}
		video.push_back(videoCapture);
		video[i].set(CAP_PROP_POS_FRAMES, frame_delay[i]);
		//video[i] >> img_temp;
		if (!video[i].retrieve(img_temp)) {
			MessageBox("Read first image of video failed");
			return false;
		}
		image.push_back(img_temp.clone());
		//test
		//cv::resize(img_temp, img_temp, cv::Size(480, 270));
		//cv::imshow(QString::number(i).toStdString(), img_temp);
	}
	if (image.empty()) return false;
	else return true;
}


bool StitchTool::stitchImage()
{
	if (num_images < 2) {
		MessageBox("Image is not enough");
		return false;
	}
	img_final_stitched.release();
	vector<cv::Mat> resize_img_temp(num_images), full_img_temp(num_images);
	vector<cv::Mat> camera_matrix(num_images), distortion_coefficients(num_images);

	//读入摄像机矫正配置文件并矫正图像
	for (int i = 0; i < num_images; i++)
	{
		map1.push_back(cv::Mat());
		map2.push_back(cv::Mat());
		const string inputSettingsFile = ("out_camera_data" + QString::number(i + 1).toStdString() + ".xml");
		cv::FileStorage fs(inputSettingsFile, cv::FileStorage::READ);
		if (!fs.isOpened()){
			MessageBox("Could not open the configuration file");
			return false;
		}
		fs["camera_matrix"] >> camera_matrix[i];
		fs["distortion_coefficients"] >> distortion_coefficients[i];
		fs.release();

		initUndistortRectifyMap(
				camera_matrix[i], distortion_coefficients[i], Mat(),
				getOptimalNewCameraMatrix(camera_matrix[i], distortion_coefficients[i], image[i].size(), 1, image[i].size(), 0), image[i].size(),
				CV_16SC2, map1[i], map2[i]);
		remap(image[i], full_img_temp[i], map1[i], map2[i], INTER_LINEAR);
		for (int j = 0, flag = 0; j < full_img_temp[i].cols; j++) {
			if (full_img_temp[i].at<cv::Vec3b>(full_img_temp[i].rows / 2, j) == cv::Vec3b(0, 0, 0))continue;
			cutX.push_back(j + 1);
			break;
		}
		for (int j = 0, flag = 0; j < full_img_temp[i].rows; j++) {
			if (full_img_temp[i].at<cv::Vec3b>(j, full_img_temp[i].cols / 2) == cv::Vec3b(0, 0, 0))continue;
			cutY.push_back(j + 1);
			break;
		}
		full_img_temp[i] = cv::Mat(full_img_temp[i], cv::Rect(cutX[i], cutY[i], full_img_temp[i].cols - 2 * cutX[i], full_img_temp[i].rows - 2 * cutY[i]));

		//cv::imshow("init"+QString::number(i).toStdString(), full_img_temp[i]);
		//cv::waitKey(1);
	}
	camera_matrix.clear();
	distortion_coefficients.clear();

	//特征提取
	Ptr<FeaturesFinder> finder;
	if (features_type == "surf") finder = makePtr<SurfFeaturesFinder>();
	else if (features_type == "orb") finder = makePtr<OrbFeaturesFinder>();
	else {
		MessageBox("Unknown 2D features type: " + QString::fromStdString(features_type));
		return false;
	}
	vector<ImageFeatures> features(num_images);
	
    #pragma omp parallel for
	for (int i = 0; i < num_images; ++i) {
		cv::resize(full_img_temp[i], resize_img_temp[i], Size(), feature_scale, feature_scale);
		//vector<cv::Rect2i> tmp;
		//tmp.push_back(cv::Rect2i(cutX[i], cutY[i], resize_img_temp[i].cols - 2 * cutX[i], resize_img_temp[i].rows - 2 * cutY[i]));
		(*finder)(resize_img_temp[i], features[i], i);

		cv::Mat draw;
		drawKeypoints(resize_img_temp[i], features[i].keypoints, draw);
		cv::imshow("features"+i, draw);
		cv::waitKey(1);
	}
	finder->collectGarbage();

	//特征匹配
	vector<MatchesInfo> pairwise_matches;
	BestOf2NearestRangeMatcher matcher(2, try_cuda, match_conf);
	matcher(features, pairwise_matches);
	matcher.collectGarbage();

	Mat imgMatches;
	cv::drawMatches(resize_img_temp[0], features[0].keypoints, resize_img_temp[1], features[1].keypoints, pairwise_matches[1].matches, imgMatches);
	cv::imshow("matches12", imgMatches);
	cv::waitKey(1);
	cv::drawMatches(resize_img_temp[1], features[1].keypoints, resize_img_temp[2], features[2].keypoints, pairwise_matches[6].matches, imgMatches);
	cv::imshow("matches23", imgMatches);
	cv::waitKey(1);
	cv::drawMatches(resize_img_temp[2], features[2].keypoints, resize_img_temp[3], features[3].keypoints, pairwise_matches[11].matches, imgMatches);
	cv::imshow("matches34", imgMatches);
	cv::waitKey(1);

	//单应性估计RANSAC LM迭代 估计图像的2D射影变换，true表示焦距已知
	HomographyBasedEstimator estimator(false);
	cameras.clear();
	if (!estimator(features, pairwise_matches, cameras)){
		MessageBox ( "Homography estimation failed.");
		return false;
	}
	for (size_t i = 0; i < cameras.size(); ++i) {
		cameras[i].R.convertTo(cameras[i].R, CV_32F);
	}
	
	//调整多个相机参数 捆集调整
	Ptr<detail::BundleAdjusterBase> adjuster;
	if (ba_cost_func == "reproj") adjuster = makePtr<detail::BundleAdjusterReproj>();
	else if (ba_cost_func == "ray") adjuster = makePtr<detail::BundleAdjusterRay>();
	else {
		MessageBox ( "Unknown bundle adjustment cost function:"+ QString::fromStdString( ba_cost_func) );
		return false;
	}
	adjuster->setConfThresh(conf_thresh);
	Mat_<uchar> refine_mask = Mat::zeros(3, 3, CV_8U);
	refine_mask(0, 0) = 1;
	refine_mask(0, 1) = 1;
	refine_mask(0, 2) = 1;
	refine_mask(1, 1) = 1;
	refine_mask(1, 2) = 1;
	adjuster->setRefinementMask(refine_mask);
	if (!(*adjuster)(features, pairwise_matches, cameras)) {
		MessageBox("Camera parameters adjusting failed.");
		return false;
	}

	//找到焦距中值
	vector<double> focals;
	for (size_t i = 0; i < cameras.size(); ++i) {
		focals.push_back(cameras[i].focal);
	}
	sort(focals.begin(), focals.end());
	float mean_focal;
	if (focals.size() % 2 == 1)
		mean_focal = static_cast<float>(focals[focals.size() / 2]);
	else
		mean_focal = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;

	//波浪调整
	vector<Mat> temp;
	for (size_t i = 0; i < cameras.size(); ++i)
		temp.push_back(cameras[i].R);
	waveCorrect(temp, wave_correct);

    //修理图片
	Ptr<WarperCreator> warper_creator;
#ifdef HAVE_OPENCV_CUDAWARPING
	if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
	{
		if (warp_type == "plane")
			warper_creator = makePtr<cv::PlaneWarperGpu>();
		else if (warp_type == "cylindrical")
			warper_creator = makePtr<cv::CylindricalWarperGpu>();
		else if (warp_type == "spherical")
			warper_creator = makePtr<cv::SphericalWarperGpu>();
	}
	else
#endif 
	{
		if (warp_type == "plane")
			warper_creator = makePtr<cv::PlaneWarper>();
		else if (warp_type == "cylindrical")
			warper_creator = makePtr<cv::CylindricalWarper>();
		else if (warp_type == "spherical")
			warper_creator = makePtr<cv::SphericalWarper>();
	}
	if (!warper_creator) {
		MessageBox( "Can't create the following warper " +QString::fromStdString(warp_type));
		return false;
	}
	vector<UMat> masks(num_images);
	vector<UMat> images_warped(num_images);
	vector<UMat> masks_warped(num_images);
	vector<UMat> images_warped_f(num_images);
	vector<cv::Point> corner_temp(num_images);
	for (int i = 0; i < num_images; ++i)
	{
		masks[i].create(resize_img_temp[i].size(), CV_8U);
		masks[i].setTo(Scalar::all(255));
	}
    #pragma omp parallel for
	for (int i = 0; i < num_images; ++i)
	{
		Mat_<float> K;
		warper = warper_creator->create(static_cast<float>(mean_focal * seam_scale / feature_scale));
		cameras[i].K().convertTo(K, CV_32F);
		float swa = (float)seam_scale / feature_scale;
		K(0, 0) *= swa; K(0, 2) *= swa;
		K(1, 1) *= swa; K(1, 2) *= swa;

		corner_temp[i] = warper->warp(resize_img_temp[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
		warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
		images_warped[i].convertTo(images_warped_f[i], CV_32F);
	}

	//曝光补偿 
	compensator = ExposureCompensator::createDefault(expos_comp_type);
	compensator->feed(corner_temp, images_warped, masks_warped);

	//图片重合部分修理
	Ptr<SeamFinder> seam_finder;
	if (seam_find_type == "no")
		seam_finder = makePtr<detail::NoSeamFinder>();
	else if (seam_find_type == "gc_color")
	{
#ifdef HAVE_OPENCV_CUDALEGACY
		if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
			seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR);
		else
#endif
			seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR);
	}
	else if (seam_find_type == "gc_colorgrad")
	{
#ifdef HAVE_OPENCV_CUDALEGACY
		if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
			seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
		else
#endif
			seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
	}
	else if (seam_find_type == "dp_color")
		seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR);
	else if (seam_find_type == "dp_colorgrad")
		seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR_GRAD);
	if (!seam_finder) {
		MessageBox("Can't create the following seam finder '" + QString::fromStdString (seam_find_type));
		return false;
	}
	seam_finder->find(images_warped_f, corner_temp, masks_warped);
	images_warped.clear();
	images_warped_f.clear();
	masks.clear();

	//融合
	corners.clear();
	sizes.clear();
	mask_warped.clear();
	blender.release();
	Mat img_warped, img_warped_s;
	Mat dilated_mask, seam_mask, mask;
	warper = warper_creator->create(mean_focal * compose_scale / feature_scale);
	for (int i = 0; i < num_images; ++i) {
		cameras[i].focal *= compose_scale / feature_scale;
		cameras[i].ppx *= compose_scale / feature_scale;
		cameras[i].ppy *= compose_scale / feature_scale;

		corners.push_back(cv::Point(0, 0));
		sizes.push_back(cv::Size());
		mask_warped.push_back(cv::Mat());

		Size sz = full_img_temp[i].size();
		sz.width = cvRound(sz.width * compose_scale);
		sz.height = cvRound(sz.height * compose_scale);

		Mat K;
		cameras[i].K().convertTo(K, CV_32F);
		Rect roi = warper->warpRoi(sz, K, cameras[i].R);
		corners[i] = roi.tl();
		sizes[i] = roi.size();
	}
	for (int img_idx = 0; img_idx < num_images; ++img_idx)
	{
		cv::resize(full_img_temp[img_idx], resize_img_temp[img_idx], Size(), compose_scale, compose_scale);
		Size img_size = resize_img_temp[img_idx].size();

		Mat K;
		cameras[img_idx].K().convertTo(K, CV_32F);

		warper->warp(resize_img_temp[img_idx], K, cameras[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);

		mask.create(img_size, CV_8U);
		mask.setTo(Scalar::all(255));
		warper->warp(mask, K, cameras[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped[img_idx]);

		compensator->apply(img_idx, corners[img_idx], img_warped, mask_warped[img_idx]);

		img_warped.convertTo(img_warped_s, CV_16S);

		img_warped.release();
		resize_img_temp[img_idx].release();
		mask.release();

		cv::dilate(masks_warped[img_idx], dilated_mask, Mat());
		cv::resize(dilated_mask, seam_mask, mask_warped[img_idx].size());
		mask_warped[img_idx] = seam_mask & mask_warped[img_idx];

		if (!blender)
		{
			blender = Blender::createDefault(Blender::MULTI_BAND, try_cuda);
			Size dst_sz = resultRoi(corners, sizes).size();
			float blend_width = sqrt(static_cast<float>(dst_sz.area())) * blend_strength / 100.f;
			if (blend_width < 1.f)
				blender = Blender::createDefault(Blender::NO, try_cuda);
			else {
				MultiBandBlender* mb = dynamic_cast<MultiBandBlender*>(blender.get());
				mb->setNumBands(static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.));
			}
			blender->prepare(corners, sizes);//确定结果图像大小
		}

		blender->feed(img_warped_s, mask_warped[img_idx], corners[img_idx]);
	}


	Mat result, result_mask;
	blender->blend(result, result_mask);
	cv::convertScaleAbs(result,img_final_stitched);
	img_final_stitched = cv::Mat(img_final_stitched, cv::Rect(cutZ, cutW, img_final_stitched.cols - 2 * cutZ, img_final_stitched.rows - cutW * 2));
	//imwrite("reult.jpg", result);
	imshow("result", img_final_stitched);
	MessageBox("Stitching finished");
	
	return true;
}


bool StitchTool::stitchVideo()
{
	cv::VideoWriter outputVideo; 
	int flag = 0;
	for (int i = 0; i < video.size(); i++)
		video[i].set(CAP_PROP_POS_FRAMES, frame_delay[i]);
	int ex = static_cast<int>(video[0].get(CAP_PROP_FOURCC));
	if (!outputVideo.open("output.wmv", CV_FOURCC('W', 'M', 'V', '2'), (int)video[0].get(CAP_PROP_FPS),
		Size(img_final_stitched.cols, img_final_stitched.rows), true))
	{
		MessageBox("video initialed failed");
		return false;
	}
		

	for (; ; )
	{
		blender->prepare(corners, sizes);
        #pragma omp parallel for
		for (int img_idx = 0; img_idx < num_images; ++img_idx)
		{
			cv::Mat src;
			Mat img_warped, img_warped_s;
			
			video[img_idx] >> src;

			if (src.empty())
			{
				flag = 1;
				break;
			}
			remap(src, src, map1[img_idx], map2[img_idx], INTER_LINEAR);

			src = cv::Mat(src, cv::Rect(cutX[img_idx], cutY[img_idx], src.cols - 2 * cutX[img_idx], src.rows - 2 * cutY[img_idx]));

			cv::resize(src, src, Size(), compose_scale, compose_scale);
			
			Mat K;
			cameras[img_idx].K().convertTo(K, CV_32F);

			warper->warp(src, K, cameras[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);

			img_warped.convertTo(img_warped_s, CV_16S);

			blender->feed(img_warped_s, mask_warped[img_idx], corners[img_idx]);
		}

		if (flag == 1)
			break;

		Mat result, result_mask, resultVideo;
		blender->blend(result, result_mask);

		cv::convertScaleAbs(result, img_final_stitched);
		img_final_stitched = cv::Mat(img_final_stitched, cv::Rect(cutZ, cutW, img_final_stitched.cols-2*cutZ, img_final_stitched.rows - cutW*2));
		cv::imshow("result", img_final_stitched);
		cv::waitKey(1);

		if (outputVideo.isOpened())
			outputVideo.write(img_final_stitched);
	}
	MessageBox("Stitch finished");
	return true;
}


bool StitchTool::sychronizeVideo(QStringList filename_list)
{
	if (filename_list.size() != 4)return false;
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

		std::string filename = filename_list[index].toStdString();
		int ret;
		AVCodec *dec;
		filter_graph = avfilter_graph_alloc();
		//------------------init codec-------------
		if ((ret = avformat_open_input(&fmt_ctx, filename.c_str(), NULL, NULL)) < 0) {
			std::cout << "Cannot open input file" << std::endl;
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
		snprintf(args, sizeof(args), "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=%d", time_base.num, time_base.den, dec_ctx->sample_rate, av_get_sample_fmt_name(dec_ctx->sample_fmt), dec_ctx->channel_layout);
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
			&inputs, &outputs, NULL)) < 0)
			return false;
		if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
			return false;
		//---------------------------------------
		while (1) {
			if ((ret = av_read_frame(fmt_ctx, packet)) < 0) {
				std::cout << "Frame read failed" << std::endl;
				return ret;
			}
			if (result[index].size() > frame->sample_rate * SAMPLE_TIEM / SAMPLE_RATE*FILTER_RATE) break;
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

	std::vector<int>::iterator biggest[4];
	int dist_max[3];
	for (int i = 0; i < 3; i++) {
		std::vector<int> dist_tmp;
		for (int j = 0; j < 400; j++) {//只分析400帧
			dist_tmp.push_back(0);
			for (int k = 0; k < FILTER_RATE*ANALISE_TIME; k++) {//比较数据的长度
				int tmp = (result[i + 1][j*(FILTER_RATE / FPS) + k] * result[0][200 * FILTER_RATE / FPS + k]);
				dist_tmp[j] += tmp / 100;
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
	return true;
}


StitchTool::StitchTool()
{

}


StitchTool::~StitchTool()
{
}
