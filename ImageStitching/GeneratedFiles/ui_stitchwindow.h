/********************************************************************************
** Form generated from reading UI file 'stitchwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STITCHWINDOW_H
#define UI_STITCHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StitchWindow
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QPushButton *closeButton;
    QPushButton *minimizeButton;
    QListView *fileNameView;
    QLabel *label;
    QComboBox *featureSelect;
    QLabel *label_2;
    QLabel *label_3;
    QComboBox *cameraSelect;
    QLabel *label_4;
    QLabel *timeLabel;
    QComboBox *seamSelect;
    QLabel *label_5;
    QLabel *result;
    QLineEdit *matchConf;
    QLabel *label_6;
    QLineEdit *cuty;
    QLineEdit *cutx;
    QLabel *label_7;
    QLabel *label_8;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *openImageButton;
    QPushButton *openVideoButton;
    QPushButton *outImageButton;
    QPushButton *outVideoButton;
    QLabel *label_9;
    QLabel *label_10;
    QComboBox *errorSelect;
    QLabel *label_11;
    QLineEdit *featureScale;
    QLineEdit *composeScale;
    QLineEdit *seamScale;
    QLabel *label_12;
    QLabel *label_13;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QLineEdit *frameDelay_2;
    QLineEdit *frameDelay;
    QLineEdit *frameDelay_3;
    QLineEdit *frameDelay_4;

    void setupUi(QDialog *StitchWindow)
    {
        if (StitchWindow->objectName().isEmpty())
            StitchWindow->setObjectName(QStringLiteral("StitchWindow"));
        StitchWindow->resize(915, 783);
        StitchWindow->setStyleSheet(QLatin1String("#frame{\n"
"/*background-color: rgb(125, 142, 255);*/\n"
"border-image:url(./background.jpg);\n"
"border-radius:30px;\n"
"padding:20px;\n"
"}\n"
"QPushButton{\n"
"	font: 75 italic 9pt \"Arial\";\n"
"    border:0px;\n"
"    background:rgba(255,255,255,0);\n"
"	color: rgb(228, 243, 255);\n"
"}\n"
"QPushButton:hover                                               \n"
"{\n"
"    background: rgba(30,144,255,0.7);                         \n"
"}\n"
"QLabel\n"
"{\n"
"    color: rgb(228, 243, 255);   \n"
"}\n"
"QListView\n"
"{\n"
"	background-color: rgba(163, 189, 229,0.7);\n"
"}"));
        gridLayout = new QGridLayout(StitchWindow);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        frame = new QFrame(StitchWindow);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        closeButton = new QPushButton(frame);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setGeometry(QRect(810, 0, 81, 71));
        minimizeButton = new QPushButton(frame);
        minimizeButton->setObjectName(QStringLiteral("minimizeButton"));
        minimizeButton->setGeometry(QRect(730, 0, 81, 71));
        fileNameView = new QListView(frame);
        fileNameView->setObjectName(QStringLiteral("fileNameView"));
        fileNameView->setGeometry(QRect(50, 80, 721, 91));
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 50, 81, 21));
        featureSelect = new QComboBox(frame);
        featureSelect->setObjectName(QStringLiteral("featureSelect"));
        featureSelect->setGeometry(QRect(50, 210, 99, 24));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 180, 131, 18));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(240, 180, 91, 18));
        cameraSelect = new QComboBox(frame);
        cameraSelect->setObjectName(QStringLiteral("cameraSelect"));
        cameraSelect->setGeometry(QRect(240, 210, 131, 24));
        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(760, 210, 111, 20));
        timeLabel = new QLabel(frame);
        timeLabel->setObjectName(QStringLiteral("timeLabel"));
        timeLabel->setGeometry(QRect(760, 240, 111, 21));
        timeLabel->setStyleSheet(QStringLiteral("background-color: rgba(163, 189, 229,0.7);"));
        seamSelect = new QComboBox(frame);
        seamSelect->setObjectName(QStringLiteral("seamSelect"));
        seamSelect->setGeometry(QRect(420, 210, 121, 24));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(430, 180, 121, 18));
        result = new QLabel(frame);
        result->setObjectName(QStringLiteral("result"));
        result->setGeometry(QRect(40, 390, 801, 341));
        matchConf = new QLineEdit(frame);
        matchConf->setObjectName(QStringLiteral("matchConf"));
        matchConf->setGeometry(QRect(430, 280, 113, 21));
        label_6 = new QLabel(frame);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(430, 250, 131, 18));
        cuty = new QLineEdit(frame);
        cuty->setObjectName(QStringLiteral("cuty"));
        cuty->setGeometry(QRect(50, 270, 113, 25));
        cutx = new QLineEdit(frame);
        cutx->setObjectName(QStringLiteral("cutx"));
        cutx->setGeometry(QRect(240, 270, 113, 25));
        label_7 = new QLabel(frame);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(50, 250, 81, 18));
        label_8 = new QLabel(frame);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(240, 250, 81, 18));
        verticalLayoutWidget = new QWidget(frame);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(770, 80, 112, 117));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        openImageButton = new QPushButton(verticalLayoutWidget);
        openImageButton->setObjectName(QStringLiteral("openImageButton"));

        verticalLayout->addWidget(openImageButton);

        openVideoButton = new QPushButton(verticalLayoutWidget);
        openVideoButton->setObjectName(QStringLiteral("openVideoButton"));

        verticalLayout->addWidget(openVideoButton);

        outImageButton = new QPushButton(verticalLayoutWidget);
        outImageButton->setObjectName(QStringLiteral("outImageButton"));

        verticalLayout->addWidget(outImageButton);

        outVideoButton = new QPushButton(verticalLayoutWidget);
        outVideoButton->setObjectName(QStringLiteral("outVideoButton"));

        verticalLayout->addWidget(outVideoButton);

        label_9 = new QLabel(frame);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(600, 180, 121, 18));
        label_10 = new QLabel(frame);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(600, 250, 131, 18));
        errorSelect = new QComboBox(frame);
        errorSelect->setObjectName(QStringLiteral("errorSelect"));
        errorSelect->setGeometry(QRect(600, 210, 99, 24));
        label_11 = new QLabel(frame);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(430, 310, 121, 18));
        featureScale = new QLineEdit(frame);
        featureScale->setObjectName(QStringLiteral("featureScale"));
        featureScale->setGeometry(QRect(430, 330, 113, 25));
        composeScale = new QLineEdit(frame);
        composeScale->setObjectName(QStringLiteral("composeScale"));
        composeScale->setGeometry(QRect(50, 330, 113, 25));
        seamScale = new QLineEdit(frame);
        seamScale->setObjectName(QStringLiteral("seamScale"));
        seamScale->setGeometry(QRect(240, 330, 113, 25));
        label_12 = new QLabel(frame);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(50, 310, 121, 18));
        label_13 = new QLabel(frame);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(240, 310, 101, 18));
        gridLayoutWidget = new QWidget(frame);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(600, 280, 160, 91));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        frameDelay_2 = new QLineEdit(gridLayoutWidget);
        frameDelay_2->setObjectName(QStringLiteral("frameDelay_2"));

        gridLayout_2->addWidget(frameDelay_2, 0, 2, 1, 1);

        frameDelay = new QLineEdit(gridLayoutWidget);
        frameDelay->setObjectName(QStringLiteral("frameDelay"));

        gridLayout_2->addWidget(frameDelay, 0, 1, 1, 1);

        frameDelay_3 = new QLineEdit(gridLayoutWidget);
        frameDelay_3->setObjectName(QStringLiteral("frameDelay_3"));

        gridLayout_2->addWidget(frameDelay_3, 1, 1, 1, 1);

        frameDelay_4 = new QLineEdit(gridLayoutWidget);
        frameDelay_4->setObjectName(QStringLiteral("frameDelay_4"));

        gridLayout_2->addWidget(frameDelay_4, 1, 2, 1, 1);


        gridLayout->addWidget(frame, 0, 1, 1, 1);


        retranslateUi(StitchWindow);

        QMetaObject::connectSlotsByName(StitchWindow);
    } // setupUi

    void retranslateUi(QDialog *StitchWindow)
    {
        StitchWindow->setWindowTitle(QApplication::translate("StitchWindow", "StitchWindow", 0));
        closeButton->setText(QApplication::translate("StitchWindow", "X", 0));
        minimizeButton->setText(QApplication::translate("StitchWindow", "\342\200\224", 0));
        label->setText(QApplication::translate("StitchWindow", "\346\226\207\344\273\266\345\210\227\350\241\250\357\274\232", 0));
        featureSelect->clear();
        featureSelect->insertItems(0, QStringList()
         << QApplication::translate("StitchWindow", "surf", 0)
         << QApplication::translate("StitchWindow", "orb", 0)
        );
        label_2->setText(QApplication::translate("StitchWindow", "\347\211\271\345\276\201\346\217\220\345\217\226\347\256\227\346\263\225\357\274\232", 0));
        label_3->setText(QApplication::translate("StitchWindow", "warper\347\256\227\346\263\225\357\274\232", 0));
        cameraSelect->clear();
        cameraSelect->insertItems(0, QStringList()
         << QApplication::translate("StitchWindow", "spherical", 0)
         << QApplication::translate("StitchWindow", "cylindrical", 0)
        );
        label_4->setText(QApplication::translate("StitchWindow", "\345\220\210\346\210\220\346\227\266\351\227\264(ms)\357\274\232", 0));
        timeLabel->setText(QString());
        seamSelect->clear();
        seamSelect->insertItems(0, QStringList()
         << QApplication::translate("StitchWindow", "dp_colorgrad", 0)
         << QApplication::translate("StitchWindow", "gc_colorgrad", 0)
         << QApplication::translate("StitchWindow", "dp_color", 0)
         << QApplication::translate("StitchWindow", "gc_color", 0)
        );
        label_5->setText(QApplication::translate("StitchWindow", "\347\274\235\351\232\231\345\241\253\350\241\245\347\256\227\346\263\225\357\274\232", 0));
        result->setText(QString());
        matchConf->setText(QApplication::translate("StitchWindow", "0.43", 0));
        label_6->setText(QApplication::translate("StitchWindow", "\347\211\271\345\276\201\345\214\271\351\205\215\351\230\210\345\200\274\357\274\232", 0));
        cuty->setText(QApplication::translate("StitchWindow", "30", 0));
        cutx->setText(QApplication::translate("StitchWindow", "0", 0));
        label_7->setText(QApplication::translate("StitchWindow", "\347\272\265\345\220\221\350\243\201\345\211\252\357\274\232", 0));
        label_8->setText(QApplication::translate("StitchWindow", "\346\250\252\345\220\221\350\243\201\345\211\252\357\274\232", 0));
        openImageButton->setText(QApplication::translate("StitchWindow", "\344\273\216\345\233\276\345\203\217\344\270\255\350\216\267\345\217\226", 0));
        openVideoButton->setText(QApplication::translate("StitchWindow", "\344\273\216\350\247\206\351\242\221\344\270\255\350\216\267\345\217\226", 0));
        outImageButton->setText(QApplication::translate("StitchWindow", "\345\233\276\345\203\217\346\213\274\346\216\245", 0));
        outVideoButton->setText(QApplication::translate("StitchWindow", "\345\220\210\346\210\220\350\247\206\351\242\221", 0));
        label_9->setText(QApplication::translate("StitchWindow", "\346\215\206\351\233\206\350\257\257\345\267\256\347\261\273\345\236\213\357\274\232", 0));
        label_10->setText(QApplication::translate("StitchWindow", "\345\273\266\350\277\237\345\270\247\346\225\260:", 0));
        errorSelect->clear();
        errorSelect->insertItems(0, QStringList()
         << QApplication::translate("StitchWindow", "ray", 0)
         << QApplication::translate("StitchWindow", "reproj", 0)
        );
        label_11->setText(QApplication::translate("StitchWindow", "featureScale:", 0));
        featureScale->setText(QApplication::translate("StitchWindow", "1", 0));
        composeScale->setText(QApplication::translate("StitchWindow", "0.7", 0));
        seamScale->setText(QApplication::translate("StitchWindow", "1", 0));
        label_12->setText(QApplication::translate("StitchWindow", "ComposeScale:", 0));
        label_13->setText(QApplication::translate("StitchWindow", "SeamScale:", 0));
        frameDelay_2->setText(QApplication::translate("StitchWindow", "0", 0));
        frameDelay->setText(QApplication::translate("StitchWindow", "0", 0));
        frameDelay_3->setText(QApplication::translate("StitchWindow", "0", 0));
        frameDelay_4->setText(QApplication::translate("StitchWindow", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class StitchWindow: public Ui_StitchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STITCHWINDOW_H
