#pragma once

#include "../qcustomplot.h"
#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <qtimer.h>
#include <vector>
#include "../Core/DSC/DataManager.hpp"
#include "../Core/DSC/EControlMode.hpp"
#include "../Core/DSC/EDataType.hpp"

class DscDataViewPlotManager : public boost::noncopyable
{
public:

    static void initialize(QCustomPlot* plot, QTimer* plotTimer);
    static void deinitialize();

    static void startDrawing();
    static void stopDrawing();
    static void clearDrawnData();

    static void addNewDataToPlotCallback();

private:

    static void _startDrawing();
    static void _stopDrawing();
    static void _clearDrawnData();

    static std::mutex mMtx;
    static QTimer* mPlotTimer;
    static QCustomPlot* mPlot;
    static std::vector<QCPPlottableLegendItem*> mActiveLegends;
    static bool mIsGraphAddedToPlots;
    static bool mIsDrawingActive;
    static double mMaxThermocoupleValue;
    static double mMinThermocoupleValue;
};