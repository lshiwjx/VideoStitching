#include "stitchwindow.h"
#include <QtWidgets/QApplication>
#include "qstring.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StitchWindow stitchWindow;
	stitchWindow.show();   

	return a.exec();
}