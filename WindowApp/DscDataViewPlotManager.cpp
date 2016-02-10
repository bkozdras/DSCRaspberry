#include "DscDataViewPlotManager.hpp"
#include <QtGui/QAction>
#include <qobject.h>
#include <qtimer.h>
#include <iostream>

void DscDataViewPlotManager::initialize(QCustomPlot* plot, QTimer* plotTimer)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    mPlot = plot;
    mPlotTimer = plotTimer;

    {
        QCPPlotTitle* title = new QCPPlotTitle(mPlot);
        {
            QFont font("Arial", 16);
            font.setBold(true);
            title->setFont(font);
        }
        title->setTextColor(QColor(51, 0, 0));
        title->setText("DSC Measured Data");
        mPlot->plotLayout()->insertRow(0);
        mPlot->plotLayout()->addElement(0, 0, title);
    }

    {
        QFont font("Arial", 14);
        font.setBold(true);

        mPlot->xAxis->setLabel("Time");

        mPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        mPlot->xAxis->setDateTimeFormat("hh:mm:ss");
        mPlot->xAxis->setAutoTickStep(false);
        mPlot->xAxis->setTickStep(60);
        mPlot->xAxis->setLabelColor(QColor(51, 0, 0));
        mPlot->xAxis->setLabelFont(font);

        /*
        mPlot->yAxis->setLabel("Sample Carrier Temperature [oC]");
        mPlot->yAxis->setLabelColor(QColor(51, 0, 0));
        mPlot->yAxis->setLabelFont(font);

        mPlot->yAxis2->setVisible(true);
        mPlot->yAxis2->setLabel("DSC Value [uV]");
        mPlot->yAxis2->setLabelColor(QColor(51, 0, 0));
        mPlot->yAxis2->setLabelFont(font);
        mPlot->yAxis2->setTickLength(3, 3);
        mPlot->yAxis2->setSubTickLength(1, 1);
        mPlot->yAxis2->setTickLabels(true);
        mPlot->yAxis2->setTicks(true);
        */

        mPlot->yAxis->setVisible(true);
        mPlot->yAxis->setLabel("DSC [uV]");
        mPlot->yAxis->setLabelColor(QColor(51, 0, 0));
        mPlot->yAxis->setLabelFont(font);
        mPlot->yAxis->setTickLength(3, 3);
        mPlot->yAxis->setSubTickLength(1, 1);
        mPlot->yAxis->setTickLabels(true);
        mPlot->yAxis->setTicks(true);

        mPlot->axisRect()->setupFullAxesBox();
    }

    {
        QObject::connect(mPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), mPlot->xAxis2, SLOT(setRange(QCPRange)));
        QObject::connect(mPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis2, SLOT(setRange(QCPRange)));
    }

    mIsGraphAddedToPlots = false;
    mIsDrawingActive = false;

    mMaxThermocoupleValue = -999999999999.0;
    mMinThermocoupleValue = 9999999999999.0;
}

void DscDataViewPlotManager::deinitialize()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
}

void DscDataViewPlotManager::startDrawing()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    _startDrawing();
}

void DscDataViewPlotManager::stopDrawing()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    _stopDrawing();
}

void DscDataViewPlotManager::clearDrawnData()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    _clearDrawnData();
}

