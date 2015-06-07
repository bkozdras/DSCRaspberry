#pragma once

#include "../qcustomplot.h"
#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <qvector>
#include "../Core/DSC/DataManager.hpp"
#include "../Core/DSC/EControlMode.hpp"
#include "../Core/DSC/EDataType.hpp"

class SegmentsProgramPlotManager : public boost::noncopyable
{
    public:

        static void initialize(QCustomPlot* plot);
        static void deinitialize();

        static void addProgramToPlot(double startTemperature, double stopTemperature, double intervalInSeconds);
        static void clearDrawnData();

    private:

        static double findMinValue(const QVector<double> & vector);
        static double findMaxValue(const QVector<double> & vector);
        
        static std::mutex mMtx;
        static QCustomPlot* mPlot;
        static QVector<double> mTimeXAxis;
        static QVector<double> mTemperatureYAxis;
        static bool mIsSomeDataOnPlot;
};