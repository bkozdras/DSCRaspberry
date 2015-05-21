/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QDockWidget *loggerDockWidget;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_3;
    QTableWidget *loggerTable;
    QTableWidget *footerDataTable;
    QDockWidget *faultsDockWidget;
    QWidget *dockWidgetContents_3;
    QGridLayout *gridLayout_4;
    QTableWidget *faultsTable;
    QDockWidget *applicationDockWidget;
    QWidget *dockWidgetContents_4;
    QGridLayout *gridLayout_5;
    QTabWidget *applicationTabWidget;
    QWidget *tabAutodetection;
    QGridLayout *gridLayout;
    QFrame *frame;
    QFrame *frame_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *labelStatusAds1248Info1;
    QLabel *labelStatusAds1248Info2;
    QLabel *labelStatusAds1248;
    QLabel *labelStatusDrv595Info1;
    QLabel *labelStatusDrv595;
    QLabel *labelStatusDrv595Info2;
    QLabel *labelStatusLmp90100CsInfo1;
    QLabel *labelStatusLmp90100Cs;
    QLabel *labelStatusLmp90100CsInfo2;
    QLabel *labelStatusMcp4716Info2;
    QLabel *label_3;
    QLabel *labelStatusStm32f401reInfo1;
    QLabel *labelStatusStm32f401reInfo2;
    QLabel *labelStatusLmp90100SmInfo1;
    QLabel *labelStatusLmp90100Sm;
    QLabel *label_5;
    QLabel *labelStatusMcp4716;
    QLabel *label_6;
    QLabel *labelStatusLmp90100SmInfo2;
    QLabel *labelStatusStm32f401re;
    QLabel *labelStatusMcp4716Info1;
    QFrame *frame_3;
    QLabel *labelStatusRefThermocouple;
    QLabel *label_8;
    QLabel *labelStatusRefThermocoupleInfo;
    QLabel *label_16;
    QLabel *labelStatusThermocouple1Info;
    QLabel *labelStatusThermocouple1;
    QLabel *label_17;
    QLabel *labelStatusThermocouple2Info;
    QLabel *labelStatusThermocouple2;
    QLabel *label_18;
    QLabel *labelStatusThermocouple3Info;
    QLabel *labelStatusThermocouple3;
    QLabel *label_19;
    QLabel *labelStatusThermocouple4Info;
    QLabel *labelStatusThermocouple4;
    QLabel *label_20;
    QLabel *labelStatusRtd2Pt100SmInfo;
    QLabel *labelStatusRtd2Pt100Sm;
    QLabel *labelStatusRtd1Pt100SmInfo;
    QLabel *label_31;
    QLabel *labelStatusRtd1Pt100Sm;
    QLabel *labelStatusRtdPt1000CsInfo;
    QLabel *label_32;
    QLabel *labelStatusRtdPt1000Cs;
    QLabel *labelStatusPeltierCellInfo;
    QLabel *labelStatusPeltierCell;
    QLabel *label_40;
    QWidget *tabUnitsDataViewer;
    QLabel *labelDevice;
    QLabel *labelUnitsDataSample1;
    QLabel *labelUnitsDataSample2;
    QLabel *labelUnitsDataRefSubst;
    QLabel *labelUnitsDataSample3;
    QLabel *labelUnitsDataSample4;
    QLabel *labelUnitsDataSampleCarrierTemperature;
    QLabel *labelUnitsDataHeaterTemperature;
    QLabel *labelUnitsDataHeaterPower;
    QLabel *labelUnitsDataSMPCBTemperature;
    QLabel *labelUnitsDataFanPower;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QFrame *line_5;
    QFrame *line_6;
    QFrame *line_7;
    QFrame *line_8;
    QFrame *line_9;
    QFrame *line_10;
    QFrame *line_11;
    QFrame *line_12;
    QFrame *line_13;
    QFrame *line_14;
    QFrame *line_15;
    QFrame *line_16;
    QFrame *line_17;
    QFrame *line_18;
    QFrame *line_19;
    QFrame *line_20;
    QFrame *line_21;
    QFrame *line_22;
    QFrame *line_23;
    QFrame *line_24;
    QWidget *tabHeaterControl;
    QCustomPlot *heaterTemperaturePlot;
    QTabWidget *tabWidgetHeaterPower;
    QWidget *tabHeaterPowerOpenLoop;
    QLabel *labelHeaterPowerControlValueInfo;
    QSlider *horizontalSliderHeaterPowerControlValue;
    QTextEdit *textEditHeaterPowerControlValue;
    QLabel *labelHeaterPowerOpenLoopPercent;
    QWidget *tabHeaterPowerSimpleFeedback;
    QWidget *tabHeaterPowerMFCFeedback;
    QFrame *frame_4;
    QLabel *labelHeaterPowerFilename;
    QTextEdit *textEdit;
    QCheckBox *checkBoxHeaterPowerSaveToFile;
    QLabel *labelHeaterPowerFileDataSampling;
    QComboBox *comboBoxHeaterPowerFileSps;
    QLabel *labelHeaterPowerControlMode_5;
    QLabel *labelHeaterPowerControlMode_3;
    QLabel *labelHeaterPowerControlMode_2;
    QLabel *labelHeaterPowerControlMode_4;
    QFrame *frame_5;
    QLabel *labelHeaterPowerPlotTemperature;
    QCheckBox *checkBoxHeaterPowerPlotTemperature;
    QComboBox *comboBoxHeaterPowerPlotTemperatureSps;
    QLabel *labelHeaterPowerControlMode;
    QComboBox *comboBox;
    QWidget *tabSegmentsManager;
    QGridLayout *gridLayout_2;
    QWidget *tabDscDataView;
    QDockWidget *dscDataDockWidget;
    QWidget *dockWidgetContents_5;
    QGridLayout *gridLayout_8;
    QLabel *labelHeaterPower;
    QLabel *labelHeaterPowerInfo;
    QLabel *labelSPHeaterTemperatureInfo;
    QLabel *labelSMPCBTemperature;
    QLabel *labelSPHeaterTemperature;
    QLabel *labelHeaterTemperature;
    QLabel *labelHeaterTemperatureInfo;
    QLabel *labelSampleCarrierTemperature;
    QLabel *labelSMPCBTemperatureInfo;
    QLabel *labelSampleCarrierTemperatureInfo;
    QLabel *labelControlModeInfo;
    QLabel *labelControlMode;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1920, 1080);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        MainWindow->setFont(font);
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        loggerDockWidget = new QDockWidget(centralWidget);
        loggerDockWidget->setObjectName(QString::fromUtf8("loggerDockWidget"));
        loggerDockWidget->setGeometry(QRect(0, 720, 1200, 281));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        loggerDockWidget->setFont(font1);
        loggerDockWidget->setContextMenuPolicy(Qt::NoContextMenu);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        gridLayout_3 = new QGridLayout(dockWidgetContents_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        loggerTable = new QTableWidget(dockWidgetContents_2);
        if (loggerTable->columnCount() < 4)
            loggerTable->setColumnCount(4);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font2);
        loggerTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font2);
        loggerTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font2);
        loggerTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font2);
        loggerTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        loggerTable->setObjectName(QString::fromUtf8("loggerTable"));
        QFont font3;
        font3.setPointSize(10);
        loggerTable->setFont(font3);
        loggerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        loggerTable->setRowCount(0);
        loggerTable->setColumnCount(4);
        loggerTable->verticalHeader()->setVisible(false);

        gridLayout_3->addWidget(loggerTable, 0, 0, 1, 1);

        loggerDockWidget->setWidget(dockWidgetContents_2);
        footerDataTable = new QTableWidget(centralWidget);
        if (footerDataTable->columnCount() < 10)
            footerDataTable->setColumnCount(10);
        footerDataTable->setObjectName(QString::fromUtf8("footerDataTable"));
        footerDataTable->setGeometry(QRect(10, 1000, 1901, 21));
        footerDataTable->setShowGrid(false);
        footerDataTable->setRowCount(0);
        footerDataTable->setColumnCount(10);
        footerDataTable->horizontalHeader()->setVisible(false);
        footerDataTable->verticalHeader()->setVisible(false);
        faultsDockWidget = new QDockWidget(centralWidget);
        faultsDockWidget->setObjectName(QString::fromUtf8("faultsDockWidget"));
        faultsDockWidget->setGeometry(QRect(1210, 720, 361, 281));
        faultsDockWidget->setFont(font1);
        faultsDockWidget->setContextMenuPolicy(Qt::NoContextMenu);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        gridLayout_4 = new QGridLayout(dockWidgetContents_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        faultsTable = new QTableWidget(dockWidgetContents_3);
        if (faultsTable->columnCount() < 4)
            faultsTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font2);
        faultsTable->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font2);
        faultsTable->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font2);
        faultsTable->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFont(font2);
        faultsTable->setHorizontalHeaderItem(3, __qtablewidgetitem7);
        faultsTable->setObjectName(QString::fromUtf8("faultsTable"));
        faultsTable->setFont(font3);
        faultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        faultsTable->setRowCount(0);
        faultsTable->setColumnCount(4);
        faultsTable->verticalHeader()->setVisible(false);

        gridLayout_4->addWidget(faultsTable, 0, 0, 1, 1);

        faultsDockWidget->setWidget(dockWidgetContents_3);
        applicationDockWidget = new QDockWidget(centralWidget);
        applicationDockWidget->setObjectName(QString::fromUtf8("applicationDockWidget"));
        applicationDockWidget->setGeometry(QRect(0, 0, 1920, 719));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(applicationDockWidget->sizePolicy().hasHeightForWidth());
        applicationDockWidget->setSizePolicy(sizePolicy);
        applicationDockWidget->setFont(font1);
        applicationDockWidget->setContextMenuPolicy(Qt::NoContextMenu);
        applicationDockWidget->setLayoutDirection(Qt::LeftToRight);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        gridLayout_5 = new QGridLayout(dockWidgetContents_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        applicationTabWidget = new QTabWidget(dockWidgetContents_4);
        applicationTabWidget->setObjectName(QString::fromUtf8("applicationTabWidget"));
        applicationTabWidget->setEnabled(true);
        QFont font4;
        font4.setPointSize(14);
        applicationTabWidget->setFont(font4);
        applicationTabWidget->setTabPosition(QTabWidget::North);
        applicationTabWidget->setTabsClosable(true);
        tabAutodetection = new QWidget();
        tabAutodetection->setObjectName(QString::fromUtf8("tabAutodetection"));
        gridLayout = new QGridLayout(tabAutodetection);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame = new QFrame(tabAutodetection);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(0, 100));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(3);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(9, 9, 930, 615));
        frame_2->setMinimumSize(QSize(0, 100));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Plain);
        frame_2->setLineWidth(3);
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 30, 210, 140));
        label->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/ADS1248.gif")));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 220, 210, 140));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/DRV593.gif")));
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 410, 210, 140));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/LMP90100.gif")));
        labelStatusAds1248Info1 = new QLabel(frame_2);
        labelStatusAds1248Info1->setObjectName(QString::fromUtf8("labelStatusAds1248Info1"));
        labelStatusAds1248Info1->setGeometry(QRect(240, 50, 200, 41));
        QFont font5;
        font5.setPointSize(18);
        font5.setBold(false);
        font5.setWeight(50);
        labelStatusAds1248Info1->setFont(font5);
        labelStatusAds1248Info2 = new QLabel(frame_2);
        labelStatusAds1248Info2->setObjectName(QString::fromUtf8("labelStatusAds1248Info2"));
        labelStatusAds1248Info2->setGeometry(QRect(240, 80, 111, 41));
        labelStatusAds1248Info2->setFont(font5);
        labelStatusAds1248 = new QLabel(frame_2);
        labelStatusAds1248->setObjectName(QString::fromUtf8("labelStatusAds1248"));
        labelStatusAds1248->setGeometry(QRect(240, 110, 221, 41));
        labelStatusAds1248->setFont(font5);
        labelStatusDrv595Info1 = new QLabel(frame_2);
        labelStatusDrv595Info1->setObjectName(QString::fromUtf8("labelStatusDrv595Info1"));
        labelStatusDrv595Info1->setGeometry(QRect(240, 240, 200, 41));
        labelStatusDrv595Info1->setFont(font5);
        labelStatusDrv595 = new QLabel(frame_2);
        labelStatusDrv595->setObjectName(QString::fromUtf8("labelStatusDrv595"));
        labelStatusDrv595->setGeometry(QRect(240, 300, 221, 41));
        labelStatusDrv595->setFont(font5);
        labelStatusDrv595Info2 = new QLabel(frame_2);
        labelStatusDrv595Info2->setObjectName(QString::fromUtf8("labelStatusDrv595Info2"));
        labelStatusDrv595Info2->setGeometry(QRect(240, 270, 111, 41));
        labelStatusDrv595Info2->setFont(font5);
        labelStatusLmp90100CsInfo1 = new QLabel(frame_2);
        labelStatusLmp90100CsInfo1->setObjectName(QString::fromUtf8("labelStatusLmp90100CsInfo1"));
        labelStatusLmp90100CsInfo1->setGeometry(QRect(240, 430, 200, 41));
        labelStatusLmp90100CsInfo1->setFont(font5);
        labelStatusLmp90100Cs = new QLabel(frame_2);
        labelStatusLmp90100Cs->setObjectName(QString::fromUtf8("labelStatusLmp90100Cs"));
        labelStatusLmp90100Cs->setGeometry(QRect(240, 490, 221, 41));
        labelStatusLmp90100Cs->setFont(font5);
        labelStatusLmp90100CsInfo2 = new QLabel(frame_2);
        labelStatusLmp90100CsInfo2->setObjectName(QString::fromUtf8("labelStatusLmp90100CsInfo2"));
        labelStatusLmp90100CsInfo2->setGeometry(QRect(240, 460, 111, 41));
        labelStatusLmp90100CsInfo2->setFont(font5);
        labelStatusMcp4716Info2 = new QLabel(frame_2);
        labelStatusMcp4716Info2->setObjectName(QString::fromUtf8("labelStatusMcp4716Info2"));
        labelStatusMcp4716Info2->setGeometry(QRect(700, 270, 111, 41));
        labelStatusMcp4716Info2->setFont(font5);
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(490, 220, 161, 140));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/MC4716.png")));
        labelStatusStm32f401reInfo1 = new QLabel(frame_2);
        labelStatusStm32f401reInfo1->setObjectName(QString::fromUtf8("labelStatusStm32f401reInfo1"));
        labelStatusStm32f401reInfo1->setGeometry(QRect(700, 430, 221, 41));
        labelStatusStm32f401reInfo1->setFont(font5);
        labelStatusStm32f401reInfo2 = new QLabel(frame_2);
        labelStatusStm32f401reInfo2->setObjectName(QString::fromUtf8("labelStatusStm32f401reInfo2"));
        labelStatusStm32f401reInfo2->setGeometry(QRect(700, 460, 111, 41));
        labelStatusStm32f401reInfo2->setFont(font5);
        labelStatusLmp90100SmInfo1 = new QLabel(frame_2);
        labelStatusLmp90100SmInfo1->setObjectName(QString::fromUtf8("labelStatusLmp90100SmInfo1"));
        labelStatusLmp90100SmInfo1->setGeometry(QRect(700, 50, 200, 41));
        labelStatusLmp90100SmInfo1->setFont(font5);
        labelStatusLmp90100Sm = new QLabel(frame_2);
        labelStatusLmp90100Sm->setObjectName(QString::fromUtf8("labelStatusLmp90100Sm"));
        labelStatusLmp90100Sm->setGeometry(QRect(700, 110, 221, 41));
        labelStatusLmp90100Sm->setFont(font5);
        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(480, 410, 171, 140));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/Nucleo.gif")));
        labelStatusMcp4716 = new QLabel(frame_2);
        labelStatusMcp4716->setObjectName(QString::fromUtf8("labelStatusMcp4716"));
        labelStatusMcp4716->setGeometry(QRect(700, 300, 221, 41));
        labelStatusMcp4716->setFont(font5);
        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(470, 30, 210, 140));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/LMP90100.gif")));
        labelStatusLmp90100SmInfo2 = new QLabel(frame_2);
        labelStatusLmp90100SmInfo2->setObjectName(QString::fromUtf8("labelStatusLmp90100SmInfo2"));
        labelStatusLmp90100SmInfo2->setGeometry(QRect(700, 80, 111, 41));
        labelStatusLmp90100SmInfo2->setFont(font5);
        labelStatusStm32f401re = new QLabel(frame_2);
        labelStatusStm32f401re->setObjectName(QString::fromUtf8("labelStatusStm32f401re"));
        labelStatusStm32f401re->setGeometry(QRect(700, 490, 221, 41));
        labelStatusStm32f401re->setFont(font5);
        labelStatusMcp4716Info1 = new QLabel(frame_2);
        labelStatusMcp4716Info1->setObjectName(QString::fromUtf8("labelStatusMcp4716Info1"));
        labelStatusMcp4716Info1->setGeometry(QRect(700, 240, 200, 41));
        labelStatusMcp4716Info1->setFont(font5);
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(940, 9, 930, 615));
        frame_3->setMinimumSize(QSize(0, 100));
        frame_3->setFrameShape(QFrame::Box);
        frame_3->setFrameShadow(QFrame::Plain);
        frame_3->setLineWidth(3);
        labelStatusRefThermocouple = new QLabel(frame_3);
        labelStatusRefThermocouple->setObjectName(QString::fromUtf8("labelStatusRefThermocouple"));
        labelStatusRefThermocouple->setGeometry(QRect(240, 80, 221, 41));
        QFont font6;
        font6.setPointSize(14);
        font6.setBold(false);
        font6.setWeight(50);
        labelStatusRefThermocouple->setFont(font6);
        label_8 = new QLabel(frame_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 50, 210, 41));
        label_8->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/Thermocouple.gif")));
        labelStatusRefThermocoupleInfo = new QLabel(frame_3);
        labelStatusRefThermocoupleInfo->setObjectName(QString::fromUtf8("labelStatusRefThermocoupleInfo"));
        labelStatusRefThermocoupleInfo->setGeometry(QRect(240, 50, 211, 41));
        labelStatusRefThermocoupleInfo->setFont(font6);
        label_16 = new QLabel(frame_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 130, 210, 41));
        label_16->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/Thermocouple.gif")));
        labelStatusThermocouple1Info = new QLabel(frame_3);
        labelStatusThermocouple1Info->setObjectName(QString::fromUtf8("labelStatusThermocouple1Info"));
        labelStatusThermocouple1Info->setGeometry(QRect(240, 130, 211, 41));
        labelStatusThermocouple1Info->setFont(font6);
        labelStatusThermocouple1 = new QLabel(frame_3);
        labelStatusThermocouple1->setObjectName(QString::fromUtf8("labelStatusThermocouple1"));
        labelStatusThermocouple1->setGeometry(QRect(240, 160, 221, 41));
        labelStatusThermocouple1->setFont(font6);
        label_17 = new QLabel(frame_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 210, 210, 41));
        label_17->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/Thermocouple.gif")));
        labelStatusThermocouple2Info = new QLabel(frame_3);
        labelStatusThermocouple2Info->setObjectName(QString::fromUtf8("labelStatusThermocouple2Info"));
        labelStatusThermocouple2Info->setGeometry(QRect(240, 210, 221, 41));
        labelStatusThermocouple2Info->setFont(font6);
        labelStatusThermocouple2 = new QLabel(frame_3);
        labelStatusThermocouple2->setObjectName(QString::fromUtf8("labelStatusThermocouple2"));
        labelStatusThermocouple2->setGeometry(QRect(240, 240, 231, 41));
        labelStatusThermocouple2->setFont(font6);
        label_18 = new QLabel(frame_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 290, 210, 41));
        label_18->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/Thermocouple.gif")));
        labelStatusThermocouple3Info = new QLabel(frame_3);
        labelStatusThermocouple3Info->setObjectName(QString::fromUtf8("labelStatusThermocouple3Info"));
        labelStatusThermocouple3Info->setGeometry(QRect(240, 290, 221, 41));
        labelStatusThermocouple3Info->setFont(font6);
        labelStatusThermocouple3 = new QLabel(frame_3);
        labelStatusThermocouple3->setObjectName(QString::fromUtf8("labelStatusThermocouple3"));
        labelStatusThermocouple3->setGeometry(QRect(240, 320, 221, 41));
        labelStatusThermocouple3->setFont(font6);
        label_19 = new QLabel(frame_3);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(10, 370, 210, 41));
        label_19->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/Thermocouple.gif")));
        labelStatusThermocouple4Info = new QLabel(frame_3);
        labelStatusThermocouple4Info->setObjectName(QString::fromUtf8("labelStatusThermocouple4Info"));
        labelStatusThermocouple4Info->setGeometry(QRect(240, 370, 211, 41));
        labelStatusThermocouple4Info->setFont(font6);
        labelStatusThermocouple4 = new QLabel(frame_3);
        labelStatusThermocouple4->setObjectName(QString::fromUtf8("labelStatusThermocouple4"));
        labelStatusThermocouple4->setGeometry(QRect(240, 400, 221, 41));
        labelStatusThermocouple4->setFont(font6);
        label_20 = new QLabel(frame_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(90, 460, 71, 81));
        label_20->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/RTDSMD.gif")));
        labelStatusRtd2Pt100SmInfo = new QLabel(frame_3);
        labelStatusRtd2Pt100SmInfo->setObjectName(QString::fromUtf8("labelStatusRtd2Pt100SmInfo"));
        labelStatusRtd2Pt100SmInfo->setGeometry(QRect(240, 460, 211, 41));
        labelStatusRtd2Pt100SmInfo->setFont(font6);
        labelStatusRtd2Pt100Sm = new QLabel(frame_3);
        labelStatusRtd2Pt100Sm->setObjectName(QString::fromUtf8("labelStatusRtd2Pt100Sm"));
        labelStatusRtd2Pt100Sm->setGeometry(QRect(240, 490, 221, 41));
        labelStatusRtd2Pt100Sm->setFont(font6);
        labelStatusRtd1Pt100SmInfo = new QLabel(frame_3);
        labelStatusRtd1Pt100SmInfo->setObjectName(QString::fromUtf8("labelStatusRtd1Pt100SmInfo"));
        labelStatusRtd1Pt100SmInfo->setGeometry(QRect(690, 50, 211, 41));
        labelStatusRtd1Pt100SmInfo->setFont(font6);
        label_31 = new QLabel(frame_3);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(480, 50, 181, 81));
        label_31->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/RTD.gif")));
        labelStatusRtd1Pt100Sm = new QLabel(frame_3);
        labelStatusRtd1Pt100Sm->setObjectName(QString::fromUtf8("labelStatusRtd1Pt100Sm"));
        labelStatusRtd1Pt100Sm->setGeometry(QRect(690, 80, 221, 41));
        labelStatusRtd1Pt100Sm->setFont(font6);
        labelStatusRtdPt1000CsInfo = new QLabel(frame_3);
        labelStatusRtdPt1000CsInfo->setObjectName(QString::fromUtf8("labelStatusRtdPt1000CsInfo"));
        labelStatusRtdPt1000CsInfo->setGeometry(QRect(690, 160, 211, 41));
        labelStatusRtdPt1000CsInfo->setFont(font6);
        label_32 = new QLabel(frame_3);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(480, 160, 181, 81));
        label_32->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/RTD.gif")));
        labelStatusRtdPt1000Cs = new QLabel(frame_3);
        labelStatusRtdPt1000Cs->setObjectName(QString::fromUtf8("labelStatusRtdPt1000Cs"));
        labelStatusRtdPt1000Cs->setGeometry(QRect(690, 190, 221, 41));
        labelStatusRtdPt1000Cs->setFont(font6);
        labelStatusPeltierCellInfo = new QLabel(frame_3);
        labelStatusPeltierCellInfo->setObjectName(QString::fromUtf8("labelStatusPeltierCellInfo"));
        labelStatusPeltierCellInfo->setGeometry(QRect(690, 290, 211, 41));
        labelStatusPeltierCellInfo->setFont(font6);
        labelStatusPeltierCell = new QLabel(frame_3);
        labelStatusPeltierCell->setObjectName(QString::fromUtf8("labelStatusPeltierCell"));
        labelStatusPeltierCell->setGeometry(QRect(690, 320, 221, 41));
        labelStatusPeltierCell->setFont(font6);
        label_40 = new QLabel(frame_3);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(470, 250, 201, 141));
        label_40->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/Peltier.gif")));

        gridLayout->addWidget(frame, 0, 0, 1, 1);

        applicationTabWidget->addTab(tabAutodetection, QString());
        tabUnitsDataViewer = new QWidget();
        tabUnitsDataViewer->setObjectName(QString::fromUtf8("tabUnitsDataViewer"));
        labelDevice = new QLabel(tabUnitsDataViewer);
        labelDevice->setObjectName(QString::fromUtf8("labelDevice"));
        labelDevice->setGeometry(QRect(280, 10, 710, 640));
        labelDevice->setPixmap(QPixmap(QString::fromUtf8(":/qss_icons/Images/DeviceTransparent.png")));
        labelUnitsDataSample1 = new QLabel(tabUnitsDataViewer);
        labelUnitsDataSample1->setObjectName(QString::fromUtf8("labelUnitsDataSample1"));
        labelUnitsDataSample1->setGeometry(QRect(1350, 15, 180, 31));
        QFont font7;
        font7.setPointSize(16);
        font7.setBold(true);
        font7.setWeight(75);
        labelUnitsDataSample1->setFont(font7);
        labelUnitsDataSample1->setMouseTracking(true);
        labelUnitsDataSample1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataSample2 = new QLabel(tabUnitsDataViewer);
        labelUnitsDataSample2->setObjectName(QString::fromUtf8("labelUnitsDataSample2"));
        labelUnitsDataSample2->setGeometry(QRect(1350, 55, 180, 20));
        labelUnitsDataSample2->setFont(font7);
        labelUnitsDataSample2->setMouseTracking(true);
        labelUnitsDataSample2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataRefSubst = new QLabel(tabUnitsDataViewer);
        labelUnitsDataRefSubst->setObjectName(QString::fromUtf8("labelUnitsDataRefSubst"));
        labelUnitsDataRefSubst->setGeometry(QRect(1350, 95, 180, 20));
        labelUnitsDataRefSubst->setFont(font7);
        labelUnitsDataRefSubst->setMouseTracking(true);
        labelUnitsDataRefSubst->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataSample3 = new QLabel(tabUnitsDataViewer);
        labelUnitsDataSample3->setObjectName(QString::fromUtf8("labelUnitsDataSample3"));
        labelUnitsDataSample3->setGeometry(QRect(1350, 135, 180, 20));
        labelUnitsDataSample3->setFont(font7);
        labelUnitsDataSample3->setMouseTracking(true);
        labelUnitsDataSample3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataSample4 = new QLabel(tabUnitsDataViewer);
        labelUnitsDataSample4->setObjectName(QString::fromUtf8("labelUnitsDataSample4"));
        labelUnitsDataSample4->setGeometry(QRect(1350, 175, 180, 20));
        labelUnitsDataSample4->setFont(font7);
        labelUnitsDataSample4->setMouseTracking(true);
        labelUnitsDataSample4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataSampleCarrierTemperature = new QLabel(tabUnitsDataViewer);
        labelUnitsDataSampleCarrierTemperature->setObjectName(QString::fromUtf8("labelUnitsDataSampleCarrierTemperature"));
        labelUnitsDataSampleCarrierTemperature->setGeometry(QRect(1350, 249, 180, 20));
        labelUnitsDataSampleCarrierTemperature->setFont(font7);
        labelUnitsDataSampleCarrierTemperature->setMouseTracking(true);
        labelUnitsDataSampleCarrierTemperature->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataHeaterTemperature = new QLabel(tabUnitsDataViewer);
        labelUnitsDataHeaterTemperature->setObjectName(QString::fromUtf8("labelUnitsDataHeaterTemperature"));
        labelUnitsDataHeaterTemperature->setGeometry(QRect(1350, 297, 180, 20));
        labelUnitsDataHeaterTemperature->setFont(font7);
        labelUnitsDataHeaterTemperature->setMouseTracking(true);
        labelUnitsDataHeaterTemperature->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataHeaterPower = new QLabel(tabUnitsDataViewer);
        labelUnitsDataHeaterPower->setObjectName(QString::fromUtf8("labelUnitsDataHeaterPower"));
        labelUnitsDataHeaterPower->setGeometry(QRect(1350, 347, 180, 20));
        labelUnitsDataHeaterPower->setFont(font7);
        labelUnitsDataHeaterPower->setMouseTracking(true);
        labelUnitsDataHeaterPower->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataSMPCBTemperature = new QLabel(tabUnitsDataViewer);
        labelUnitsDataSMPCBTemperature->setObjectName(QString::fromUtf8("labelUnitsDataSMPCBTemperature"));
        labelUnitsDataSMPCBTemperature->setGeometry(QRect(1350, 445, 180, 20));
        labelUnitsDataSMPCBTemperature->setFont(font7);
        labelUnitsDataSMPCBTemperature->setMouseTracking(true);
        labelUnitsDataSMPCBTemperature->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelUnitsDataFanPower = new QLabel(tabUnitsDataViewer);
        labelUnitsDataFanPower->setObjectName(QString::fromUtf8("labelUnitsDataFanPower"));
        labelUnitsDataFanPower->setGeometry(QRect(1350, 618, 180, 20));
        QFont font8;
        font8.setPointSize(16);
        font8.setBold(true);
        font8.setUnderline(false);
        font8.setWeight(75);
        font8.setStrikeOut(false);
        labelUnitsDataFanPower->setFont(font8);
        labelUnitsDataFanPower->setMouseTracking(true);
        labelUnitsDataFanPower->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line = new QFrame(tabUnitsDataViewer);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(980, 36, 381, 1));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);
        line_2 = new QFrame(tabUnitsDataViewer);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(980, 60, 355, 1));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setFrameShape(QFrame::HLine);
        line_3 = new QFrame(tabUnitsDataViewer);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(1335, 60, 1, 5));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setFrameShape(QFrame::VLine);
        line_4 = new QFrame(tabUnitsDataViewer);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(1335, 65, 24, 1));
        line_4->setFrameShadow(QFrame::Plain);
        line_4->setFrameShape(QFrame::HLine);
        line_5 = new QFrame(tabUnitsDataViewer);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(1311, 103, 51, 1));
        line_5->setFrameShadow(QFrame::Plain);
        line_5->setFrameShape(QFrame::HLine);
        line_6 = new QFrame(tabUnitsDataViewer);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setGeometry(QRect(980, 84, 331, 1));
        line_6->setFrameShadow(QFrame::Plain);
        line_6->setFrameShape(QFrame::HLine);
        line_7 = new QFrame(tabUnitsDataViewer);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setGeometry(QRect(1311, 84, 1, 20));
        line_7->setFrameShadow(QFrame::Plain);
        line_7->setFrameShape(QFrame::VLine);
        line_8 = new QFrame(tabUnitsDataViewer);
        line_8->setObjectName(QString::fromUtf8("line_8"));
        line_8->setGeometry(QRect(1288, 143, 74, 1));
        line_8->setFrameShadow(QFrame::Plain);
        line_8->setFrameShape(QFrame::HLine);
        line_9 = new QFrame(tabUnitsDataViewer);
        line_9->setObjectName(QString::fromUtf8("line_9"));
        line_9->setGeometry(QRect(1288, 106, 1, 37));
        line_9->setFrameShadow(QFrame::Plain);
        line_9->setFrameShape(QFrame::VLine);
        line_10 = new QFrame(tabUnitsDataViewer);
        line_10->setObjectName(QString::fromUtf8("line_10"));
        line_10->setGeometry(QRect(980, 106, 308, 1));
        line_10->setFrameShadow(QFrame::Plain);
        line_10->setFrameShape(QFrame::HLine);
        line_11 = new QFrame(tabUnitsDataViewer);
        line_11->setObjectName(QString::fromUtf8("line_11"));
        line_11->setGeometry(QRect(1260, 182, 101, 1));
        line_11->setFrameShadow(QFrame::Plain);
        line_11->setFrameShape(QFrame::HLine);
        line_12 = new QFrame(tabUnitsDataViewer);
        line_12->setObjectName(QString::fromUtf8("line_12"));
        line_12->setGeometry(QRect(980, 130, 281, 1));
        line_12->setFrameShadow(QFrame::Plain);
        line_12->setFrameShape(QFrame::HLine);
        line_13 = new QFrame(tabUnitsDataViewer);
        line_13->setObjectName(QString::fromUtf8("line_13"));
        line_13->setGeometry(QRect(1260, 130, 1, 53));
        line_13->setFrameShadow(QFrame::Plain);
        line_13->setFrameShape(QFrame::VLine);
        line_14 = new QFrame(tabUnitsDataViewer);
        line_14->setObjectName(QString::fromUtf8("line_14"));
        line_14->setGeometry(QRect(920, 156, 311, 1));
        line_14->setFrameShadow(QFrame::Plain);
        line_14->setFrameShape(QFrame::HLine);
        line_15 = new QFrame(tabUnitsDataViewer);
        line_15->setObjectName(QString::fromUtf8("line_15"));
        line_15->setGeometry(QRect(1230, 260, 131, 1));
        line_15->setFrameShadow(QFrame::Plain);
        line_15->setFrameShape(QFrame::HLine);
        line_16 = new QFrame(tabUnitsDataViewer);
        line_16->setObjectName(QString::fromUtf8("line_16"));
        line_16->setGeometry(QRect(1230, 156, 1, 104));
        line_16->setFrameShadow(QFrame::Plain);
        line_16->setFrameShape(QFrame::VLine);
        line_17 = new QFrame(tabUnitsDataViewer);
        line_17->setObjectName(QString::fromUtf8("line_17"));
        line_17->setGeometry(QRect(870, 207, 331, 1));
        line_17->setFrameShadow(QFrame::Plain);
        line_17->setFrameShape(QFrame::HLine);
        line_18 = new QFrame(tabUnitsDataViewer);
        line_18->setObjectName(QString::fromUtf8("line_18"));
        line_18->setGeometry(QRect(1202, 306, 159, 1));
        line_18->setFrameShadow(QFrame::Plain);
        line_18->setFrameShape(QFrame::HLine);
        line_19 = new QFrame(tabUnitsDataViewer);
        line_19->setObjectName(QString::fromUtf8("line_19"));
        line_19->setGeometry(QRect(1201, 207, 1, 100));
        line_19->setFrameShadow(QFrame::Plain);
        line_19->setFrameShape(QFrame::VLine);
        line_20 = new QFrame(tabUnitsDataViewer);
        line_20->setObjectName(QString::fromUtf8("line_20"));
        line_20->setGeometry(QRect(820, 276, 341, 1));
        line_20->setFrameShadow(QFrame::Plain);
        line_20->setFrameShape(QFrame::HLine);
        line_21 = new QFrame(tabUnitsDataViewer);
        line_21->setObjectName(QString::fromUtf8("line_21"));
        line_21->setGeometry(QRect(1161, 355, 200, 1));
        line_21->setFrameShadow(QFrame::Plain);
        line_21->setFrameShape(QFrame::HLine);
        line_22 = new QFrame(tabUnitsDataViewer);
        line_22->setObjectName(QString::fromUtf8("line_22"));
        line_22->setGeometry(QRect(1161, 276, 1, 80));
        line_22->setFrameShadow(QFrame::Plain);
        line_22->setFrameShape(QFrame::VLine);
        line_23 = new QFrame(tabUnitsDataViewer);
        line_23->setObjectName(QString::fromUtf8("line_23"));
        line_23->setGeometry(QRect(880, 455, 481, 1));
        line_23->setFrameShadow(QFrame::Plain);
        line_23->setFrameShape(QFrame::HLine);
        line_24 = new QFrame(tabUnitsDataViewer);
        line_24->setObjectName(QString::fromUtf8("line_24"));
        line_24->setGeometry(QRect(800, 627, 561, 1));
        line_24->setFrameShadow(QFrame::Plain);
        line_24->setFrameShape(QFrame::HLine);
        applicationTabWidget->addTab(tabUnitsDataViewer, QString());
        tabHeaterControl = new QWidget();
        tabHeaterControl->setObjectName(QString::fromUtf8("tabHeaterControl"));
        heaterTemperaturePlot = new QCustomPlot(tabHeaterControl);
        heaterTemperaturePlot->setObjectName(QString::fromUtf8("heaterTemperaturePlot"));
        heaterTemperaturePlot->setGeometry(QRect(360, 20, 1511, 611));
        tabWidgetHeaterPower = new QTabWidget(tabHeaterControl);
        tabWidgetHeaterPower->setObjectName(QString::fromUtf8("tabWidgetHeaterPower"));
        tabWidgetHeaterPower->setGeometry(QRect(10, 460, 341, 171));
        tabWidgetHeaterPower->setFont(font4);
        tabHeaterPowerOpenLoop = new QWidget();
        tabHeaterPowerOpenLoop->setObjectName(QString::fromUtf8("tabHeaterPowerOpenLoop"));
        labelHeaterPowerControlValueInfo = new QLabel(tabHeaterPowerOpenLoop);
        labelHeaterPowerControlValueInfo->setObjectName(QString::fromUtf8("labelHeaterPowerControlValueInfo"));
        labelHeaterPowerControlValueInfo->setGeometry(QRect(10, 20, 221, 21));
        QFont font9;
        font9.setPointSize(16);
        font9.setItalic(false);
        labelHeaterPowerControlValueInfo->setFont(font9);
        horizontalSliderHeaterPowerControlValue = new QSlider(tabHeaterPowerOpenLoop);
        horizontalSliderHeaterPowerControlValue->setObjectName(QString::fromUtf8("horizontalSliderHeaterPowerControlValue"));
        horizontalSliderHeaterPowerControlValue->setGeometry(QRect(10, 50, 311, 22));
        QFont font10;
        font10.setPointSize(16);
        horizontalSliderHeaterPowerControlValue->setFont(font10);
        horizontalSliderHeaterPowerControlValue->setMaximum(100);
        horizontalSliderHeaterPowerControlValue->setOrientation(Qt::Horizontal);
        textEditHeaterPowerControlValue = new QTextEdit(tabHeaterPowerOpenLoop);
        textEditHeaterPowerControlValue->setObjectName(QString::fromUtf8("textEditHeaterPowerControlValue"));
        textEditHeaterPowerControlValue->setGeometry(QRect(10, 90, 91, 31));
        textEditHeaterPowerControlValue->setFont(font10);
        textEditHeaterPowerControlValue->setLayoutDirection(Qt::LeftToRight);
        labelHeaterPowerOpenLoopPercent = new QLabel(tabHeaterPowerOpenLoop);
        labelHeaterPowerOpenLoopPercent->setObjectName(QString::fromUtf8("labelHeaterPowerOpenLoopPercent"));
        labelHeaterPowerOpenLoopPercent->setGeometry(QRect(110, 95, 21, 21));
        labelHeaterPowerOpenLoopPercent->setFont(font9);
        tabWidgetHeaterPower->addTab(tabHeaterPowerOpenLoop, QString());
        tabHeaterPowerSimpleFeedback = new QWidget();
        tabHeaterPowerSimpleFeedback->setObjectName(QString::fromUtf8("tabHeaterPowerSimpleFeedback"));
        tabWidgetHeaterPower->addTab(tabHeaterPowerSimpleFeedback, QString());
        tabHeaterPowerMFCFeedback = new QWidget();
        tabHeaterPowerMFCFeedback->setObjectName(QString::fromUtf8("tabHeaterPowerMFCFeedback"));
        tabWidgetHeaterPower->addTab(tabHeaterPowerMFCFeedback, QString());
        frame_4 = new QFrame(tabHeaterControl);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(0, 190, 351, 191));
        frame_4->setFrameShape(QFrame::Box);
        frame_4->setFrameShadow(QFrame::Raised);
        frame_4->setLineWidth(2);
        labelHeaterPowerFilename = new QLabel(frame_4);
        labelHeaterPowerFilename->setObjectName(QString::fromUtf8("labelHeaterPowerFilename"));
        labelHeaterPowerFilename->setGeometry(QRect(10, 10, 221, 21));
        QFont font11;
        font11.setPointSize(16);
        font11.setItalic(true);
        labelHeaterPowerFilename->setFont(font11);
        textEdit = new QTextEdit(frame_4);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 40, 331, 31));
        textEdit->setFont(font10);
        checkBoxHeaterPowerSaveToFile = new QCheckBox(frame_4);
        checkBoxHeaterPowerSaveToFile->setObjectName(QString::fromUtf8("checkBoxHeaterPowerSaveToFile"));
        checkBoxHeaterPowerSaveToFile->setGeometry(QRect(10, 156, 271, 21));
        checkBoxHeaterPowerSaveToFile->setFont(font10);
        labelHeaterPowerFileDataSampling = new QLabel(frame_4);
        labelHeaterPowerFileDataSampling->setObjectName(QString::fromUtf8("labelHeaterPowerFileDataSampling"));
        labelHeaterPowerFileDataSampling->setGeometry(QRect(10, 80, 221, 21));
        labelHeaterPowerFileDataSampling->setFont(font11);
        comboBoxHeaterPowerFileSps = new QComboBox(frame_4);
        comboBoxHeaterPowerFileSps->setObjectName(QString::fromUtf8("comboBoxHeaterPowerFileSps"));
        comboBoxHeaterPowerFileSps->setGeometry(QRect(10, 110, 241, 31));
        comboBoxHeaterPowerFileSps->setFont(font10);
        labelHeaterPowerControlMode_5 = new QLabel(tabHeaterControl);
        labelHeaterPowerControlMode_5->setObjectName(QString::fromUtf8("labelHeaterPowerControlMode_5"));
        labelHeaterPowerControlMode_5->setGeometry(QRect(160, 40, 61, 21));
        labelHeaterPowerControlMode_5->setFont(font11);
        labelHeaterPowerControlMode_3 = new QLabel(tabHeaterControl);
        labelHeaterPowerControlMode_3->setObjectName(QString::fromUtf8("labelHeaterPowerControlMode_3"));
        labelHeaterPowerControlMode_3->setGeometry(QRect(10, 40, 131, 21));
        QFont font12;
        font12.setPointSize(16);
        font12.setItalic(false);
        font12.setUnderline(true);
        labelHeaterPowerControlMode_3->setFont(font12);
        labelHeaterPowerControlMode_2 = new QLabel(tabHeaterControl);
        labelHeaterPowerControlMode_2->setObjectName(QString::fromUtf8("labelHeaterPowerControlMode_2"));
        labelHeaterPowerControlMode_2->setGeometry(QRect(10, 10, 61, 21));
        labelHeaterPowerControlMode_2->setFont(font12);
        labelHeaterPowerControlMode_4 = new QLabel(tabHeaterControl);
        labelHeaterPowerControlMode_4->setObjectName(QString::fromUtf8("labelHeaterPowerControlMode_4"));
        labelHeaterPowerControlMode_4->setGeometry(QRect(160, 10, 61, 21));
        labelHeaterPowerControlMode_4->setFont(font11);
        frame_5 = new QFrame(tabHeaterControl);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(0, 70, 351, 121));
        frame_5->setFrameShape(QFrame::Box);
        frame_5->setFrameShadow(QFrame::Raised);
        frame_5->setLineWidth(2);
        labelHeaterPowerPlotTemperature = new QLabel(frame_5);
        labelHeaterPowerPlotTemperature->setObjectName(QString::fromUtf8("labelHeaterPowerPlotTemperature"));
        labelHeaterPowerPlotTemperature->setGeometry(QRect(10, 10, 271, 21));
        labelHeaterPowerPlotTemperature->setFont(font11);
        checkBoxHeaterPowerPlotTemperature = new QCheckBox(frame_5);
        checkBoxHeaterPowerPlotTemperature->setObjectName(QString::fromUtf8("checkBoxHeaterPowerPlotTemperature"));
        checkBoxHeaterPowerPlotTemperature->setGeometry(QRect(10, 86, 211, 21));
        checkBoxHeaterPowerPlotTemperature->setFont(font10);
        comboBoxHeaterPowerPlotTemperatureSps = new QComboBox(frame_5);
        comboBoxHeaterPowerPlotTemperatureSps->setObjectName(QString::fromUtf8("comboBoxHeaterPowerPlotTemperatureSps"));
        comboBoxHeaterPowerPlotTemperatureSps->setGeometry(QRect(10, 40, 241, 31));
        comboBoxHeaterPowerPlotTemperatureSps->setFont(font10);
        labelHeaterPowerControlMode = new QLabel(tabHeaterControl);
        labelHeaterPowerControlMode->setObjectName(QString::fromUtf8("labelHeaterPowerControlMode"));
        labelHeaterPowerControlMode->setGeometry(QRect(10, 390, 151, 21));
        labelHeaterPowerControlMode->setFont(font11);
        comboBox = new QComboBox(tabHeaterControl);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 420, 241, 31));
        comboBox->setFont(font10);
        applicationTabWidget->addTab(tabHeaterControl, QString());
        tabSegmentsManager = new QWidget();
        tabSegmentsManager->setObjectName(QString::fromUtf8("tabSegmentsManager"));
        tabSegmentsManager->setEnabled(false);
        gridLayout_2 = new QGridLayout(tabSegmentsManager);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        applicationTabWidget->addTab(tabSegmentsManager, QString());
        tabDscDataView = new QWidget();
        tabDscDataView->setObjectName(QString::fromUtf8("tabDscDataView"));
        applicationTabWidget->addTab(tabDscDataView, QString());

        gridLayout_5->addWidget(applicationTabWidget, 1, 0, 1, 1);

        applicationDockWidget->setWidget(dockWidgetContents_4);
        dscDataDockWidget = new QDockWidget(centralWidget);
        dscDataDockWidget->setObjectName(QString::fromUtf8("dscDataDockWidget"));
        dscDataDockWidget->setGeometry(QRect(1580, 720, 340, 281));
        dscDataDockWidget->setFont(font1);
        dscDataDockWidget->setContextMenuPolicy(Qt::NoContextMenu);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        gridLayout_8 = new QGridLayout(dockWidgetContents_5);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        labelHeaterPower = new QLabel(dockWidgetContents_5);
        labelHeaterPower->setObjectName(QString::fromUtf8("labelHeaterPower"));
        QFont font13;
        font13.setPointSize(12);
        font13.setBold(false);
        font13.setWeight(50);
        labelHeaterPower->setFont(font13);

        gridLayout_8->addWidget(labelHeaterPower, 1, 1, 1, 1);

        labelHeaterPowerInfo = new QLabel(dockWidgetContents_5);
        labelHeaterPowerInfo->setObjectName(QString::fromUtf8("labelHeaterPowerInfo"));
        labelHeaterPowerInfo->setFont(font13);
        labelHeaterPowerInfo->setMouseTracking(true);

        gridLayout_8->addWidget(labelHeaterPowerInfo, 1, 0, 1, 1);

        labelSPHeaterTemperatureInfo = new QLabel(dockWidgetContents_5);
        labelSPHeaterTemperatureInfo->setObjectName(QString::fromUtf8("labelSPHeaterTemperatureInfo"));
        labelSPHeaterTemperatureInfo->setFont(font13);

        gridLayout_8->addWidget(labelSPHeaterTemperatureInfo, 3, 0, 1, 1);

        labelSMPCBTemperature = new QLabel(dockWidgetContents_5);
        labelSMPCBTemperature->setObjectName(QString::fromUtf8("labelSMPCBTemperature"));
        labelSMPCBTemperature->setFont(font13);

        gridLayout_8->addWidget(labelSMPCBTemperature, 5, 1, 1, 1);

        labelSPHeaterTemperature = new QLabel(dockWidgetContents_5);
        labelSPHeaterTemperature->setObjectName(QString::fromUtf8("labelSPHeaterTemperature"));
        labelSPHeaterTemperature->setFont(font13);

        gridLayout_8->addWidget(labelSPHeaterTemperature, 3, 1, 1, 1);

        labelHeaterTemperature = new QLabel(dockWidgetContents_5);
        labelHeaterTemperature->setObjectName(QString::fromUtf8("labelHeaterTemperature"));
        labelHeaterTemperature->setFont(font13);

        gridLayout_8->addWidget(labelHeaterTemperature, 2, 1, 1, 1);

        labelHeaterTemperatureInfo = new QLabel(dockWidgetContents_5);
        labelHeaterTemperatureInfo->setObjectName(QString::fromUtf8("labelHeaterTemperatureInfo"));
        labelHeaterTemperatureInfo->setFont(font13);
        labelHeaterTemperatureInfo->setMouseTracking(true);

        gridLayout_8->addWidget(labelHeaterTemperatureInfo, 2, 0, 1, 1);

        labelSampleCarrierTemperature = new QLabel(dockWidgetContents_5);
        labelSampleCarrierTemperature->setObjectName(QString::fromUtf8("labelSampleCarrierTemperature"));
        labelSampleCarrierTemperature->setFont(font13);

        gridLayout_8->addWidget(labelSampleCarrierTemperature, 4, 1, 1, 1);

        labelSMPCBTemperatureInfo = new QLabel(dockWidgetContents_5);
        labelSMPCBTemperatureInfo->setObjectName(QString::fromUtf8("labelSMPCBTemperatureInfo"));
        labelSMPCBTemperatureInfo->setFont(font13);

        gridLayout_8->addWidget(labelSMPCBTemperatureInfo, 5, 0, 1, 1);

        labelSampleCarrierTemperatureInfo = new QLabel(dockWidgetContents_5);
        labelSampleCarrierTemperatureInfo->setObjectName(QString::fromUtf8("labelSampleCarrierTemperatureInfo"));
        labelSampleCarrierTemperatureInfo->setFont(font13);

        gridLayout_8->addWidget(labelSampleCarrierTemperatureInfo, 4, 0, 1, 1);

        labelControlModeInfo = new QLabel(dockWidgetContents_5);
        labelControlModeInfo->setObjectName(QString::fromUtf8("labelControlModeInfo"));
        labelControlModeInfo->setFont(font13);
        labelControlModeInfo->setMouseTracking(true);

        gridLayout_8->addWidget(labelControlModeInfo, 0, 0, 1, 1);

        labelControlMode = new QLabel(dockWidgetContents_5);
        labelControlMode->setObjectName(QString::fromUtf8("labelControlMode"));
        labelControlMode->setFont(font13);
        labelControlMode->setMouseTracking(true);

        gridLayout_8->addWidget(labelControlMode, 0, 1, 1, 1);

        dscDataDockWidget->setWidget(dockWidgetContents_5);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1920, 21));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);
        QObject::connect(applicationTabWidget, SIGNAL(currentChanged(int)), MainWindow, SLOT(applicationTabWidgetChanged()));

        applicationTabWidget->setCurrentIndex(0);
        tabWidgetHeaterPower->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "DSC Application", 0, QApplication::UnicodeUTF8));
        loggerDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Logger", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = loggerTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Time", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = loggerTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Source", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = loggerTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Severity", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = loggerTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Log", 0, QApplication::UnicodeUTF8));
        faultsDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Active Faults", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = faultsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = faultsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = faultsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Unit", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = faultsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Subunit", 0, QApplication::UnicodeUTF8));
        applicationDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Application", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QString());
        label_4->setText(QString());
        labelStatusAds1248Info1->setText(QApplication::translate("MainWindow", "ADS1248", 0, QApplication::UnicodeUTF8));
        labelStatusAds1248Info2->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        labelStatusAds1248->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        labelStatusDrv595Info1->setText(QApplication::translate("MainWindow", "DRV595", 0, QApplication::UnicodeUTF8));
        labelStatusDrv595->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        labelStatusDrv595Info2->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        labelStatusLmp90100CsInfo1->setText(QApplication::translate("MainWindow", "LMP90100 CS", 0, QApplication::UnicodeUTF8));
        labelStatusLmp90100Cs->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        labelStatusLmp90100CsInfo2->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        labelStatusMcp4716Info2->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        labelStatusStm32f401reInfo1->setText(QApplication::translate("MainWindow", "STM32F4 NUCLEO", 0, QApplication::UnicodeUTF8));
        labelStatusStm32f401reInfo2->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        labelStatusLmp90100SmInfo1->setText(QApplication::translate("MainWindow", "LMP90100 SM", 0, QApplication::UnicodeUTF8));
        labelStatusLmp90100Sm->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_5->setText(QString());
        labelStatusMcp4716->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_6->setText(QString());
        labelStatusLmp90100SmInfo2->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        labelStatusStm32f401re->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        labelStatusMcp4716Info1->setText(QApplication::translate("MainWindow", "MCP4716", 0, QApplication::UnicodeUTF8));
        labelStatusRefThermocouple->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_8->setText(QString());
        labelStatusRefThermocoupleInfo->setText(QApplication::translate("MainWindow", "Ref. Thermocouple", 0, QApplication::UnicodeUTF8));
        label_16->setText(QString());
        labelStatusThermocouple1Info->setText(QApplication::translate("MainWindow", "Thermocouple 1", 0, QApplication::UnicodeUTF8));
        labelStatusThermocouple1->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_17->setText(QString());
        labelStatusThermocouple2Info->setText(QApplication::translate("MainWindow", "Thermocouple 2", 0, QApplication::UnicodeUTF8));
        labelStatusThermocouple2->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_18->setText(QString());
        labelStatusThermocouple3Info->setText(QApplication::translate("MainWindow", "Thermocouple 3", 0, QApplication::UnicodeUTF8));
        labelStatusThermocouple3->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_19->setText(QString());
        labelStatusThermocouple4Info->setText(QApplication::translate("MainWindow", "Thermocouple 4", 0, QApplication::UnicodeUTF8));
        labelStatusThermocouple4->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_20->setText(QString());
        labelStatusRtd2Pt100SmInfo->setText(QApplication::translate("MainWindow", "RTD2 Pt100 SM ", 0, QApplication::UnicodeUTF8));
        labelStatusRtd2Pt100Sm->setText(QApplication::translate("MainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        labelStatusRtd1Pt100SmInfo->setText(QApplication::translate("MainWindow", "RTD1 Pt100 SM", 0, QApplication::UnicodeUTF8));
        label_31->setText(QString());
        labelStatusRtd1Pt100Sm->setText(QApplication::translate("MainWindow", "Detection ongoing...", 0, QApplication::UnicodeUTF8));
        labelStatusRtdPt1000CsInfo->setText(QApplication::translate("MainWindow", "RTD Pt1000 CS", 0, QApplication::UnicodeUTF8));
        label_32->setText(QString());
        labelStatusRtdPt1000Cs->setText(QApplication::translate("MainWindow", "Detection ongoing...", 0, QApplication::UnicodeUTF8));
        labelStatusPeltierCellInfo->setText(QApplication::translate("MainWindow", "Peltier Cell", 0, QApplication::UnicodeUTF8));
        labelStatusPeltierCell->setText(QApplication::translate("MainWindow", "Detection ongoing...", 0, QApplication::UnicodeUTF8));
        label_40->setText(QString());
        applicationTabWidget->setTabText(applicationTabWidget->indexOf(tabAutodetection), QApplication::translate("MainWindow", "Device Startup", 0, QApplication::UnicodeUTF8));
        labelDevice->setText(QString());
        labelUnitsDataSample1->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataSample2->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataRefSubst->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataSample3->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataSample4->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataSampleCarrierTemperature->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataHeaterTemperature->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataHeaterPower->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataSMPCBTemperature->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelUnitsDataFanPower->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        applicationTabWidget->setTabText(applicationTabWidget->indexOf(tabUnitsDataViewer), QApplication::translate("MainWindow", "Units Data Viewer", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerControlValueInfo->setText(QApplication::translate("MainWindow", "Control Value", 0, QApplication::UnicodeUTF8));
        textEditHeaterPowerControlValue->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:16pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">N/A</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerOpenLoopPercent->setText(QApplication::translate("MainWindow", "%", 0, QApplication::UnicodeUTF8));
        tabWidgetHeaterPower->setTabText(tabWidgetHeaterPower->indexOf(tabHeaterPowerOpenLoop), QApplication::translate("MainWindow", "Open Loop", 0, QApplication::UnicodeUTF8));
        tabWidgetHeaterPower->setTabText(tabWidgetHeaterPower->indexOf(tabHeaterPowerSimpleFeedback), QApplication::translate("MainWindow", "Simple Feedback", 0, QApplication::UnicodeUTF8));
        tabWidgetHeaterPower->setTabText(tabWidgetHeaterPower->indexOf(tabHeaterPowerMFCFeedback), QApplication::translate("MainWindow", "MFC Feedback", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerFilename->setText(QApplication::translate("MainWindow", "Filename", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:16pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">dataFile.txt</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        checkBoxHeaterPowerSaveToFile->setText(QApplication::translate("MainWindow", "Save Heater Data To File", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerFileDataSampling->setText(QApplication::translate("MainWindow", "Data Save Frequency", 0, QApplication::UnicodeUTF8));
        comboBoxHeaterPowerFileSps->clear();
        comboBoxHeaterPowerFileSps->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1 SPS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "2 SPS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "5 SPS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "10 SPS", 0, QApplication::UnicodeUTF8)
        );
        labelHeaterPowerControlMode_5->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerControlMode_3->setText(QApplication::translate("MainWindow", "Temperature", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerControlMode_2->setText(QApplication::translate("MainWindow", "Power", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerControlMode_4->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerPlotTemperature->setText(QApplication::translate("MainWindow", "Plot Sampling Frequency", 0, QApplication::UnicodeUTF8));
        checkBoxHeaterPowerPlotTemperature->setText(QApplication::translate("MainWindow", "Plot Data", 0, QApplication::UnicodeUTF8));
        comboBoxHeaterPowerPlotTemperatureSps->clear();
        comboBoxHeaterPowerPlotTemperatureSps->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1 SPS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "2 SPS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "5 SPS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "10 SPS", 0, QApplication::UnicodeUTF8)
        );
        labelHeaterPowerControlMode->setText(QApplication::translate("MainWindow", "Control Mode", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "No Control", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Open Loop", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Simple Feedback", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "MFC Feedback", 0, QApplication::UnicodeUTF8)
        );
        applicationTabWidget->setTabText(applicationTabWidget->indexOf(tabHeaterControl), QApplication::translate("MainWindow", "Heater Power Control", 0, QApplication::UnicodeUTF8));
        applicationTabWidget->setTabText(applicationTabWidget->indexOf(tabSegmentsManager), QApplication::translate("MainWindow", "Segments Configurator", 0, QApplication::UnicodeUTF8));
        applicationTabWidget->setTabText(applicationTabWidget->indexOf(tabDscDataView), QApplication::translate("MainWindow", "DSC Data Viewer", 0, QApplication::UnicodeUTF8));
        dscDataDockWidget->setWindowTitle(QApplication::translate("MainWindow", "DSC / Device Data", 0, QApplication::UnicodeUTF8));
        labelHeaterPower->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
        labelHeaterPowerInfo->setText(QApplication::translate("MainWindow", "Heater Power", 0, QApplication::UnicodeUTF8));
        labelSPHeaterTemperatureInfo->setText(QApplication::translate("MainWindow", "<html><head/><body><p>SP Heater Temp.</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelSMPCBTemperature->setText(QApplication::translate("MainWindow", "<html><head/><body><p>N/A</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelSPHeaterTemperature->setText(QApplication::translate("MainWindow", "<html><head/><body><p>N/A</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelHeaterTemperature->setText(QApplication::translate("MainWindow", "<html><head/><body><p>N/A</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelHeaterTemperatureInfo->setText(QApplication::translate("MainWindow", "Heater Temp.", 0, QApplication::UnicodeUTF8));
        labelSampleCarrierTemperature->setText(QApplication::translate("MainWindow", "<html><head/><body><p>N/A</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelSMPCBTemperatureInfo->setText(QApplication::translate("MainWindow", "<html><head/><body><p>SM PCB Temp.</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelSampleCarrierTemperatureInfo->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Sample Carrier</p><p>Temp.</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelControlModeInfo->setText(QApplication::translate("MainWindow", "Control Mode", 0, QApplication::UnicodeUTF8));
        labelControlMode->setText(QApplication::translate("MainWindow", "N/A", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
