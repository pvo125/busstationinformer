/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frame_12;
    QLabel *string4_routname;
    QFrame *frame_7;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_23;
    QLabel *string3_timeleft;
    QSpacerItem *horizontalSpacer_24;
    QFrame *frame_15;
    QGridLayout *gridLayout_8;
    QSpacerItem *horizontalSpacer_15;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_16;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_21;
    QLabel *string2_lefttime;
    QSpacerItem *horizontalSpacer_22;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_19;
    QLabel *string1_lefttime;
    QSpacerItem *horizontalSpacer_20;
    QFrame *frame_11;
    QLabel *string3_routname;
    QFrame *frame_8;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_25;
    QLabel *string4_timeleft;
    QSpacerItem *horizontalSpacer_26;
    QFrame *frame_2;
    QGridLayout *gridLayout_4;
    QLabel *string2_routnumber;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_11;
    QFrame *frame_16;
    QGridLayout *gridLayout_9;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *horizontalSpacer_18;
    QFrame *frame_10;
    QLabel *string2_routnumber_2;
    QFrame *frame_13;
    QGridLayout *gridLayout_2;
    QLabel *labelDate;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QLabel *labelTime;
    QSpacerItem *horizontalSpacer_4;
    QFrame *frame_9;
    QLabel *string1_routname;
    QFrame *frame_3;
    QGridLayout *gridLayout_5;
    QLabel *string3_routnumber;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_10;
    QFrame *frame_4;
    QGridLayout *gridLayout_6;
    QSpacerItem *horizontalSpacer_8;
    QLabel *string4_routnumber;
    QSpacerItem *horizontalSpacer_9;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QLabel *string1_routnumber;
    QSpacerItem *horizontalSpacer_12;
    QFrame *frame_14;
    QGridLayout *gridLayout_7;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *horizontalSpacer_14;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(814, 531);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame_12 = new QFrame(centralwidget);
        frame_12->setObjectName(QString::fromUtf8("frame_12"));
        frame_12->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        string4_routname = new QLabel(frame_12);
        string4_routname->setObjectName(QString::fromUtf8("string4_routname"));
        string4_routname->setGeometry(QRect(10, 10, 350, 60));

        gridLayout->addWidget(frame_12, 4, 1, 1, 1);

        frame_7 = new QFrame(centralwidget);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setMaximumSize(QSize(220, 16777215));
        frame_7->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_7);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_23);

        string3_timeleft = new QLabel(frame_7);
        string3_timeleft->setObjectName(QString::fromUtf8("string3_timeleft"));
        string3_timeleft->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_3->addWidget(string3_timeleft);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_24);


        gridLayout->addWidget(frame_7, 3, 2, 1, 1);

        frame_15 = new QFrame(centralwidget);
        frame_15->setObjectName(QString::fromUtf8("frame_15"));
        frame_15->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        frame_15->setFrameShape(QFrame::StyledPanel);
        frame_15->setFrameShadow(QFrame::Raised);
        gridLayout_8 = new QGridLayout(frame_15);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_15, 0, 0, 1, 1);

        label_9 = new QLabel(frame_15);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_8->addWidget(label_9, 0, 1, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_16, 0, 2, 1, 1);


        gridLayout->addWidget(frame_15, 0, 1, 1, 1);

        frame_6 = new QFrame(centralwidget);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setMaximumSize(QSize(220, 16777215));
        frame_6->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_21);

        string2_lefttime = new QLabel(frame_6);
        string2_lefttime->setObjectName(QString::fromUtf8("string2_lefttime"));
        string2_lefttime->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(string2_lefttime);

        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_22);


        gridLayout->addWidget(frame_6, 2, 2, 1, 1);

        frame_5 = new QFrame(centralwidget);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setMaximumSize(QSize(220, 16777215));
        frame_5->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_19);

        string1_lefttime = new QLabel(frame_5);
        string1_lefttime->setObjectName(QString::fromUtf8("string1_lefttime"));
        string1_lefttime->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(string1_lefttime);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_20);


        gridLayout->addWidget(frame_5, 1, 2, 1, 1);

        frame_11 = new QFrame(centralwidget);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        string3_routname = new QLabel(frame_11);
        string3_routname->setObjectName(QString::fromUtf8("string3_routname"));
        string3_routname->setGeometry(QRect(10, 10, 350, 60));

        gridLayout->addWidget(frame_11, 3, 1, 1, 1);

        frame_8 = new QFrame(centralwidget);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setMaximumSize(QSize(220, 16777215));
        frame_8->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_8);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_25);

        string4_timeleft = new QLabel(frame_8);
        string4_timeleft->setObjectName(QString::fromUtf8("string4_timeleft"));
        string4_timeleft->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(string4_timeleft);

        horizontalSpacer_26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_26);


        gridLayout->addWidget(frame_8, 4, 2, 1, 1);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMaximumSize(QSize(200, 16777215));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        string2_routnumber = new QLabel(frame_2);
        string2_routnumber->setObjectName(QString::fromUtf8("string2_routnumber"));

        gridLayout_4->addWidget(string2_routnumber, 0, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_6, 0, 0, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_11, 0, 2, 1, 1);


        gridLayout->addWidget(frame_2, 2, 0, 1, 1);

        frame_16 = new QFrame(centralwidget);
        frame_16->setObjectName(QString::fromUtf8("frame_16"));
        frame_16->setMaximumSize(QSize(220, 16777215));
        frame_16->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        frame_16->setFrameShape(QFrame::StyledPanel);
        frame_16->setFrameShadow(QFrame::Raised);
        gridLayout_9 = new QGridLayout(frame_16);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_10 = new QLabel(frame_16);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_9->addWidget(label_10, 0, 1, 1, 1);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_17, 0, 0, 1, 1);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_18, 0, 2, 1, 1);


        gridLayout->addWidget(frame_16, 0, 2, 1, 1);

        frame_10 = new QFrame(centralwidget);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        string2_routnumber_2 = new QLabel(frame_10);
        string2_routnumber_2->setObjectName(QString::fromUtf8("string2_routnumber_2"));
        string2_routnumber_2->setGeometry(QRect(10, 10, 350, 60));

        gridLayout->addWidget(frame_10, 2, 1, 1, 1);

        frame_13 = new QFrame(centralwidget);
        frame_13->setObjectName(QString::fromUtf8("frame_13"));
        frame_13->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 109);"));
        frame_13->setFrameShape(QFrame::StyledPanel);
        frame_13->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_13);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelDate = new QLabel(frame_13);
        labelDate->setObjectName(QString::fromUtf8("labelDate"));
        QPalette palette;
        QBrush brush(QColor(170, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(79, 79, 109, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        labelDate->setPalette(palette);
        QFont font;
        font.setPointSize(28);
        labelDate->setFont(font);

        gridLayout_2->addWidget(labelDate, 0, 1, 1, 1);

        label_7 = new QLabel(frame_13);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 5, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 4, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 6, 1, 1);

        labelTime = new QLabel(frame_13);
        labelTime->setObjectName(QString::fromUtf8("labelTime"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        labelTime->setPalette(palette1);
        labelTime->setFont(font);

        gridLayout_2->addWidget(labelTime, 0, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 0, 1, 1);


        gridLayout->addWidget(frame_13, 5, 0, 1, 3);

        frame_9 = new QFrame(centralwidget);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        string1_routname = new QLabel(frame_9);
        string1_routname->setObjectName(QString::fromUtf8("string1_routname"));
        string1_routname->setGeometry(QRect(10, 10, 350, 60));
        string1_routname->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(frame_9, 1, 1, 1, 1);

        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setMaximumSize(QSize(200, 16777215));
        frame_3->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        string3_routnumber = new QLabel(frame_3);
        string3_routnumber->setObjectName(QString::fromUtf8("string3_routnumber"));

        gridLayout_5->addWidget(string3_routnumber, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_7, 0, 0, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_10, 0, 2, 1, 1);


        gridLayout->addWidget(frame_3, 3, 0, 1, 1);

        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMaximumSize(QSize(200, 16777215));
        frame_4->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(frame_4);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_8, 0, 0, 1, 1);

        string4_routnumber = new QLabel(frame_4);
        string4_routnumber->setObjectName(QString::fromUtf8("string4_routnumber"));

        gridLayout_6->addWidget(string4_routnumber, 0, 1, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_9, 0, 2, 1, 1);


        gridLayout->addWidget(frame_4, 4, 0, 1, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMaximumSize(QSize(200, 16777215));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(83, 118, 129);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 0, 0, 1, 1);

        string1_routnumber = new QLabel(frame);
        string1_routnumber->setObjectName(QString::fromUtf8("string1_routnumber"));
        string1_routnumber->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_3->addWidget(string1_routnumber, 0, 1, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_12, 0, 2, 1, 1);


        gridLayout->addWidget(frame, 1, 0, 1, 1);

        frame_14 = new QFrame(centralwidget);
        frame_14->setObjectName(QString::fromUtf8("frame_14"));
        frame_14->setMaximumSize(QSize(200, 16777215));
        frame_14->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        frame_14->setFrameShape(QFrame::StyledPanel);
        frame_14->setFrameShadow(QFrame::Raised);
        gridLayout_7 = new QGridLayout(frame_14);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_8 = new QLabel(frame_14);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_7->addWidget(label_8, 0, 1, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_13, 0, 0, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_14, 0, 2, 1, 1);


        gridLayout->addWidget(frame_14, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 814, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        string4_routname->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">99</span></p></body></html>", nullptr));
        string3_timeleft->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">5 \320\242</span></p></body></html>", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt;\">\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265</span></p></body></html>", nullptr));
        string2_lefttime->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">5 \320\242</span></p></body></html>", nullptr));
        string1_lefttime->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">5 \320\242</span></p></body></html>", nullptr));
        string3_routname->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">87</span></p></body></html>", nullptr));
        string4_timeleft->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">5 \320\242</span></p></body></html>", nullptr));
        string2_routnumber->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">37</span></p></body></html>", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt;\">\320\236\320\266\320\270\320\264\320\260\320\275\320\270\320\265</span></p></body></html>", nullptr));
        string2_routnumber_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">37</span></p></body></html>", nullptr));
        labelDate->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#aaffff;\">23-10-2023</span></p></body></html>", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt; color:#aaffff;\">-12 \320\241</span></p></body></html>", nullptr));
        labelTime->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt; color:#aaffff;\">12:54</span></p></body></html>", nullptr));
        string1_routname->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">5 \320\242</span></p></body></html>", nullptr));
        string3_routnumber->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">87</span></p></body></html>", nullptr));
        string4_routnumber->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">99</span></p></body></html>", nullptr));
        string1_routnumber->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffff7f;\">5 \320\242</span></p></body></html>", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt;\">\320\234\320\260\321\200\321\210\321\200\321\203\321\202</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
