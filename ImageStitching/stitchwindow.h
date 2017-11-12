#ifndef STITCHWINDOW_H
#define STITCHWINDOW_H

#include <QDialog>
#include "ui_stitchwindow.h"
#include "stitchtool.h"
#include "qfiledialog.h"
#include "qstringlistmodel.h"
#include <QTime>
#include <qtimer.h>

class StitchWindow : public QDialog
{
	Q_OBJECT

public:
	StitchWindow(QWidget *parent = 0);
	~StitchWindow();

	bool is_mouse_pressed;
	int x_distance;
	int y_distance;
	void mousePressEvent(QMouseEvent *event);
	void StitchWindow::mouseReleaseEvent(QMouseEvent *event);
	void StitchWindow::mouseMoveEvent(QMouseEvent *event);
	StitchTool stitch_tool;
	//QTimer video_timer;

private slots:
	void on_openImageButton_clicked();
	void on_openVideoButton_clicked();
	void on_outImageButton_clicked();
	void on_closeButton_clicked();
	void on_minimizeButton_clicked();
	void on_outVideoButton_clicked();
	//void updatePic();
private:
	Ui::StitchWindow ui;
    QStringListModel model;

};

#endif // STITCHWINDOW_H
