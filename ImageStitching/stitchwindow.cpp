#include "stitchwindow.h"

StitchWindow::StitchWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//去掉了border
	setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	//去掉了背景
	setAttribute(Qt::WA_TranslucentBackground);
	//connect(&video_timer, SIGNAL(timeout()), this, SLOT(updatePic()));
}


StitchWindow::~StitchWindow()
{
}

//ui移动事件
void StitchWindow::mousePressEvent(QMouseEvent *event)
{
	is_mouse_pressed = true;
	x_distance = QCursor::pos().x() - this->pos().x();
	y_distance = QCursor::pos().y() - this->pos().y();
}


void StitchWindow::mouseReleaseEvent(QMouseEvent *event)
{
	is_mouse_pressed = false;
}


void StitchWindow::mouseMoveEvent(QMouseEvent *event)
{
	if (is_mouse_pressed) {
		QPoint pos = QPoint((QCursor::pos().x() - x_distance), (QCursor::pos().y() - y_distance));
		this->setProperty("pos", pos);
	}
}

//读取图像
void StitchWindow::on_openImageButton_clicked()
{
	QStringList filename_list;
	QFileDialog dialog(this);
	dialog.setDirectory(QDir::homePath());
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
	if (dialog.exec())
		filename_list = dialog.selectedFiles();

	if (stitch_tool.readImage(filename_list)) {
		model.setStringList(filename_list);
		this->ui.fileNameView->setModel(&model);
	}
	else {
		QMessageBox msgbox;
		msgbox.setText("Load image failed");
		msgbox.exec();
	}
}

//读取视频并同步
void StitchWindow::on_openVideoButton_clicked()
{
	QStringList filename_list;
	QFileDialog dialog(this);

	dialog.setDirectory(QDir::currentPath());
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(tr("Videos (*.mp4 *.wmv)"));
	if (dialog.exec())
		filename_list = dialog.selectedFiles();

	//视频同步
	stitch_tool.sychronizeVideo(filename_list);
	this->ui.frameDelay->setText(QString::number(stitch_tool.frame_delay[0]));
	this->ui.frameDelay_2->setText(QString::number(stitch_tool.frame_delay[1]));
	this->ui.frameDelay_3->setText(QString::number(stitch_tool.frame_delay[2]));
	this->ui.frameDelay_4->setText(QString::number(stitch_tool.frame_delay[3]));

	if (stitch_tool.readVideo(filename_list)) {
		model.setStringList(filename_list);
		this->ui.fileNameView->setModel(&model);
	}
	else {
		QMessageBox msgbox;
		msgbox.setText("Load video failed");
		msgbox.exec();
	}
}

//进行合成
void StitchWindow::on_outImageButton_clicked()
{
	QTime time;
	QImage img_stitched;
	QPixmap pixmap_stitch;
	Mat rgb;
	time.start();
	int start_time = time.elapsed();
	stitch_tool.features_type = this->ui.featureSelect->currentText().toStdString();
	stitch_tool.warp_type = this->ui.cameraSelect->currentText().toStdString();
	stitch_tool.seam_find_type = this->ui.seamSelect->currentText().toStdString();
	stitch_tool.ba_cost_func = this->ui.errorSelect->currentText().toStdString();
	stitch_tool.match_conf = this->ui.matchConf->text().toDouble();
	stitch_tool.cutZ = this->ui.cutx->text().toDouble();
	stitch_tool.cutW = this->ui.cuty->text().toDouble();
	stitch_tool.compose_scale = this->ui.composeScale->text().toDouble();
	stitch_tool.seam_scale = this->ui.seamScale->text().toDouble();
	stitch_tool.feature_scale = this->ui.featureScale->text().toDouble();
	stitch_tool.frame_delay[0] = this->ui.frameDelay->text().toDouble();
	stitch_tool.frame_delay[1] = this->ui.frameDelay_2->text().toDouble();
	stitch_tool.frame_delay[2] = this->ui.frameDelay_3->text().toDouble();
	stitch_tool.frame_delay[3] = this->ui.frameDelay_4->text().toDouble();
	//显示运行时间
	this->ui.timeLabel->clear();
	if(stitch_tool.stitchImage()) { 
		int time_diff = time.elapsed() - start_time;
		this->ui.timeLabel->setText(QString::number(time_diff));

		//在Lable上显示图片
		cv::cvtColor(stitch_tool.img_final_stitched, rgb, CV_BGR2RGB);
		img_stitched = QImage((const uchar*)(rgb.data),  //(const unsigned char*)  
			rgb.cols, rgb.rows,
			rgb.cols * rgb.channels(),   //new add  
			QImage::Format_RGB888);
		if(ui.result->width() > ui.result->height())
			pixmap_stitch = QPixmap::fromImage(img_stitched).scaledToWidth(ui.result->width());
		else
			pixmap_stitch = QPixmap::fromImage(img_stitched).scaledToHeight(ui.result->height());
		this->ui.result->setPixmap(pixmap_stitch);
		this->ui.result->show();
	}
	else {
		QMessageBox msgbox;
		msgbox.setText("Stitch failed");
		msgbox.exec();
	}
	
}

//合成视频
void StitchWindow::on_outVideoButton_clicked()
{
	QTime time;
	time.start();
	int start_time = time.elapsed();

	if (this->stitch_tool.stitchVideo()) {
		this->ui.timeLabel->clear();
		int time_diff = time.elapsed() - start_time;
		this->ui.timeLabel->setText(QString::number(time_diff));
	}
	else {
		QMessageBox msgbox;
		msgbox.setText("Stitch video failed");
		msgbox.exec();
	}
	
}

//关闭按钮
void StitchWindow::on_closeButton_clicked()
{
	exit(0);
}

//最小化按钮
void StitchWindow::on_minimizeButton_clicked()
{
	this->showMinimized();
}


/*void StitchWindow::updatePic()
{
	QImage img_stitched;
	QPixmap pixmap_stitch;
	Mat rgb;
	cvtColor(stitch_tool.img_final_stitched, rgb, CV_BGR2RGB);
	img_stitched = QImage((const uchar*)(rgb.data),  //(const unsigned char*)
		rgb.cols, rgb.rows,
		rgb.cols * rgb.channels(),   //new add  
		QImage::Format_RGB888);
	if (ui.result->width()>ui.result->height())
		pixmap_stitch = QPixmap::fromImage(img_stitched).scaledToWidth(ui.result->width());
	else
		pixmap_stitch = QPixmap::fromImage(img_stitched).scaledToHeight(ui.result->height());
	this->ui.result->setPixmap(pixmap_stitch);
	this->ui.result->show();
}*/