void DscDataViewPlotManager::_startDrawing()
{
    if (!mIsGraphAddedToPlots)
    {
        mPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

        {
            mPlot->legend->setVisible(true);
            QFont font("Arial", 9);
            font.setItalic(true);
            mPlot->legend->setFont(font);
            mPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignLeft);
        }
        /*
        {
            mPlot->addGraph(mPlot->xAxis, mPlot->yAxis);
            mPlot->graph(0)->setName("SC Temp.");
            mPlot->graph(0)->setPen(QPen(Qt::red));
        }*/

        {
            mPlot->addGraph(mPlot->xAxis, mPlot->yAxis);
            mPlot->graph(0)->setName("Sample 1");
            mPlot->graph(0)->setPen(QPen(Qt::blue));

            mPlot->addGraph(mPlot->xAxis, mPlot->yAxis);
            mPlot->graph(1)->setName("Sample 2");
            mPlot->graph(1)->setPen(QPen(Qt::green));

            mPlot->addGraph(mPlot->xAxis, mPlot->yAxis);
            mPlot->graph(2)->setName("Sample 3");
            mPlot->graph(2)->setPen(QPen(Qt::gray));

            mPlot->addGraph(mPlot->xAxis, mPlot->yAxis);
            mPlot->graph(3)->setName("Sample 4");
            mPlot->graph(3)->setPen(QPen(Qt::magenta));
        }

        mPlot->replot();

        mIsGraphAddedToPlots = true;
    }

    {
        mPlotTimer->start(1000);
    }

    mIsDrawingActive = true;
}

void DscDataViewPlotManager::_stopDrawing()
{
    mPlotTimer->stop();
    mIsDrawingActive = false;
}

void DscDataViewPlotManager::_clearDrawnData()
{
    mPlot->clearGraphs();
    for (int iter = 0; mPlot->graphCount() > iter; ++iter)
    {
        mPlot->removeGraph(iter);
    }

    mPlot->replot();

    mIsGraphAddedToPlots = false;
}

void DscDataViewPlotManager::addNewDataToPlotCallback()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    auto time = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    auto temperature = DSC::DataManager::getData(EDataType::SampleCarrierTemperature);
    auto sample1 = DSC::DataManager::getData(EDataType::FilteredThermocouple1);
    auto sample2 = DSC::DataManager::getData(EDataType::FilteredThermocouple2);
    auto sample3 = DSC::DataManager::getData(EDataType::FilteredThermocouple3);
    auto sample4 = DSC::DataManager::getData(EDataType::FilteredThermocouple4);
    /*
    {
        mPlot->graph(0)->addData(time, temperature);
        mPlot->graph(0)->removeDataBefore(time - 600);
        mPlot->graph(0)->rescaleAxes();
    }*/

    {
        mPlot->graph(0)->addData(time, sample1);
        mPlot->graph(0)->removeDataBefore(time - 600);
        mPlot->graph(0)->rescaleAxes();

        mPlot->graph(1)->addData(time, sample2);
        mPlot->graph(1)->removeDataBefore(time - 600);
        //mPlot->graph(1)->rescaleAxes();

        mPlot->graph(2)->addData(time, sample3);
        mPlot->graph(2)->removeDataBefore(time - 600);
        //mPlot->graph(2)->rescaleAxes();

        mPlot->graph(3)->addData(time, sample4);
        mPlot->graph(3)->removeDataBefore(time - 600);
        //mPlot->graph(3)->rescaleAxes();
    }
    /*
    auto updateScale = 
        [](const double & value)
        {
            if (mMinThermocoupleValue > value)
            {
                mMinThermocoupleValue = value;
            }

            if (mMaxThermocoupleValue < value)
            {
                mMaxThermocoupleValue = value;
            }
        };

    updateScale(sample1);
    updateScale(sample2);
    updateScale(sample3);
    updateScale(sample4);*/

    mPlot->xAxis->setRange(time, 600, Qt::AlignRight);
    mPlot->replot();
}

std::mutex DscDataViewPlotManager::mMtx;
QCustomPlot* DscDataViewPlotManager::mPlot;
QTimer* DscDataViewPlotManager::mPlotTimer;
std::vector<QCPPlottableLegendItem*> DscDataViewPlotManager::mActiveLegends;
bool DscDataViewPlotManager::mIsGraphAddedToPlots;
bool DscDataViewPlotManager::mIsDrawingActive;
double DscDataViewPlotManager::mMaxThermocoupleValue;
double DscDataViewPlotManager::mMinThermocoupleValue;