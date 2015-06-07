#include "SegmentsProgramPlotManager.hpp"

#include <algorithm>

void SegmentsProgramPlotManager::initialize(QCustomPlot* plot)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    mPlot = plot;
    mIsSomeDataOnPlot = false;

    {
        QCPPlotTitle* title = new QCPPlotTitle(mPlot);
        {
            QFont font("Arial", 16);
            font.setBold(true);
            title->setFont(font);
        }
        title->setTextColor(QColor(51, 0, 0));
        title->setText("Applied Segments Program");
        mPlot->plotLayout()->insertRow(0);
        mPlot->plotLayout()->addElement(0, 0, title);
    }

    {
        QFont font("Arial", 14);
        font.setBold(true);

        mPlot->xAxis->setLabel("Time [s]");
        mPlot->xAxis->setLabelColor(QColor(51, 0, 0));
        mPlot->xAxis->setLabelFont(font);
        mPlot->yAxis->setLabel("Temperature [oC]");
        mPlot->yAxis->setLabelColor(QColor(51, 0, 0));
        mPlot->yAxis->setLabelFont(font);

        mPlot->axisRect()->setupFullAxesBox();
    }

    plot->addGraph();
}

void SegmentsProgramPlotManager::deinitialize()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
}

void SegmentsProgramPlotManager::addProgramToPlot(double startTemperature, double stopTemperature, double intervalInSeconds)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    if (!mIsSomeDataOnPlot)
    {
        mTimeXAxis.push_back(0.0);
        mTemperatureYAxis.push_back(startTemperature);
        mIsSomeDataOnPlot = true;
    }

    mTimeXAxis.push_back(mTimeXAxis.back() + 0.01);
    mTimeXAxis.push_back(mTimeXAxis.back() + intervalInSeconds);

    mTemperatureYAxis.push_back(startTemperature);
    mTemperatureYAxis.push_back(stopTemperature);

    mPlot->graph(0)->setData(mTimeXAxis, mTemperatureYAxis);
    mPlot->xAxis->setRange(findMinValue(mTimeXAxis), findMaxValue(mTimeXAxis));
    mPlot->yAxis->setRange(findMinValue(mTemperatureYAxis) - 1.0, findMaxValue(mTemperatureYAxis) + 1.0);

    mPlot->replot();
}

void SegmentsProgramPlotManager::clearDrawnData()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    mTimeXAxis.clear();
    mTemperatureYAxis.clear();
    mPlot->graph(0)->setData(mTimeXAxis, mTemperatureYAxis);
    mPlot->replot();
}

double SegmentsProgramPlotManager::findMinValue(const QVector<double> & vector)
{
    double minValue = 99999999.0;
    for (auto iter = vector.begin(); vector.end() > iter; ++iter)
    {
        if (*iter < minValue)
        {
            minValue = *iter;
        }
    }

    return minValue;
}

double SegmentsProgramPlotManager::findMaxValue(const QVector<double> & vector)
{
    double maxValue = 0.0;
    for (auto iter = vector.begin(); vector.end() > iter; ++iter)
    {
        if (*iter > maxValue)
        {
            maxValue = *iter;
        }
    }

    return maxValue;
}

std::mutex SegmentsProgramPlotManager::mMtx;
QCustomPlot* SegmentsProgramPlotManager::mPlot;
QVector<double> SegmentsProgramPlotManager::mTimeXAxis;
QVector<double> SegmentsProgramPlotManager::mTemperatureYAxis;
bool SegmentsProgramPlotManager::mIsSomeDataOnPlot;