#include "HeaterTemperaturePlotManager.hpp"
#include <QtGui/QAction>
#include <qobject.h>
#include <qtimer.h>

void HeaterTemperaturePlotManager::initialize(QCustomPlot* plot, std::shared_ptr<QTimer> plotTimer)
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
        title->setText("Heater Temperature Value");
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
        mPlot->yAxis->setLabel("Temperature [oC]");
        mPlot->yAxis->setLabelColor(QColor(51, 0, 0));
        mPlot->yAxis->setLabelFont(font);

        mPlot->axisRect()->setupFullAxesBox();
    }

    {
        QObject::connect(mPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), mPlot->xAxis2, SLOT(setRange(QCPRange)));
        QObject::connect(mPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis2, SLOT(setRange(QCPRange)));
    }

    mIsGraphAddedToPlots = false;
    mIsDrawingActive = false;
}

void HeaterTemperaturePlotManager::deinitialize()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
}

void HeaterTemperaturePlotManager::startDrawing()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    _startDrawing();
}

void HeaterTemperaturePlotManager::stopDrawing()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    _stopDrawing();
}

void HeaterTemperaturePlotManager::clearDrawnData()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    _clearDrawnData();
}

void HeaterTemperaturePlotManager::_startDrawing()
{
    mControlMode = DSC::DataManager::getControlMode();

    if (!mIsGraphAddedToPlots)
    {
        {
            mPlot->addGraph();
            mPlot->graph(0)->setName("PV");
            mPlot->graph(0)->setPen(QPen(Qt::red));
        }

        if (EControlMode::SimpleFeedback == mControlMode || EControlMode::MFCFeedback == mControlMode)
        {
            mPlot->addGraph();
            mPlot->graph(1)->setName("SP");
            mPlot->graph(1)->setPen(QPen(Qt::blue));
        }

        {
            QFont font("Arial", 12);
            font.setItalic(true);
            mPlot->legend->setFont(font);
            mPlot->legend->setLayer("legend");

            for (int iter = 0; mPlot->graphCount() > iter; ++iter)
            {
                mActiveLegends.push_back(new QCPPlottableLegendItem(mPlot->legend, mPlot->graph(iter)));
                mPlot->legend->addItem(mActiveLegends.back());
            }

            mPlot->legend->setVisible(true);
        }

        mPlot->replot();

        mIsGraphAddedToPlots = true;
    }

    {
        auto samplingSpeed = static_cast<uint64_t>(DSC::DataManager::getData(EDataType::HeaterPowerControlPlotDataSampling));
        samplingSpeed *= 1000U;
        mPlotTimer->start(samplingSpeed);
    }

    mIsDrawingActive = true;
}

void HeaterTemperaturePlotManager::_stopDrawing()
{
    mPlotTimer->stop();
    mIsDrawingActive = false;
}

void HeaterTemperaturePlotManager::_clearDrawnData()
{
    for (auto & legend : mActiveLegends)
    {
        mPlot->legend->removeItem(legend);
    }
    mActiveLegends.clear();

    mPlot->clearGraphs();
    for (int iter = 0; mPlot->graphCount() > iter; ++iter)
    {
        mPlot->removeGraph(iter);
    }

    mPlot->replot();

    mIsGraphAddedToPlots = false;
}

void HeaterTemperaturePlotManager::newSamplingNotification()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    if (mIsDrawingActive)
    {
        _stopDrawing();
        _startDrawing();
    }
}

void HeaterTemperaturePlotManager::newControlModeNotification()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    
    switch (DSC::DataManager::getControlMode())
    {
        case EControlMode::NotSet:
        {
            _stopDrawing();
            break;
        }

        case EControlMode::OpenLoop:
        case EControlMode::SimpleFeedback:
        case EControlMode::MFCFeedback:
        {
            _stopDrawing();
            _clearDrawnData();
            _startDrawing();
            break;
        }

        default :
            break;
    }
}

void HeaterTemperaturePlotManager::addNewDataToPlotCallback()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    auto time = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    auto temperature = DSC::DataManager::getData(EDataType::HeaterTemperature);

    mPlot->graph(0)->addData(time, temperature);
    mPlot->graph(0)->removeDataBefore(time - 600);
    mPlot->graph(0)->rescaleAxes();

    if (EControlMode::SimpleFeedback == mControlMode || EControlMode::MFCFeedback == mControlMode)
    {
        auto spTemperature = DSC::DataManager::getData(EDataType::SPHeaterTemperature);
        mPlot->graph(1)->addData(time, spTemperature);
        mPlot->graph(1)->removeDataBefore(time - 600);
        mPlot->graph(1)->rescaleAxes();
    }

    mPlot->xAxis->setRange(time + 0.5, 600, Qt::AlignRight);
    mPlot->replot();
}

std::mutex HeaterTemperaturePlotManager::mMtx;
QCustomPlot* HeaterTemperaturePlotManager::mPlot;
std::shared_ptr<QTimer> HeaterTemperaturePlotManager::mPlotTimer;
std::vector<QCPPlottableLegendItem*> HeaterTemperaturePlotManager::mActiveLegends;
EControlMode HeaterTemperaturePlotManager::mControlMode;
bool HeaterTemperaturePlotManager::mIsGraphAddedToPlots;
bool HeaterTemperaturePlotManager::mIsDrawingActive;