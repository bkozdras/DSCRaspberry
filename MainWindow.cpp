#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "InfoDialog.h"
#include <QMessageBox>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <sstream>
#include "../Core/FaultManagement/FaultManager.hpp"
#include "../Core/System/ThreadPool.hpp"
#include "../Core/System/TimerManager.hpp"
#include "../Core/System/DeviceConfigurator.hpp"
#include "../Core/Utilities/Logger.hpp"
#include "../Core/Utilities/ToStringConverter.hpp"
#include "../Core/Utilities/Utilities.hpp"
#include "../Core/DevicePeripherals/UnitsDetector.hpp"
#include "../Core/DSC/IntegratedCircuitsManager.hpp"
#include "../Core/DSC/DataManager.hpp"
#include "../Core/DSC/HeaterManager.hpp"
#include "../Core/DSC/FileDataManager.hpp"
#include "../Core/DSC/SampleCarrierManager.hpp"
#include "../WindowApp/HeaterTemperaturePlotManager.hpp"
#include "../WindowApp/SegmentsProgramPlotManager.hpp"
#include "../WindowApp/DscDataViewPlotManager.hpp"
#include <map>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mInfoDialog = nullptr;
    ui->setupUi(this);
    
    mActiveSegment = 1U;

    setupDockLogger();
    setupLoggerTable();
    setupFaultsTable();
    setupFooterTable();
    setupAutodetectionTabLabels();
    setupDscDeviceDataLabels();
    setupUnitsDataViewer();
    setupHeaterPowerControl();
    setupSegmentsConfigurator();
    setupCallibrationSettings();
    setupDscDataView();

    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::run()
{
    openInfoDialog();
}

void MainWindow::applicationTabWidgetChanged()
{
    const auto deviceStartupTab = 0;
    const auto unitsDataViewerTab = 1;
    const auto heaterPowerControlTab = 2;
    const auto segmentsConfiguratorTab = 3;
    const auto callibrationSettingsTab = 4;
    const auto dscDataViewerTab = 5;

    static auto actualTab = deviceStartupTab;

    switch (actualTab)
    {
        case unitsDataViewerTab:
        {
            unitsDataViewerStopWorking();
            break;
        }

        case heaterPowerControlTab:
        {
            heaterPowerControlStopWorking();
            break;
        }

        case segmentsConfiguratorTab:
        {
            break;
        }

        case callibrationSettingsTab:
        {
            break;
        }

        case dscDataViewerTab:
        {
            dscDataViewStopWorking();
            break;
        }

        default :
            break;
    }

    switch (ui->applicationTabWidget->currentIndex())
    {
        case unitsDataViewerTab:
        {
            unitsDataViewerStartWorking();
            break;
        }

        case heaterPowerControlTab:
        {
            heaterPowerControlStartWorking();
            break;
        }

        case segmentsConfiguratorTab:
        {
            segmentsConfiguratorStartWorking();
            break;
        }

        case callibrationSettingsTab:
        {
            callibrationSettingsStartWorking();
            break;
        }

        case dscDataViewerTab:
        {
            dscDataViewStartWorking();
            break;
        }

        default:
            break;
    }

    actualTab = ui->applicationTabWidget->currentIndex();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    DeviceConfigurator::shutdownSystem();
    FaultManager::deregisterNotificationCallback(mNewFaultCallbackId);
    Logger::deregisterGuiTableLogCallback();
}

void MainWindow::openInfoDialog()
{
    mInfoDialog = new InfoDialog
    (
        [this]()
        {
            closeInfoDialog();
            DeviceConfigurator::configureSystem();
        }
    );
    mInfoDialog->show();
    mInfoDialog->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
    mInfoDialog->exec();
}

void MainWindow::closeInfoDialog()
{
    QMetaObject::invokeMethod(mInfoDialog, "close", Qt::QueuedConnection);
}

void MainWindow::setupApplicationDockLogger()
{
    {
        QFont font("Arial", 12);
        font.setBold(true);
        ui->applicationTabWidget->setFont(font);
        ui->tabAutodetection->setFont(font);
        ui->tabDscDataView->setFont(font);
        ui->tabHeaterControl->setFont(font);
        ui->tabSegmentsConfigurator->setFont(font);
        ui->tabUnitsDataViewer->setFont(font);
    }
}

void MainWindow::setupDockLogger()
{
    ui->loggerDockWidget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

void MainWindow::setupLoggerTable()
{
    std::lock_guard<std::mutex> lockGuard(mLoggerTableMtx);
    mNumberOfDebugLogs = 0U;
    mNumberOfLogs = 0U;

    const auto timeColumnWidth = 70;
    const auto sourceColumnWidth = 85;
    const auto severityColumnWidth = 70;
    const auto logColumnWidth = 958;
    ui->loggerTable->setColumnWidth(0, timeColumnWidth);
    ui->loggerTable->setColumnWidth(1, sourceColumnWidth);
    ui->loggerTable->setColumnWidth(2, severityColumnWidth);
    ui->loggerTable->setColumnWidth(3, logColumnWidth);

    Logger::registerGuiTableLogCallback
    (
        [this](std::shared_ptr<Logger::StoredLog> log)
        {
            addNewLogToLoggerTable(log);
        }
    );
}

void MainWindow::addNewLogToLoggerTable(std::shared_ptr<Logger::StoredLog> log)
{
    std::lock_guard<std::mutex> lockGuard(mLoggerTableMtx);

    if (100 < mNumberOfDebugLogs)
    {
        //removeTheOldestDebugLogFromLoggerTable();
    }

    {
        ui->loggerTable->insertRow(0);
        //QMetaObject::invokeMethod(ui->loggerTable, "insertRow", Qt::QueuedConnection, Q_ARG(int, 0));
        for (auto iter = 0; 4 > iter; ++iter)
        {
            ui->loggerTable->setItem(0, iter, new QTableWidgetItem());

            {
                QFont font("Arial", 12);
                QBrush brush;
                brush.setColor(getColorForLogSeverity(log->severity));
                ui->loggerTable->item(0, iter)->setForeground(brush);
                ui->loggerTable->item(0, iter)->setFont(font);
            }

            if (3 != iter)
            {
                ui->loggerTable->item(0, iter)->setTextAlignment(Qt::AlignCenter);
            }
            else
            {
                ui->loggerTable->item(0, iter)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
        }
    }

    ui->loggerTable->item(0, 0)->setText(QString::fromStdString(log->timeData.time));
    ui->loggerTable->item(0, 1)->setText(getQStringSource(log->source));
    ui->loggerTable->item(0, 2)->setText(getQStringSeverity(log->severity));
    ui->loggerTable->item(0, 3)->setText(QString::fromStdString(log->log));

    if (ELogSeverity_Debug == log->severity)
    {
        ++mNumberOfDebugLogs;
    }
    ++mNumberOfLogs;
}

void MainWindow::removeTheOldestDebugLogFromLoggerTable()
{
    auto numberOfRows = ui->loggerTable->rowCount();
    auto qString = QString("Debug");
    for (int iter = numberOfRows - 1; iter >= 0; --iter)
    {
        if (qString == ui->loggerTable->item(iter, 2)->text())
        {
            //ui->loggerTable->removeRow(iter);
            QMetaObject::invokeMethod(ui->loggerTable, "removeRow", Qt::QueuedConnection, Q_ARG(int, iter));
            --mNumberOfDebugLogs;
            break;
        }
    }
}

const QColor & MainWindow::getColorForLogSeverity(ELogSeverity severity)
{
    static std::map<ELogSeverity, QColor> severityColorMap = decltype(severityColorMap)
    {
        { ELogSeverity_Debug, QColor(0, 211, 211) },
        { ELogSeverity_Info, QColor(250, 250, 250) },
        { ELogSeverity_Warning, QColor(0, 191, 255) },
        { ELogSeverity_Error, QColor(255, 69, 0) }
    };

    return severityColorMap[severity];
}

const QString & MainWindow::getQStringSource(Logger::LogSource source)
{
    static std::map<Logger::LogSource, QString> sourceStringMap = decltype(sourceStringMap)
    {
        { Logger::LogSource::Nucleo, "Nucleo" },
        { Logger::LogSource::Raspberry, "Raspberry" }
    };

    return sourceStringMap[source];
}

const QString & MainWindow::getQStringSeverity(ELogSeverity severity)
{
    static std::map<ELogSeverity, QString> severityStringMap = decltype(severityStringMap)
    {
        { ELogSeverity_Debug, "Debug" },
        { ELogSeverity_Info, "Info" },
        { ELogSeverity_Warning, "Warning" },
        { ELogSeverity_Error, "Error" }
    };

    return severityStringMap[severity];
}

void MainWindow::setupFooterTable()
{
    QFont font("Arial", 12);
    QBrush blackBrush;
    blackBrush.setColor(QColor(250, 250, 250));

    ui->footerDataTable->insertRow(0);
    //QMetaObject::invokeMethod(ui->footerDataTable, "insertRow", Qt::QueuedConnection, Q_ARG(int, 0));

    {
        ui->footerDataTable->setColumnWidth(0, 70);
        ui->footerDataTable->setItem(0, 0, new QTableWidgetItem());
        //QMetaObject::invokeMethod(ui->footerDataTable, "setColumnWidth", Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(int, 70));
        //QMetaObject::invokeMethod(ui->footerDataTable, "setItem", Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(int, 70), Q_ARG(QTableWidgetItem*, new QTableWidgetItem()));
        ui->footerDataTable->item(0, 0)->setFont(font);
        ui->footerDataTable->item(0, 0)->setForeground(blackBrush);
        //  ui->footerDataTable->item(0, 0)->setText("@bkozdras");
    }
}

void MainWindow::setupFaultsTable()
{
    std::lock_guard<std::mutex> lockGuard(mFaultsTableMtx);

    const auto nameColumnWidth = 100;
    const auto unitColumnWidth = 100;
    const auto subunitColumnWidth = 100;
    const auto idColumnWidth = 43;
    ui->faultsTable->setColumnWidth(0, idColumnWidth);
    ui->faultsTable->setColumnWidth(1, nameColumnWidth);
    ui->faultsTable->setColumnWidth(2, unitColumnWidth);
    ui->faultsTable->setColumnWidth(3, subunitColumnWidth);

    mNewFaultCallbackId = FaultManager::registerNotificationCallback
    (
        [this](std::shared_ptr<SFaultIndication> indication)
        {
            addNewFaultToFaultsTable(indication);
        },
        [this](std::shared_ptr<SFaultIndication> indication)
        {
            removeFaultFromFaultsTable(indication);
        }
    );
}

void MainWindow::addNewFaultToFaultsTable(std::shared_ptr<SFaultIndication> faultInd)
{
    std::lock_guard<std::mutex> lockGuard(mFaultsTableMtx);

    {
        ui->faultsTable->insertRow(0);
        //QMetaObject::invokeMethod(ui->faultsTable, "insertRow", Qt::QueuedConnection, Q_ARG(int, 0));
        for (auto iter = 0; 4 > iter; ++iter)
        {
            ui->faultsTable->setItem(0, iter, new QTableWidgetItem());
            //QMetaObject::invokeMethod(ui->faultsTable, "setItem", Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(int, iter), Q_ARG(QTableWidgetItem*, new QTableWidgetItem()));

            {
                QFont font("Arial", 12);
                QBrush brush;
                brush.setColor(QColor(255, 69, 0));
                ui->faultsTable->item(0, iter)->setForeground(brush);
                ui->faultsTable->item(0, iter)->setFont(font);
            }

            ui->faultsTable->item(0, iter)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
    
    ui->faultsTable->item(0, 0)->setText(QString::fromStdString(std::to_string(faultInd->faultId)));
    ui->faultsTable->item(0, 1)->setText(QString::fromStdString(ToStringConverter::getFaultId(faultInd->faultId)));
    ui->faultsTable->item(0, 2)->setText(QString::fromStdString(ToStringConverter::getUnitId(faultInd->faultyUnitId)));
    if (EUnitId_Empty != faultInd->faultySubUnitId)
    {
        ui->faultsTable->item(0, 3)->setText(QString::fromStdString(ToStringConverter::getUnitId(faultInd->faultySubUnitId)));
    }

    mStoredFaults.push_back(std::make_pair(faultInd, ui->faultsTable->item(0, 0)));
}

void MainWindow::removeFaultFromFaultsTable(std::shared_ptr<SFaultIndication> faultInd)
{
    std::lock_guard<std::mutex> lockGuard(mFaultsTableMtx);

    auto faultIter =    std::find_if
                        (
                            std::begin(mStoredFaults),
                            std::end(mStoredFaults),
                            [&faultInd](const std::pair<std::shared_ptr<SFaultIndication>, QTableWidgetItem*> & storedFault)
                            {
                                if (FaultManager::compareFaults(*faultInd, *(storedFault.first)))
                                {
                                    return true;
                                }

                                return false;
                            }
                        );

    if (std::end(mStoredFaults) != faultIter)
    {
        auto rowNumber = std::numeric_limits<int>::max();

        for (auto iter = 0; ui->faultsTable->rowCount() > iter; ++iter)
        {
            if (ui->faultsTable->item(iter, 0) == faultIter->second)
            {
                rowNumber = faultIter->second->row();
                break;
            }
        }

        mStoredFaults.erase(faultIter);

        if (std::numeric_limits<int>::max() != rowNumber)
        {
            //ui->faultsTable->removeRow(rowNumber);
            QMetaObject::invokeMethod(ui->faultsTable, "removeRow", Qt::QueuedConnection, Q_ARG(int, rowNumber));
        }
    }
}

void MainWindow::setupAutodetectionTabLabels()
{
    auto setWhiteTextColor =
        [this](QLabel* label)
        {
            label->setStyleSheet("QLabel { color:rgb(0,211,211) }");
        };

    setWhiteTextColor(ui->labelStatusAds1248Info1);
    setWhiteTextColor(ui->labelStatusAds1248Info2);
    setWhiteTextColor(ui->labelStatusDrv595Info1);
    setWhiteTextColor(ui->labelStatusDrv595Info2);
    setWhiteTextColor(ui->labelStatusLmp90100CsInfo1);
    setWhiteTextColor(ui->labelStatusLmp90100CsInfo2);
    setWhiteTextColor(ui->labelStatusLmp90100SmInfo1);
    setWhiteTextColor(ui->labelStatusLmp90100SmInfo2);
    setWhiteTextColor(ui->labelStatusMcp4716Info1);
    setWhiteTextColor(ui->labelStatusMcp4716Info2);
    setWhiteTextColor(ui->labelStatusStm32f401reInfo1);
    setWhiteTextColor(ui->labelStatusStm32f401reInfo2);
    setWhiteTextColor(ui->labelStatusRefThermocoupleInfo);
    setWhiteTextColor(ui->labelStatusThermocouple1Info);
    setWhiteTextColor(ui->labelStatusThermocouple2Info);
    setWhiteTextColor(ui->labelStatusThermocouple3Info);
    setWhiteTextColor(ui->labelStatusThermocouple4Info);
    setWhiteTextColor(ui->labelStatusRtd1Pt100SmInfo);
    setWhiteTextColor(ui->labelStatusRtd2Pt100SmInfo);
    setWhiteTextColor(ui->labelStatusRtdPt1000CsInfo);
    setWhiteTextColor(ui->labelStatusPeltierCellInfo);

    changeUnitStatus(EUnitId_ADS1248, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_DRV595, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_LMP90100ControlSystem, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_LMP90100SignalsMeasurement, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_MCP4716, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Nucleo, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_ThermocoupleReference, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Thermocouple1, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Thermocouple2, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Thermocouple3, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Thermocouple4, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Rtd1Pt100, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Rtd2Pt100, DevicePeripherals::UnitsDetector::Status::Unknown);
    changeUnitStatus(EUnitId_Peltier, DevicePeripherals::UnitsDetector::Status::Unknown);

    DevicePeripherals::UnitsDetector::registerUnitReadyNotificationCallback([this](EUnitId unitId, DevicePeripherals::UnitsDetector::Status status){ changeUnitStatus(unitId, status); });
}

void MainWindow::changeUnitStatus(EUnitId unitId, DevicePeripherals::UnitsDetector::Status status)
{
    auto* qLabel = getQLabelForUnit(unitId);
    if (qLabel)
    {
        //qLabel->setText(getQStringForUnitStatus(status));
        //qLabel->setStyleSheet(getColorForUnitStatus(status));
        QMetaObject::invokeMethod(qLabel, "setText", Qt::QueuedConnection, Q_ARG(QString, getQStringForUnitStatus(status)));
        QMetaObject::invokeMethod(qLabel, "setStyleSheet", Qt::QueuedConnection, Q_ARG(QString, getColorForUnitStatus(status)));
    }
}

QLabel* MainWindow::getQLabelForUnit(EUnitId unitId)
{
    static std::map<EUnitId, QLabel*> unitIdToQLabel = decltype(unitIdToQLabel)
    {
        { EUnitId_ADS1248, ui->labelStatusAds1248 },
        { EUnitId_DRV595, ui->labelStatusDrv595 },
        { EUnitId_LMP90100ControlSystem, ui->labelStatusLmp90100Cs },
        { EUnitId_LMP90100SignalsMeasurement, ui->labelStatusLmp90100Sm },
        { EUnitId_MCP4716, ui->labelStatusMcp4716 },
        { EUnitId_Nucleo, ui->labelStatusStm32f401re },
        { EUnitId_ThermocoupleReference, ui->labelStatusRefThermocouple },
        { EUnitId_Thermocouple1, ui->labelStatusThermocouple1 },
        { EUnitId_Thermocouple2, ui->labelStatusThermocouple2 },
        { EUnitId_Thermocouple3, ui->labelStatusThermocouple3 },
        { EUnitId_Thermocouple4, ui->labelStatusThermocouple4 },
        { EUnitId_Rtd1Pt100, ui->labelStatusRtd1Pt100Sm },
        { EUnitId_Rtd2Pt100, ui->labelStatusRtd2Pt100Sm },
        { EUnitId_RtdPt1000, ui->labelStatusRtdPt1000Cs },
        { EUnitId_Peltier, ui->labelStatusPeltierCell }
    };

    for (const auto & elem : unitIdToQLabel)
    {
        if (unitId == elem.first)
        {
            return elem.second;
        }
    }

    return nullptr;
}

const QString & MainWindow::getColorForUnitStatus(DevicePeripherals::UnitsDetector::Status status)
{
    static std::map<DevicePeripherals::UnitsDetector::Status, QString> statusToColor = decltype(statusToColor)
    {
        { DevicePeripherals::UnitsDetector::Status::Detected, "QLabel { color:rgb(128,255,0) }" },
        { DevicePeripherals::UnitsDetector::Status::Working, "QLabel { color:rgb(255,205,0) }" },
        { DevicePeripherals::UnitsDetector::Status::Lost, "QLabel { color:rgb(255,69,0) }" },
        { DevicePeripherals::UnitsDetector::Status::Unknown, "QLabel { color:rgb(0,211,211) }" }
    };

    return statusToColor[status];
}

const QString & MainWindow::getQStringForUnitStatus(DevicePeripherals::UnitsDetector::Status status)
{
    static std::map<DevicePeripherals::UnitsDetector::Status, QString> statusToQString = decltype(statusToQString)
    {
        { DevicePeripherals::UnitsDetector::Status::Detected, "Detected" },
        { DevicePeripherals::UnitsDetector::Status::Working, "Working" },
        { DevicePeripherals::UnitsDetector::Status::Lost, "Lost" },
        { DevicePeripherals::UnitsDetector::Status::Unknown, "Detection ongoing..." }
    };

    return statusToQString[status];
}

void MainWindow::setupDscDeviceDataLabels()
{
    auto setWhiteTextColor =
        [this](QLabel* label)
    {
        label->setStyleSheet("QLabel { color:rgb(0,211,211) }");
    };

    setWhiteTextColor(ui->labelControlMode);
    setWhiteTextColor(ui->labelControlModeInfo);
    setWhiteTextColor(ui->labelHeaterPowerInfo);
    setWhiteTextColor(ui->labelHeaterPower);
    setWhiteTextColor(ui->labelHeaterTemperature);
    setWhiteTextColor(ui->labelHeaterTemperatureInfo);
    setWhiteTextColor(ui->labelSPHeaterTemperature);
    setWhiteTextColor(ui->labelSPHeaterTemperatureInfo);
    setWhiteTextColor(ui->labelSampleCarrierTemperature);
    setWhiteTextColor(ui->labelSampleCarrierTemperatureInfo);
    setWhiteTextColor(ui->labelSMPCBTemperature);
    setWhiteTextColor(ui->labelSMPCBTemperatureInfo);

    DSC::DataManager::registerNewDataCallback
    (
        [this](EDataType dataType, double value)
        {
            changeDataValue(dataType, value);
        }
    );

    DSC::DataManager::registerNewControlModeCallback
    (
        [this](EControlMode controlMode)
        {
            changeControlMode(controlMode);
        }
    );
}

void MainWindow::changeControlMode(EControlMode controlMode)
{
    QMetaObject::invokeMethod(ui->labelControlMode, "setText", Qt::QueuedConnection, Q_ARG(QString, getQStringForControlMode(controlMode)));
}

const QString & MainWindow::getQStringForControlMode(EControlMode controlMode)
{
    static std::map<EControlMode, QString> modeToQString = decltype(modeToQString)
    {
        { EControlMode::NotSet, "Not Set" },
        { EControlMode::SimpleFeedback, "Simple Feedback" },
        { EControlMode::MFCFeedback, "MFC Feedback" },
        { EControlMode::OpenLoop, "Open Loop" }
    };

    return modeToQString[controlMode];
}

void MainWindow::changeDataValue(EDataType dataType, double value)
{
    auto* qLabel = getQLabelForDataType(dataType);
    if (qLabel)
    {
        QMetaObject::invokeMethod(qLabel, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDataValueToQString(dataType, value)));
    }
}

QLabel* MainWindow::getQLabelForDataType(EDataType dataType)
{
    static std::map<EDataType, QLabel*> dataTypeToQLabel = decltype(dataTypeToQLabel)
    {
        { EDataType::HeaterPower, ui->labelHeaterPower },
        { EDataType::HeaterTemperature, ui->labelHeaterTemperature },
        { EDataType::SampleCarrierTemperature, ui->labelSampleCarrierTemperature },
        { EDataType::SMPCBTemperature, ui->labelSMPCBTemperature },
        { EDataType::SPHeaterTemperature, ui->labelSPHeaterTemperature }
    };

    auto it = dataTypeToQLabel.find(dataType);
    if (std::end(dataTypeToQLabel) != it)
    {
        return it->second;
    }

    return nullptr;
}

QString MainWindow::convertDataValueToQString(EDataType dataType, double value)
{
    if (DSC::DataManager::UnknownValue == value)
    {
        return "N/A";
    }

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    QString str = QString::fromStdString(stream.str());
    if (EDataType::HeaterPower == dataType)
    {
        str += " %";
    }
    else
    {
        str += " <sup>o</sup>C";
    }
    return str;
}

void MainWindow::setupUnitsDataViewer()
{
    std::lock_guard<std::mutex> lockGuard(mUnitsDataViewerMtx);
    mIsUnitsDataViewerWorking = false;

    auto setLightGreenTextColor =
        [this](QLabel* label)
    {
        label->setStyleSheet("QLabel { color:rgb(153,255,51) }");
    };

    auto setLightRedTextColor =
        [this](QLabel* label)
    {
        label->setStyleSheet("QLabel { color:rgb(255,51,51) }");
    };

    auto setGreyTextColor =
        [this](QLabel* label)
    {
        label->setStyleSheet("QLabel { color:rgb(128,128,128) }");
    };

    auto setBlueTextColor =
        [this](QLabel* label)
    {
        label->setStyleSheet("QLabel { color:rgb(51,255,255) }");
    };

    setLightGreenTextColor(ui->labelUnitsDataSample1);
    setLightGreenTextColor(ui->labelUnitsDataSample2);
    setLightGreenTextColor(ui->labelUnitsDataSample3);
    setLightGreenTextColor(ui->labelUnitsDataSample4);
    setLightGreenTextColor(ui->labelUnitsDataRefSubst);

    setLightRedTextColor(ui->labelUnitsDataHeaterTemperature);
    setLightRedTextColor(ui->labelUnitsDataSampleCarrierTemperature);
    setLightRedTextColor(ui->labelUnitsDataSMPCBTemperature);

    setGreyTextColor(ui->labelUnitsDataFanPower);

    setBlueTextColor(ui->labelUnitsDataHeaterPower);
}

void MainWindow::unitsDataViewerStartWorking()
{
    std::lock_guard<std::mutex> lockGuard(mUnitsDataViewerMtx);

    auto startUpdatingData = 
        [this](EDataType dataType)
        {
            auto timerId = TimerManager::create
            (
                1000U,
                1000U,
                [this, dataType]()
                {
                    changeUnitsDataViewerDataValue(dataType, DSC::DataManager::getData(dataType));
                }
            );

            mUnitsDataViewerQLabelToTimerId[dataType] = timerId;
        };

    startUpdatingData(EDataType::FanPower);
    startUpdatingData(EDataType::HeaterPower);
    startUpdatingData(EDataType::HeaterTemperature);
    startUpdatingData(EDataType::SampleCarrierTemperature);
    startUpdatingData(EDataType::SMPCBTemperature);
    startUpdatingData(EDataType::ReferenceThermocouple);
    startUpdatingData(EDataType::Thermocouple1);
    startUpdatingData(EDataType::Thermocouple2);
    startUpdatingData(EDataType::Thermocouple3);
    startUpdatingData(EDataType::Thermocouple4);

    mIsUnitsDataViewerWorking = true;
}

void MainWindow::unitsDataViewerStopWorking()
{
    std::lock_guard<std::mutex> lockGuard(mUnitsDataViewerMtx);

    for (const auto timerPair : mUnitsDataViewerQLabelToTimerId)
    {
        TimerManager::destroy(timerPair.second);
    }

    mUnitsDataViewerQLabelToTimerId.clear();

    mIsUnitsDataViewerWorking = false;
}

void MainWindow::changeUnitsDataViewerDataValue(EDataType dataType, double value)
{
    std::lock_guard<std::mutex> lockGuard(mUnitsDataViewerMtx);

    auto* qLabel = getQLabelForUnitsDataViewer(dataType);
    if (qLabel)
    {
        QMetaObject::invokeMethod(qLabel, "setText", Qt::QueuedConnection, Q_ARG(QString, convertUnitsDataViewerDataValueToQString(dataType, value)));
    }
}

QLabel* MainWindow::getQLabelForUnitsDataViewer(EDataType dataType)
{
    static std::map<EDataType, QLabel*> dataTypeToQLabel = decltype(dataTypeToQLabel)
    {
        { EDataType::HeaterPower, ui->labelUnitsDataHeaterPower },
        { EDataType::HeaterTemperature, ui->labelUnitsDataHeaterTemperature },
        { EDataType::SampleCarrierTemperature, ui->labelUnitsDataSampleCarrierTemperature },
        { EDataType::SMPCBTemperature, ui->labelUnitsDataSMPCBTemperature },
        { EDataType::FanPower, ui->labelUnitsDataFanPower },
        { EDataType::ReferenceThermocouple, ui->labelUnitsDataRefSubst },
        { EDataType::Thermocouple1, ui->labelUnitsDataSample1 },
        { EDataType::Thermocouple2, ui->labelUnitsDataSample2 },
        { EDataType::Thermocouple3, ui->labelUnitsDataSample3 },
        { EDataType::Thermocouple4, ui->labelUnitsDataSample4 }
    };

    auto it = dataTypeToQLabel.find(dataType);
    if (std::end(dataTypeToQLabel) != it)
    {
        return it->second;
    }

    return nullptr;
}

QString MainWindow::convertUnitsDataViewerDataValueToQString(EDataType dataType, double value)
{
    if (DSC::DataManager::UnknownValue == value)
    {
        return "N/A";
    }

    std::stringstream stream;
    QString postFix;
    int precision;

    {
        switch (dataType)
        {
            case EDataType::ReferenceThermocouple:
            case EDataType::Thermocouple1:
            case EDataType::Thermocouple2:
            case EDataType::Thermocouple3:
            case EDataType::Thermocouple4:
            {
                precision = 3;
                postFix += ' ';
                postFix += QChar(0x3BC);
                postFix += "V";
                break;
            }

            case EDataType::FanPower :
            case EDataType::HeaterPower:
            {
                precision = 2;
                postFix = " %";
                break;
            }

            case EDataType::HeaterTemperature :
            case EDataType::SampleCarrierTemperature :
            case EDataType::SMPCBTemperature:
            {
                precision = 2;
                postFix = " <sup>o</sup>C";
                break;
            }

            default:
            {
                return "Error";
            }
        }
    }

    stream << std::fixed << std::setprecision(precision) << value;
    QString str = QString::fromStdString(stream.str());
    str += postFix;

    return str;
}

void MainWindow::setupHeaterPowerControl()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    mIsHeaterPowerControlWorking = false;

    auto setLightGreenTextColor =
        [this](QWidget* widget)
        {
            widget->setStyleSheet("QLabel { color:rgb(153,255,51) }");
        };

    auto setLightRedTextColor =
        [this](QWidget* widget)
        {
            widget->setStyleSheet("QLabel { color:rgb(255,51,51) }");
        };

    auto setBlueTextColor =
        [this](QWidget* widget)
        {
            widget->setStyleSheet("QLabel { color:rgb(51,51,255) }");
        };

    auto setYellowTextColor =
        [this](QWidget* widget)
        {
            widget->setStyleSheet("QLabel { color:rgb(255,255,0) }");
        };

    auto setFontForTunes =
        [this](QWidget* widget)
        {
            QFont font("Arial", 8);
            widget->setFont(font);
        };

    setLightRedTextColor(ui->labelHeaterPowerControlModeTemperatureInfo);
    setYellowTextColor(ui->labelHeaterPowerControlModePowerInfo);

    setLightGreenTextColor(ui->labelHeaterPowerControlErrInfo);
    setLightGreenTextColor(ui->labelHeaterPowerControlErr);
    setBlueTextColor(ui->labelHeaterPowerControlSpInfo);
    setBlueTextColor(ui->textEditHeaterPowerControlSp);
    setYellowTextColor(ui->labelHeaterPowerControlCvInfo);
    setYellowTextColor(ui->labelHeaterPowerControlCv);
    setLightRedTextColor(ui->labelHeaterPowerControlPv);
    setLightRedTextColor(ui->labelHeaterPowerControlPvInfo);

    setFontForTunes(ui->textEditHeaterPowerControlKp);
    setFontForTunes(ui->textEditHeaterPowerControlKi);
    setFontForTunes(ui->textEditHeaterPowerControlKd);
    setFontForTunes(ui->textEditHeaterPowerControlN);

    mHeaterPowerControlPlotTimer = new QTimer(this);
    QObject::connect(mHeaterPowerControlPlotTimer, SIGNAL(timeout()), this, SLOT(heaterPowerControlPlotData()));
    HeaterTemperaturePlotManager::initialize(ui->heaterTemperaturePlot, mHeaterPowerControlPlotTimer);

    QObject::connect(this, SIGNAL(signalHeaterPowerControlPlotNewSamplingCallback()), this, SLOT(heaterPowerControlPlotNewSamplingCallback()));
    QObject::connect(this, SIGNAL(signalHeaterPowerControlPlotNewControlModeCallback()), this, SLOT(heaterPowerControlPlotNewControlModeCallback()));
}

void MainWindow::heaterPowerControlStartWorking()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto startUpdatingData =
        [this](EDataType dataType)
        {
            auto timerId = TimerManager::create
            (
                1000U,
                1000U,
                [this, dataType]()
                {
                    changeHeaterPowerControlDataValue(dataType, DSC::DataManager::getData(dataType));
                }
            );

            mHeaterPowerQLabelToTimerId[dataType] = timerId;
        };

    startUpdatingData(EDataType::HeaterPower);
    startUpdatingData(EDataType::HeaterTemperature);
    startUpdatingData(EDataType::CVHeaterTemperature);
    startUpdatingData(EDataType::PVHeaterTemperature);
    startUpdatingData(EDataType::ERRHeaterTemperature);

    mHeaterPowerControlFileNewFilenameCallbackId = DSC::DataManager::registerNewUnitAttributeCallback
    (
        [this](EUnitId unitId, const std::string & attribute, const std::string & value)
        {
            if (EUnitId_Raspberry == unitId && "HeaterPowerControlDataFileName" == attribute)
            {
                heaterPowerNewFilenameCallback(value);
            }
        }
    );

    QMetaObject::invokeMethod(ui->textEditHeaterPowerControlValue, "setText", Qt::QueuedConnection, Q_ARG(QString, convertHeaterPowerToQString(DSC::DataManager::getData(EDataType::HeaterPower))));
    auto spHeaterTemperature = DSC::DataManager::getData(EDataType::SPHeaterTemperature);
    QString qSpHeaterTemperatureString = "";
    if (DSC::DataManager::UnknownValue != spHeaterTemperature)
    {
        qSpHeaterTemperatureString = convertHeaterTemperatureToQString(spHeaterTemperature);
    }
    else
    {
        qSpHeaterTemperatureString = "N/A";
    }
    QMetaObject::invokeMethod(ui->textEditHeaterPowerControlSp, "setText", Qt::QueuedConnection, Q_ARG(QString, qSpHeaterTemperatureString));

    setActiveHeaterPowerTab();
    setActiveHeaterPowerComboBoxMode();
    setActiveHeaterPowerComboBoxFileSps();
    setActiveHeaterPowerComboBoxPlotSps();
    setHeaterPowerCVSlider();

    mIsHeaterPowerControlWorking = true;
}

void MainWindow::heaterPowerControlStopWorking()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    for (const auto timerPair : mHeaterPowerQLabelToTimerId)
    {
        TimerManager::destroy(timerPair.second);
    }

    mHeaterPowerQLabelToTimerId.clear();

    DSC::DataManager::deregisterNewUnitAttributeCallback(mHeaterPowerControlFileNewFilenameCallbackId);

    mIsHeaterPowerControlWorking = false;
}

void MainWindow::changeHeaterPowerControlDataValue(EDataType dataType, double value)
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto* qLabel = getQLabelForHeaterPowerControl(dataType);
    if (qLabel)
    {
        QMetaObject::invokeMethod(qLabel, "setText", Qt::QueuedConnection, Q_ARG(QString, convertHeaterPowerControlDataValueToQString(dataType, value)));
    }
}

QLabel* MainWindow::getQLabelForHeaterPowerControl(EDataType dataType)
{
    static std::map<EDataType, QLabel*> dataTypeToQLabel = decltype(dataTypeToQLabel)
    {
        { EDataType::HeaterPower, ui->labelHeaterPowerControlModePower },
        { EDataType::HeaterTemperature, ui->labelHeaterPowerControlModeTemperature },
        { EDataType::CVHeaterTemperature, ui->labelHeaterPowerControlCv },
        { EDataType::ERRHeaterTemperature, ui->labelHeaterPowerControlErr },
        { EDataType::PVHeaterTemperature, ui->labelHeaterPowerControlPv }
    };

    auto it = dataTypeToQLabel.find(dataType);
    if (std::end(dataTypeToQLabel) != it)
    {
        return it->second;
    }

    return nullptr;
}

QString MainWindow::convertHeaterPowerControlDataValueToQString(EDataType dataType, double value)
{
    if (DSC::DataManager::UnknownValue == value)
    {
        return "N/A";
    }

    std::stringstream stream;
    QString postFix;

    switch (dataType)
    {
        case EDataType::HeaterPower:
        case EDataType::CVHeaterTemperature :
        {
            postFix = " %";
            break;
        }

        case EDataType::HeaterTemperature:
        case EDataType::ERRHeaterTemperature :
        case EDataType::PVHeaterTemperature :
        case EDataType::SPHeaterTemperature :
        {
            postFix = " <sup>o</sup>C";
            break;
        }

        default:
        {
            return QString("Invalid type!");
        }
    }

    stream << std::fixed << std::setprecision(2) << value;
    QString str = QString::fromStdString(stream.str());
    str += postFix;

    return str;
}

void MainWindow::heaterPowerNewFilenameCallback(const std::string & filename)
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    QMetaObject::invokeMethod(ui->textEditHeaterPowerFilename, "setText", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(filename)));
}

void MainWindow::setActiveHeaterPowerTab()
{
    const auto openLoopIndex = 0;
    const auto simpleFeedbackIndex = 1;

    int index = 0;

    switch (DSC::DataManager::getControlMode())
    {
        case EControlMode::OpenLoop:
        {
            index = openLoopIndex;
            break;
        }

        case EControlMode::SimpleFeedback:
        case EControlMode::MFCFeedback:
        {
            index = simpleFeedbackIndex;
            break;
        }

        default:
            break;
    }

    ui->tabWidgetHeaterPower->blockSignals(true);
    ui->tabWidgetHeaterPower->setCurrentIndex(index);
    ui->tabWidgetHeaterPower->blockSignals(false);
}

void MainWindow::setActiveHeaterPowerComboBoxMode()
{
    const auto openLoopIndex = 0;
    const auto simpleFeedbackIndex = 1;
    const auto mfcFeedbackIndex = 1;

    int index = 0;

    switch (DSC::DataManager::getControlMode())
    {
        case EControlMode::OpenLoop:
        {
            index = openLoopIndex;
            break;
        }

        case EControlMode::SimpleFeedback:
        {
            index = simpleFeedbackIndex;
            break;
        }

        case EControlMode::MFCFeedback:
        {
            index = mfcFeedbackIndex;
            break;
        }

        default:
            break;
    }

    ui->comboBoxHeaterPowerControlMode->blockSignals(true);
    ui->comboBoxHeaterPowerControlMode->setCurrentIndex(index);
    ui->comboBoxHeaterPowerControlMode->blockSignals(false);
}

void MainWindow::setActiveHeaterPowerComboBoxPlotSps()
{
    auto speed = DSC::DataManager::getData(EDataType::HeaterPowerControlPlotDataSampling);
    
    ui->comboBoxHeaterPowerPlotTemperatureSps->blockSignals(true);

    if (DSC::DataManager::UnknownValue == speed)
    {
        speed = 1.0;
        DSC::DataManager::updateData(EDataType::HeaterPowerControlPlotDataSampling, speed);
    }

    if (1.0 == speed)
    {
        ui->comboBoxHeaterPowerPlotTemperatureSps->setCurrentIndex(0);
    }
    else if (2.0 == speed)
    {
        ui->comboBoxHeaterPowerPlotTemperatureSps->setCurrentIndex(1);
    }
    else if (5.0 == speed)
    {
        ui->comboBoxHeaterPowerPlotTemperatureSps->setCurrentIndex(2);
    }
    else if (10.0 == speed)
    {
        ui->comboBoxHeaterPowerPlotTemperatureSps->setCurrentIndex(3);
    }

    ui->comboBoxHeaterPowerPlotTemperatureSps->blockSignals(false);
}

void MainWindow::setActiveHeaterPowerComboBoxFileSps()
{
    auto speed = DSC::DataManager::getData(EDataType::HeaterPowerControlFileDataSampling);

    ui->comboBoxHeaterPowerFileSps->blockSignals(true);

    if (DSC::DataManager::UnknownValue == speed)
    {
        speed = 0.5;
        DSC::DataManager::updateData(EDataType::HeaterPowerControlFileDataSampling, speed);
    }

    if (0.5 == speed)
    {
        ui->comboBoxHeaterPowerFileSps->setCurrentIndex(0);
    }
    else if (1.0 == speed)
    {
        ui->comboBoxHeaterPowerFileSps->setCurrentIndex(1);
    }
    else if (2.0 == speed)
    {
        ui->comboBoxHeaterPowerFileSps->setCurrentIndex(2);
    }
    else if (5.0 == speed)
    {
        ui->comboBoxHeaterPowerFileSps->setCurrentIndex(3);
    }
    else if (10.0 == speed)
    {
        ui->comboBoxHeaterPowerFileSps->setCurrentIndex(4);
    }

    ui->comboBoxHeaterPowerFileSps->blockSignals(false);
}

void MainWindow::setHeaterPowerCVSlider()
{
    auto heaterPower = static_cast<int>(std::round(DSC::DataManager::getData(EDataType::HeaterPower)));
    ui->horizontalSliderHeaterPowerControlValue->blockSignals(true);
    ui->horizontalSliderHeaterPowerControlValue->setValue(heaterPower);
    ui->horizontalSliderHeaterPowerControlValue->blockSignals(false);
}

QString MainWindow::convertHeaterPowerToQString(double value)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return QString::fromStdString(stream.str());
}

QString MainWindow::convertHeaterTemperatureToQString(double value)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return QString::fromStdString(stream.str());
}

void MainWindow::setupSegmentsConfigurator()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);
    mActiveSegment = 1U;

    auto setGreyTextColor =
        [this](QWidget* widget)
        {
            widget->setStyleSheet("QLabel { color:rgb(128,128,128) }");
        };

    auto setWhiteTextColor =
        [this](QWidget* widget)
        {
            widget->setStyleSheet("QLabel { color:rgb(255,255,255) }");
        };

    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousNumberInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousNumber);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousTypeInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousType);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousTemperatureStartInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousTemperatureStart);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousTemperatureStopInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousTemperatureStop);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousTemperatureStepInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousTemperatureStep);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousProgramDurationInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousProgramDuration);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousProgramDurationScale);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousDegree1);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousDegree2);
    setGreyTextColor(ui->labelSegmentsConfiguratorPreviousDegree3);

    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveNumberInfo);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveNumber);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveTypeInfo);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveTemperatureStartInfo);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveTemperatureStopInfo);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveTemperatureStepInfo);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveProgramDurationInfo);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveDegree1);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveDegree2);
    setWhiteTextColor(ui->labelSegmentsConfiguratorActiveDegree3);
    setWhiteTextColor(ui->textEditSegmentsConfiguratorActiveProgramDuration);
    setWhiteTextColor(ui->textEditSegmentsConfiguratorActiveTemperatureStart);
    setWhiteTextColor(ui->textEditSegmentsConfiguratorActiveTemperatureStep);
    setWhiteTextColor(ui->textEditSegmentsConfiguratorActiveTemperatureStop);
    setWhiteTextColor(ui->comboBoxSegmentsConfiguratorActiveScale);
    setWhiteTextColor(ui->comboBoxSegmentsConfiguratorActiveType);

    setGreyTextColor(ui->labelSegmentsConfiguratorNextNumberInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextNumber);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextTypeInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextType);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextTemperatureStartInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextTemperatureStart);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextTemperatureStopInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextTemperatureStop);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextTemperatureStepInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextTemperatureStep);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextProgramDurationInfo);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextProgramDuration);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextProgramDurationScale);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextDegree1);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextDegree2);
    setGreyTextColor(ui->labelSegmentsConfiguratorNextDegree3);

    setWhiteTextColor(ui->pushButtonSegmentsConfiguratorAddToProgram);
    setWhiteTextColor(ui->pushButtonSegmentsConfiguratorDeleteFromProgram);
    setWhiteTextColor(ui->pushButtonSegmentsConfiguratorApplyProgram);

    ui->labelSegmentsConfiguratorTickImage->hide();
    ui->comboBoxSegmentsConfiguratorActiveScale->setCurrentIndex(1);

    SegmentsProgramPlotManager::initialize(ui->segmentsConfiguratorProgramPlot);
}

void MainWindow::segmentsConfiguratorStartWorking()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);
    refreshDisplayedSegmentProgram();
}

void MainWindow::refreshDisplayedSegmentProgram()
{
    {
        // Previous

        if (1U == mActiveSegment)
        {
            ui->labelSegmentsConfiguratorPreviousNumber->setText("N/A");
            ui->labelSegmentsConfiguratorPreviousType->setText("N/A");
            ui->labelSegmentsConfiguratorPreviousTemperatureStart->setText("N/A");
            ui->labelSegmentsConfiguratorPreviousTemperatureStop->setText("N/A");
            ui->labelSegmentsConfiguratorPreviousTemperatureStep->setText("N/A");
            ui->labelSegmentsConfiguratorPreviousProgramDuration->setText("N/A");
            ui->labelSegmentsConfiguratorPreviousProgramDurationScale->setText("min.");
        }
        else
        {
            const auto & segmentData = DSC::SegmentsManager::getSegmentData(mActiveSegment - 1);

            ui->labelSegmentsConfiguratorPreviousNumber->setText(QString::fromStdString(std::to_string(segmentData.nucleoData.number)));
            ui->labelSegmentsConfiguratorPreviousType->setText(convertSegmentTypeToQString(segmentData.nucleoData.type));
            ui->labelSegmentsConfiguratorPreviousTemperatureStart->setText(convertDoubleToQString(segmentData.nucleoData.startTemperature));
            if (ESegmentType_Dynamic == segmentData.nucleoData.type)
            {
                ui->labelSegmentsConfiguratorPreviousTemperatureStop->setText(convertDoubleToQString(segmentData.nucleoData.stopTemperature));
                ui->labelSegmentsConfiguratorPreviousTemperatureStep->setText(convertDoubleToQString(segmentData.nucleoData.temperatureStep));
            }
            else
            {
                ui->labelSegmentsConfiguratorPreviousTemperatureStop->setText("N/A");
                ui->labelSegmentsConfiguratorPreviousTemperatureStep->setText("N/A");
            }
            ui->labelSegmentsConfiguratorPreviousProgramDuration->setText(convertDoubleToQString(segmentData.programDuration));
            ui->labelSegmentsConfiguratorPreviousProgramDurationScale->setText(convertTimeUnitToQString(segmentData.programDurationUnit));
        }
    }

    {
        // Active

        if (DSC::SegmentsManager::getNextFreeSegmentNumber() == mActiveSegment)
        {
            ui->labelSegmentsConfiguratorActiveNumber->setText(QString::fromStdString(std::to_string(mActiveSegment)));
          
            if ((0 == DSC::SegmentsManager::getNumberOfSegments()) && (1U == mActiveSegment))
            {
                ui->comboBoxSegmentsConfiguratorActiveType->blockSignals(true);
                ui->comboBoxSegmentsConfiguratorActiveType->setCurrentIndex(convertSegmentTypeToComboBoxIndex(ESegmentType_Dynamic));
                ui->comboBoxSegmentsConfiguratorActiveType->blockSignals(false);
                ui->textEditSegmentsConfiguratorActiveTemperatureStart->setText("25.00");
                ui->textEditSegmentsConfiguratorActiveTemperatureStop->setText("50.00");
                ui->textEditSegmentsConfiguratorActiveTemperatureStep->setText("0.1");
            }
            else
            {
                auto segmentNumber = (1U == mActiveSegment) ? mActiveSegment : (mActiveSegment - 1);
                const auto & segmentData = DSC::SegmentsManager::getSegmentData(segmentNumber);
                ui->comboBoxSegmentsConfiguratorActiveType->blockSignals(true);
                ui->comboBoxSegmentsConfiguratorActiveType->setCurrentIndex(convertSegmentTypeToComboBoxIndex(segmentData.nucleoData.type));
                ui->comboBoxSegmentsConfiguratorActiveType->blockSignals(false);
                if (ESegmentType_Dynamic == segmentData.nucleoData.type)
                {
                    ui->textEditSegmentsConfiguratorActiveTemperatureStart->setText(convertDoubleToQString(segmentData.nucleoData.stopTemperature));
                    ui->textEditSegmentsConfiguratorActiveTemperatureStop->setText(convertDoubleToQString(segmentData.nucleoData.stopTemperature));
                    ui->textEditSegmentsConfiguratorActiveTemperatureStep->setText(convertDoubleToQString(segmentData.nucleoData.temperatureStep));
                }
                else
                {
                    ui->textEditSegmentsConfiguratorActiveTemperatureStart->setText(convertDoubleToQString(segmentData.nucleoData.startTemperature));
                    ui->textEditSegmentsConfiguratorActiveTemperatureStop->setText(convertDoubleToQString(segmentData.nucleoData.startTemperature));
                    ui->textEditSegmentsConfiguratorActiveTemperatureStep->setText("0.1");
                }

                if (ESegmentType_Dynamic == segmentData.nucleoData.type)
                {
                    ui->textEditSegmentsConfiguratorActiveTemperatureStep->setEnabled(true);
                    ui->textEditSegmentsConfiguratorActiveTemperatureStop->setEnabled(true);
                }
                else
                {
                    ui->textEditSegmentsConfiguratorActiveTemperatureStep->setEnabled(false);
                    ui->textEditSegmentsConfiguratorActiveTemperatureStop->setEnabled(false);
                }
            }
            ui->textEditSegmentsConfiguratorActiveProgramDuration->setText("5.00");
            ui->comboBoxSegmentsConfiguratorActiveScale->setCurrentIndex(convertTimeUnitToComboBoxIndex(DSC::SegmentsManager::TimeUnit::Minutes));

            ui->labelSegmentsConfiguratorTickImage->hide();
            ui->labelSegmentsConfiguratorCrossImage->show();

            ui->pushButtonSegmentsConfiguratorAddToProgram->setEnabled(true);
        }
        else
        {
            const auto & segmentData = DSC::SegmentsManager::getSegmentData(mActiveSegment);

            ui->labelSegmentsConfiguratorActiveNumber->setText(QString::fromStdString(std::to_string(segmentData.nucleoData.number)));
            ui->comboBoxSegmentsConfiguratorActiveType->blockSignals(true);
            ui->comboBoxSegmentsConfiguratorActiveType->setCurrentIndex(convertSegmentTypeToComboBoxIndex(segmentData.nucleoData.type));
            ui->comboBoxSegmentsConfiguratorActiveType->blockSignals(false);
            if (ESegmentType_Dynamic == segmentData.nucleoData.type)
            {
                ui->textEditSegmentsConfiguratorActiveTemperatureStart->setText(convertDoubleToQString(segmentData.nucleoData.stopTemperature));
                ui->textEditSegmentsConfiguratorActiveTemperatureStop->setText(convertDoubleToQString(segmentData.nucleoData.stopTemperature));
                ui->textEditSegmentsConfiguratorActiveTemperatureStep->setText(convertDoubleToQString(segmentData.nucleoData.temperatureStep));
            }
            else
            {
                ui->textEditSegmentsConfiguratorActiveTemperatureStart->setText(convertDoubleToQString(segmentData.nucleoData.startTemperature));
                ui->textEditSegmentsConfiguratorActiveTemperatureStop->setText(convertDoubleToQString(segmentData.nucleoData.startTemperature));
                ui->textEditSegmentsConfiguratorActiveTemperatureStep->setText("0.0");
            }
            ui->textEditSegmentsConfiguratorActiveProgramDuration->setText(convertDoubleToQString(segmentData.programDuration));
            ui->comboBoxSegmentsConfiguratorActiveScale->setCurrentIndex(convertTimeUnitToComboBoxIndex(segmentData.programDurationUnit));

            ui->labelSegmentsConfiguratorCrossImage->hide();
            ui->labelSegmentsConfiguratorTickImage->show();

            ui->pushButtonSegmentsConfiguratorAddToProgram->setEnabled(false);
        }
    }

    {
        // Next

        if (DSC::SegmentsManager::getNumberOfSegments() <= mActiveSegment)
        {
            ui->labelSegmentsConfiguratorNextNumber->setText("N/A");
            ui->labelSegmentsConfiguratorNextType->setText("N/A");
            ui->labelSegmentsConfiguratorNextTemperatureStart->setText("N/A");
            ui->labelSegmentsConfiguratorNextTemperatureStop->setText("N/A");
            ui->labelSegmentsConfiguratorNextTemperatureStep->setText("N/A");
            ui->labelSegmentsConfiguratorNextProgramDuration->setText("N/A");
            ui->labelSegmentsConfiguratorNextProgramDurationScale->setText("min.");
        }
        else
        {
            const auto & segmentData = DSC::SegmentsManager::getSegmentData(mActiveSegment + 1);

            ui->labelSegmentsConfiguratorNextNumber->setText(QString::fromStdString(std::to_string(segmentData.nucleoData.number)));
            ui->labelSegmentsConfiguratorNextType->setText(convertSegmentTypeToQString(segmentData.nucleoData.type));
            ui->labelSegmentsConfiguratorNextTemperatureStart->setText(convertDoubleToQString(segmentData.nucleoData.startTemperature));
            if (ESegmentType_Dynamic == segmentData.nucleoData.type)
            {
                ui->labelSegmentsConfiguratorNextTemperatureStop->setText(convertDoubleToQString(segmentData.nucleoData.stopTemperature));
                ui->labelSegmentsConfiguratorNextTemperatureStep->setText(convertDoubleToQString(segmentData.nucleoData.temperatureStep));
            }
            else
            {
                ui->labelSegmentsConfiguratorNextTemperatureStart->setText(convertDoubleToQString(segmentData.nucleoData.startTemperature));
                ui->labelSegmentsConfiguratorNextTemperatureStep->setText("0.0");
            }
            ui->labelSegmentsConfiguratorNextProgramDuration->setText(convertDoubleToQString(segmentData.programDuration));
            ui->labelSegmentsConfiguratorNextProgramDurationScale->setText(convertTimeUnitToQString(segmentData.programDurationUnit));
        }
    }
}

QString MainWindow::convertDoubleToQString(const double & value, int precision)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return QString::fromStdString(stream.str());
}

DSC::SegmentsManager::TimeUnit MainWindow::convertComboBoxIndexToTimeUnit(int index)
{
    switch (index)
    {
        case 0 :
            return DSC::SegmentsManager::TimeUnit::Seconds;

        case 1:
            return DSC::SegmentsManager::TimeUnit::Minutes;

        case 2:
            return DSC::SegmentsManager::TimeUnit::Hours;

        default :
            break;
    }

    return DSC::SegmentsManager::TimeUnit::Seconds;
}

int MainWindow::convertTimeUnitToComboBoxIndex(DSC::SegmentsManager::TimeUnit timeUnit)
{
    switch (timeUnit)
    {
        case DSC::SegmentsManager::TimeUnit::Seconds :
            return 0;

        case DSC::SegmentsManager::TimeUnit::Minutes :
            return 1;

        case DSC::SegmentsManager::TimeUnit::Hours :
            return 2;

        default :
            break;
    }

    return 0;
}

QString MainWindow::convertTimeUnitToQString(DSC::SegmentsManager::TimeUnit timeUnit)
{
    switch (timeUnit)
    {
        case DSC::SegmentsManager::TimeUnit::Seconds:
            return "sec.";

        case DSC::SegmentsManager::TimeUnit::Minutes:
            return "min.";

        case DSC::SegmentsManager::TimeUnit::Hours:
            return "hr.";

        default:
            break;
    }

    return "";
}

ESegmentType MainWindow::convertComboBoxIndexToSegmentType(int index)
{
    if (0 == index)
    {
        return ESegmentType_Dynamic;
    }
    else
    {
        return ESegmentType_Static;
    }
}

int MainWindow::convertSegmentTypeToComboBoxIndex(ESegmentType type)
{
    if (ESegmentType_Dynamic == type)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

QString MainWindow::convertSegmentTypeToQString(ESegmentType type)
{
    if (ESegmentType_Dynamic == type)
    {
        return "Dynamic";
    }
    else
    {
        return "Static";
    }
}

void MainWindow::setupCallibrationSettings()
{
    std::lock_guard<std::mutex> lockGuard(mCallibrationSettingsMtx);
}

void MainWindow::callibrationSettingsStartWorking()
{
    std::lock_guard<std::mutex> lockGuard(mCallibrationSettingsMtx);

    auto threshold1 = DSC::DataManager::getData(EDataType::FilteringThreshold1);
    auto coefficient1 = DSC::DataManager::getData(EDataType::FilteringThreshold1Coefficient);
    auto threshold2 = DSC::DataManager::getData(EDataType::FilteringThreshold2);
    auto coefficient2 = DSC::DataManager::getData(EDataType::FilteringThreshold2Coefficient);
    auto threshold3 = DSC::DataManager::getData(EDataType::FilteringThreshold3);
    auto coefficient3 = DSC::DataManager::getData(EDataType::FilteringThreshold3Coefficient);
    auto coefficient4 = DSC::DataManager::getData(EDataType::FilteringThreshold4Coefficient);

    ui->textEditCallibrationSettingsThreshold1->setText(convertDoubleToQString(threshold1, 2));
    ui->textEditCallibrationSettingsThreshold2->setText(convertDoubleToQString(threshold2, 2));
    ui->textEditCallibrationSettingsThreshold3->setText(convertDoubleToQString(threshold3, 2));
    ui->textEditCallibrationSettingsThreshold4->setText("");

    ui->textEditCallibrationSettingsCoefficient1->setText(convertDoubleToQString(coefficient1, 6));
    ui->textEditCallibrationSettingsCoefficient2->setText(convertDoubleToQString(coefficient2, 6));
    ui->textEditCallibrationSettingsCoefficient3->setText(convertDoubleToQString(coefficient3, 6));
    ui->textEditCallibrationSettingsCoefficient4->setText(convertDoubleToQString(coefficient4, 6));
}

void MainWindow::setupDscDataView()
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    auto setBlueTextColor =
        [](QWidget* widget)
    {
        widget->setStyleSheet("QLabel { color:rgb(0,128,255) }");
    };

    auto setRedTextColor =
        [](QWidget* widget)
    {
        widget->setStyleSheet("QLabel { color:rgb(255,102,102) }");
    };

    auto setGreenTextColor =
        [](QWidget* widget)
    {
        widget->setStyleSheet("QLabel { color:rgb(0,204,204) }");
    };

    auto setDarkOrangeTextColor =
        [](QWidget* widget)
    {
        widget->setStyleSheet("QLabel { color:rgb(255,128,0) }");
    };

    auto setGreyTextColor =
        [](QWidget* widget)
    {
        widget->setStyleSheet("QLabel { color:rgb(128,128,128) }");
    };

    auto setWhiteTextColor =
        [](QWidget* widget)
    {
        widget->setStyleSheet("QLabel { color:rgb(255,255,255) }");
    };

    setWhiteTextColor(ui->pushButtonDscDataViewStartProgram);
    setWhiteTextColor(ui->pushButtonDscDataViewStopProgram);

    setWhiteTextColor(ui->labelDscDataViewFilename);
    setWhiteTextColor(ui->labelDscDataViewDataSampling);

    setWhiteTextColor(ui->labelDscDataViewActiveNumberInfo);
    setWhiteTextColor(ui->labelDscDataViewActiveNumber);
    setWhiteTextColor(ui->labelDscDataViewActiveTypeInfo);
    setWhiteTextColor(ui->labelDscDataViewActiveType);
    setWhiteTextColor(ui->labelDscDataViewActiveTemperatureStartInfo);
    setWhiteTextColor(ui->labelDscDataViewActiveTemperatureStart);
    setWhiteTextColor(ui->labelDscDataViewActiveTemperatureStopInfo);
    setWhiteTextColor(ui->labelDscDataViewActiveTemperatureStop);
    setWhiteTextColor(ui->labelDscDataViewActiveProgramDurationInfo);
    setWhiteTextColor(ui->labelDscDataViewActiveProgramDuration);
    setWhiteTextColor(ui->labelDscDataViewActiveProgramDurationScale);
    setWhiteTextColor(ui->labelDscDataViewActiveDegree1);
    setWhiteTextColor(ui->labelDscDataViewActiveDegree2);

    setGreyTextColor(ui->labelDscDataViewNextNumberInfo);
    setGreyTextColor(ui->labelDscDataViewNextNumber);
    setGreyTextColor(ui->labelDscDataViewNextTypeInfo);
    setGreyTextColor(ui->labelDscDataViewNextType);
    setGreyTextColor(ui->labelDscDataViewNextTemperatureStartInfo);
    setGreyTextColor(ui->labelDscDataViewNextTemperatureStart);
    setGreyTextColor(ui->labelDscDataViewNextTemperatureStopInfo);
    setGreyTextColor(ui->labelDscDataViewNextTemperatureStop);
    setGreyTextColor(ui->labelDscDataViewNextProgramDurationInfo);
    setGreyTextColor(ui->labelDscDataViewNextProgramDuration);
    setGreyTextColor(ui->labelDscDataViewNextProgramDurationScale);
    setGreyTextColor(ui->labelDscDataViewNextDegree1);
    setGreyTextColor(ui->labelDscDataViewNextDegree2);

    setBlueTextColor(ui->labelDscDataViewIdealHeaterTemperatureInfo);
    setBlueTextColor(ui->labelDscDataViewIdealHeaterTemperature);
    setRedTextColor(ui->labelDscDataViewRealHeaterTemperatureInfo);
    setRedTextColor(ui->labelDscDataViewRealHeaterTemperature);
    setGreenTextColor(ui->labelDscDataViewDifferenceErrorInfo);
    setGreenTextColor(ui->labelDscDataViewDifferenceError);

    setRedTextColor(ui->labelDscDataViewSampleCarrierTemperatureInfo);
    setRedTextColor(ui->labelDscDataViewSampleCarrierTemperature);

    setDarkOrangeTextColor(ui->labelDscDataViewSample1Info);
    setDarkOrangeTextColor(ui->labelDscDataViewSample1);
    setGreyTextColor(ui->labelDscDataViewSample1HeatFlowInfo);
    setGreyTextColor(ui->labelDscDataViewSample1HeatFlow);

    setDarkOrangeTextColor(ui->labelDscDataViewSample2Info);
    setDarkOrangeTextColor(ui->labelDscDataViewSample2);
    setGreyTextColor(ui->labelDscDataViewSample2HeatFlowInfo);
    setGreyTextColor(ui->labelDscDataViewSample2HeatFlow);

    setDarkOrangeTextColor(ui->labelDscDataViewSample3Info);
    setDarkOrangeTextColor(ui->labelDscDataViewSample3);
    setGreyTextColor(ui->labelDscDataViewSample3HeatFlowInfo);
    setGreyTextColor(ui->labelDscDataViewSample3HeatFlow);

    setDarkOrangeTextColor(ui->labelDscDataViewSample4Info);
    setDarkOrangeTextColor(ui->labelDscDataViewSample4);
    setGreyTextColor(ui->labelDscDataViewSample4HeatFlowInfo);
    setGreyTextColor(ui->labelDscDataViewSample4HeatFlow);

    mDscDataViewPlotTimer = new QTimer(this);
    QObject::connect(mDscDataViewPlotTimer, SIGNAL(timeout()), this, SLOT(dscDataViewPlotDataCallback()));

    ui->pushButtonDscDataViewStartProgram->setEnabled(false);
    ui->pushButtonDscDataViewStopProgram->setEnabled(false);

    DscDataViewPlotManager::initialize(ui->dscDataViewPlot, mDscDataViewPlotTimer);
}

void MainWindow::dscDataViewStartWorking()
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    mDscDataViewNewDataCallbackId = DSC::DataManager::registerNewDataCallback
    (
        [this](EDataType dataType, double value)
        {
            changeDscViewDataValue(dataType, value);
        }
    );

    mDscDataViewFileNewFilenameCallbackId = DSC::DataManager::registerNewUnitAttributeCallback
    (
        [this](EUnitId unitId, const std::string & attribute, const std::string & value)
        {
            if (EUnitId_Raspberry == unitId && "DscDataFileName" == attribute)
            {
                dscDataNewFilenameCallback(value);
            }
        }
    );

    auto actualRealizedSegment = DSC::DataManager::getData(EDataType::ActualRealizedSegment);
    updateSegmentsLabels(static_cast<u8>(actualRealizedSegment));
}

void MainWindow::dscDataViewStopWorking()
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    DSC::DataManager::deregisterNewDataCallback(mDscDataViewNewDataCallbackId);
    DSC::DataManager::deregisterNewUnitAttributeCallback(mDscDataViewFileNewFilenameCallbackId);
}

void MainWindow::changeDscViewDataValue(EDataType dataType, double value)
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    if (EDataType::ActualRealizedSegment == dataType)
    {
        updateSegmentsLabels(static_cast<u8>(value));
    }
    else if (EDataType::IsSegmentsProgramOngoing == dataType)
    {
        if (1.0 == value)
        {
            DSC::DataManager::updateUnitAttribute(EUnitId_Raspberry, "DscDataFileName", ui->textEditDscDataViewFilename->toPlainText().toStdString());
        }
        else
        {
            DSC::FileDataManager::stopRegisteringDscData();
            ui->pushButtonDscDataViewStartProgram->setEnabled(true);
            ui->pushButtonDscDataViewStopProgram->setEnabled(false);
        }
    }
    else
    {
        auto* qLabel = getQLabelForDscViewData(dataType);
        if (qLabel)
        {
            QMetaObject::invokeMethod(qLabel, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDscViewDataValueToQString(dataType, value)));
        }
    }
}

QLabel* MainWindow::getQLabelForDscViewData(EDataType dataType)
{
    static std::map<EDataType, QLabel*> dataTypeToQLabel = decltype(dataTypeToQLabel)
    {
        { EDataType::SPHeaterTemperature, ui->labelDscDataViewIdealHeaterTemperature },
        { EDataType::HeaterTemperature, ui->labelDscDataViewRealHeaterTemperature },
        { EDataType::ERRHeaterTemperature, ui->labelDscDataViewDifferenceError },
        { EDataType::SampleCarrierTemperature, ui->labelDscDataViewSampleCarrierTemperature },
        { EDataType::FilteredThermocouple1, ui->labelDscDataViewSample1 },
        { EDataType::FilteredThermocouple2, ui->labelDscDataViewSample2 },
        { EDataType::FilteredThermocouple3, ui->labelDscDataViewSample3 },
        { EDataType::FilteredThermocouple4, ui->labelDscDataViewSample4 }
    };

    auto it = dataTypeToQLabel.find(dataType);
    if (std::end(dataTypeToQLabel) != it)
    {
        return it->second;
    }

    return nullptr;
}

QString MainWindow::convertDscViewDataValueToQString(EDataType dataType, double value)
{
    if (DSC::DataManager::UnknownValue == value)
    {
        return "N/A";
    }

    std::stringstream stream;
    QString postFix;
    int precision = 0;
    postFix += ' ';

    switch (dataType)
    {
        case EDataType::HeaterTemperature:
        case EDataType::ERRHeaterTemperature:
        case EDataType::SPHeaterTemperature:
        case EDataType::SampleCarrierTemperature :
        {
            postFix = "<sup>o</sup>C";
            precision = 2;
            break;
        }

        case EDataType::FilteredThermocouple1:
        case EDataType::FilteredThermocouple2:
        case EDataType::FilteredThermocouple3:
        case EDataType::FilteredThermocouple4:
        {
            postFix += QChar(0x3BC);
            postFix += "V";
            precision = 3;
            break;
        }

        default:
        {
            return QString("Invalid type!");
        }
    }

    stream << std::fixed << std::setprecision(precision) << value;
    QString str = QString::fromStdString(stream.str());
    str += postFix;

    return str;
}

void MainWindow::updateSegmentsLabels(u8 number)
{
    const auto numberOfRegisteredSegments = DSC::SegmentsManager::getNumberOfSegments();
    if ((number > numberOfRegisteredSegments) || (0 == number))
    {
        return;
    }

    const auto & segmentData = DSC::SegmentsManager::getSegmentData(number);

    auto segmentNumberStr = std::to_string(segmentData.nucleoData.number) + "/" + std::to_string(static_cast<u8>(numberOfRegisteredSegments));

    QMetaObject::invokeMethod(ui->labelDscDataViewActiveNumber, "setText", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(segmentNumberStr)));
    QMetaObject::invokeMethod(ui->labelDscDataViewActiveType, "setText", Qt::QueuedConnection, Q_ARG(QString, convertSegmentTypeToQString(segmentData.nucleoData.type)));
    QMetaObject::invokeMethod(ui->labelDscDataViewActiveTemperatureStart, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDoubleToQString(segmentData.nucleoData.startTemperature)));
    if (ESegmentType_Dynamic == segmentData.nucleoData.type)
    {
        QMetaObject::invokeMethod(ui->labelDscDataViewActiveTemperatureStop, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDoubleToQString(segmentData.nucleoData.stopTemperature)));
    }
    else
    {
        QMetaObject::invokeMethod(ui->labelDscDataViewActiveTemperatureStop, "setText", Qt::QueuedConnection, Q_ARG(QString, QString("N/A")));
    }
    QMetaObject::invokeMethod(ui->labelDscDataViewActiveProgramDuration, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDoubleToQString(segmentData.programDuration)));
    QMetaObject::invokeMethod(ui->labelDscDataViewActiveProgramDurationScale, "setText", Qt::QueuedConnection, Q_ARG(QString, convertTimeUnitToQString(segmentData.programDurationUnit)));

    if (number != numberOfRegisteredSegments)
    {
        const auto & segmentData = DSC::SegmentsManager::getSegmentData(number + 1);

        auto segmentNumberStr = std::to_string(segmentData.nucleoData.number) + "/" + std::to_string(static_cast<u8>(numberOfRegisteredSegments));

        QMetaObject::invokeMethod(ui->labelDscDataViewNextNumber, "setText", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(segmentNumberStr)));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextType, "setText", Qt::QueuedConnection, Q_ARG(QString, convertSegmentTypeToQString(segmentData.nucleoData.type)));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextTemperatureStart, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDoubleToQString(segmentData.nucleoData.startTemperature)));
        if (ESegmentType_Dynamic == segmentData.nucleoData.type)
        {
            QMetaObject::invokeMethod(ui->labelDscDataViewNextTemperatureStop, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDoubleToQString(segmentData.nucleoData.stopTemperature)));
        }
        else
        {
            QMetaObject::invokeMethod(ui->labelDscDataViewNextTemperatureStop, "setText", Qt::QueuedConnection, Q_ARG(QString, QString("N/A")));
        }
        QMetaObject::invokeMethod(ui->labelDscDataViewNextProgramDuration, "setText", Qt::QueuedConnection, Q_ARG(QString, convertDoubleToQString(segmentData.programDuration)));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextProgramDurationScale, "setText", Qt::QueuedConnection, Q_ARG(QString, convertTimeUnitToQString(segmentData.programDurationUnit)));
    }
    else
    {
        QMetaObject::invokeMethod(ui->labelDscDataViewNextNumber, "setText", Qt::QueuedConnection, Q_ARG(QString, "N/A"));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextType, "setText", Qt::QueuedConnection, Q_ARG(QString, "N/A"));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextTemperatureStart, "setText", Qt::QueuedConnection, Q_ARG(QString, "N/A"));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextTemperatureStop, "setText", Qt::QueuedConnection, Q_ARG(QString, "N/A"));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextProgramDuration, "setText", Qt::QueuedConnection, Q_ARG(QString, "N/A"));
        QMetaObject::invokeMethod(ui->labelDscDataViewNextProgramDurationScale, "setText", Qt::QueuedConnection, Q_ARG(QString, "N/A"));
    }
}

void MainWindow::dscDataNewFilenameCallback(const std::string & filename)
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    QMetaObject::invokeMethod(ui->textEditDscDataViewFilename, "setText", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(filename)));
}

void MainWindow::heaterPowerControlClearDataClicked()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    HeaterTemperaturePlotManager::clearDrawnData();
}

void MainWindow::heaterPowerControlApplyValueClicked()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto requestedPower = ui->textEditHeaterPowerControlValue->toPlainText().toStdString();
    if (!Utilities::isDouble(requestedPower))
    {
        QMessageBox msgBox;
        QString message = "String " + QString::fromStdString(requestedPower) + " is not correct number value!";
        msgBox.setText(message);
        msgBox.setDefaultButton(QMessageBox::Button::Ok);
        msgBox.exec();
        return; 
    }

    auto powerNumber = std::stod(requestedPower);

    ThreadPool::submit
    (
        TaskPriority::Normal,
        [this, powerNumber]()
        {
            DSC::HeaterManager::setPowerInPercent(powerNumber);
        }
    );
}

void MainWindow::heaterPowerControlControlModeChanged()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);
    switch (ui->comboBoxHeaterPowerControlMode->currentIndex())
    {
        case 0:
        {
            DSC::HeaterManager::setPowerControlMode(EControlMode::OpenLoop);
            break;
        }

        case 1:
        {
            DSC::HeaterManager::setPowerControlMode(EControlMode::SimpleFeedback);
            break;
        }

        case 2:
        {
            DSC::HeaterManager::setPowerControlMode(EControlMode::MFCFeedback);
            break;
        }

        default :
            break;
    }
}

void MainWindow::heaterPowerControlCVSliderChanged()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto value = ui->horizontalSliderHeaterPowerControlValue->value();
    auto qStringValue = convertHeaterPowerToQString(static_cast<double>(value));

    ui->textEditHeaterPowerControlValue->blockSignals(true);
    ui->textEditHeaterPowerControlValue->setText(convertHeaterPowerToQString(static_cast<double>(value)));
    ui->textEditHeaterPowerControlValue->blockSignals(false);
}

void MainWindow::heaterPowerControlSaveDataToFileClicked()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    if (ui->checkBoxHeaterPowerSaveToFile->isChecked())
    {
        DSC::DataManager::updateUnitAttribute(EUnitId_Raspberry, "HeaterPowerControlDataFileName", ui->textEditHeaterPowerFilename->toPlainText().toStdString());
        DSC::FileDataManager::startRegisteringHeaterControlSystemData();
    }
    else
    {
        DSC::FileDataManager::stopRegisteringHeaterControlSystemData();
    }
}

void MainWindow::heaterPowerControPlotDataClicked()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    if (ui->checkBoxHeaterPowerPlotTemperature->isChecked())
    {
        mHeaterPowerControlPlotNewDataCallbackId = DSC::DataManager::registerNewDataCallback
        (
            [this](EDataType type, double value)
            {
                if (EDataType::HeaterPowerControlPlotDataSampling == type)
                {
                    emit signalHeaterPowerControlPlotNewSamplingCallback();
                }
            }
        );
        mHeaterPowerControlPlotNewModeCallbackId = DSC::DataManager::registerNewControlModeCallback
        (
            [this](EControlMode mode)
            {
                emit signalHeaterPowerControlPlotNewControlModeCallback();
            }
        );
        HeaterTemperaturePlotManager::startDrawing();
    }
    else
    {
        DSC::DataManager::deregisterNewControlModeCallback(mHeaterPowerControlPlotNewModeCallbackId);
        DSC::DataManager::deregisterNewDataCallback(mHeaterPowerControlPlotNewDataCallbackId);
        HeaterTemperaturePlotManager::stopDrawing();
    }
}

void MainWindow::heaterPowerControlPlotFrequencyChanged()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto speedString = ui->comboBoxHeaterPowerPlotTemperatureSps->currentText().toStdString();

    char chars [] = "second";
    for (unsigned int iter = 0; strlen(chars) > iter; ++iter)
    {
        speedString.erase(std::remove(speedString.begin(), speedString.end(), chars[iter]), speedString.end());
    }

    speedString.erase(std::remove_if(speedString.begin(), speedString.end(), ::isspace), speedString.end());

    auto speed = std::stod(speedString);
    DSC::DataManager::updateData(EDataType::HeaterPowerControlPlotDataSampling, speed);
}

void MainWindow::heaterPowerControlDataSaveFrequencyClicked()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto speedString = ui->comboBoxHeaterPowerFileSps->currentText().toStdString();

    char chars[] = "second";
    for (unsigned int iter = 0; strlen(chars) > iter; ++iter)
    {
        speedString.erase(std::remove(speedString.begin(), speedString.end(), chars[iter]), speedString.end());
    }

    speedString.erase(std::remove_if(speedString.begin(), speedString.end(), ::isspace), speedString.end());

    auto speed = std::stod(speedString);
    DSC::DataManager::updateData(EDataType::HeaterPowerControlFileDataSampling, speed);
}

void MainWindow::heaterPowerControlValueTextChanged()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);
    auto textValue = ui->textEditHeaterPowerControlValue->toPlainText().toStdString();
    if (Utilities::isDouble(textValue))
    {
        auto value = std::stod(textValue);
        if (0.0 <= value && 100.0 >= value)
        {
            ui->horizontalSliderHeaterPowerControlValue->blockSignals(true);
            ui->horizontalSliderHeaterPowerControlValue->setValue(static_cast<int>(std::round(value)));
            ui->horizontalSliderHeaterPowerControlValue->blockSignals(false);
        }
    }
}

void MainWindow::heaterPowerControlSetTunesClicked()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto requestedKpStr = ui->textEditHeaterPowerControlKp->toPlainText().toStdString();
    auto requestedKiStr = ui->textEditHeaterPowerControlKi->toPlainText().toStdString();
    auto requestedKdStr = ui->textEditHeaterPowerControlKd->toPlainText().toStdString();
    auto requestedNStr = ui->textEditHeaterPowerControlN->toPlainText().toStdString();

    bool result = true;
    Utilities::conditionalExecutor(result, [&requestedKpStr](){ return Utilities::isDouble(requestedKpStr); });
    Utilities::conditionalExecutor(result, [&requestedKiStr](){ return Utilities::isDouble(requestedKiStr); });
    Utilities::conditionalExecutor(result, [&requestedKdStr](){ return Utilities::isDouble(requestedKdStr); });
    Utilities::conditionalExecutor(result, [&requestedNStr](){ return Utilities::isDouble(requestedNStr); });

    if (!result)
    {
        QMessageBox msgBox;
        QString message = "At least one of applied PID tunes is not correct (NaN)!";
        msgBox.setText(message);
        msgBox.setDefaultButton(QMessageBox::Button::Ok);
        msgBox.exec();
        return;
    }

    DSC::HeaterManager::setPowerControlProcessPidTunes(std::stod(requestedKpStr), std::stod(requestedKiStr), std::stod(requestedKdStr), std::stod(requestedNStr));
}

void MainWindow::heaterPowerControlSetTemperatureClicked()
{
    std::lock_guard<std::mutex> lockGuard(mHeaterPowerControlMtx);

    auto requestedSp = ui->textEditHeaterPowerControlSp->toPlainText().toStdString();
    if (!Utilities::isDouble(requestedSp))
    {
        QMessageBox msgBox;
        QString message = "String " + QString::fromStdString(requestedSp) + " is not correct number value!";
        msgBox.setText(message);
        msgBox.setDefaultButton(QMessageBox::Button::Ok);
        msgBox.exec();
        return;
    }

    auto spNumber = std::stod(requestedSp);

    ThreadPool::submit
    (
        TaskPriority::Normal,
        [this, spNumber]()
        {
            DSC::HeaterManager::setTemperatureInFeedbackMode(spNumber);
        }
    );
}

void MainWindow::heaterPowerControlPlotData()
{
    HeaterTemperaturePlotManager::addNewDataToPlotCallback();
}

void MainWindow::heaterPowerControlPlotNewSamplingCallback()
{
    HeaterTemperaturePlotManager::newSamplingNotification();
}

void MainWindow::heaterPowerControlPlotNewControlModeCallback()
{
    HeaterTemperaturePlotManager::newControlModeNotification();
}

void MainWindow::segmentsConfiguratorAddToProgramClicked()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);

    auto requestedStartTemperature = ui->textEditSegmentsConfiguratorActiveTemperatureStart->toPlainText().toStdString();
    auto requestedStopTemperature = ui->textEditSegmentsConfiguratorActiveTemperatureStop->toPlainText().toStdString();
    auto requestedStepTemperature = ui->textEditSegmentsConfiguratorActiveTemperatureStep->toPlainText().toStdString();
    auto requestedProgramDuration = ui->textEditSegmentsConfiguratorActiveProgramDuration->toPlainText().toStdString();

    bool result = true;
    Utilities::conditionalExecutor(result, [&requestedStartTemperature](){ return Utilities::isDouble(requestedStartTemperature); });
    Utilities::conditionalExecutor(result, [&requestedStopTemperature](){ return Utilities::isDouble(requestedStopTemperature); });
    Utilities::conditionalExecutor(result, [&requestedStepTemperature](){ return Utilities::isDouble(requestedStepTemperature); });
    Utilities::conditionalExecutor(result, [&requestedProgramDuration](){ return Utilities::isDouble(requestedProgramDuration); });

    if (!result)
    {
        QMessageBox msgBox;
        QString message = "At least one of applied segment configuration is not correct (NaN)!";
        msgBox.setText(message);
        msgBox.setDefaultButton(QMessageBox::Button::Ok);
        msgBox.exec();
        return;
    }

    ESegmentType segmentType = convertComboBoxIndexToSegmentType(ui->comboBoxSegmentsConfiguratorActiveType->currentIndex());
    double startTemperature = std::stod(requestedStartTemperature);
    double stopTemperature = (ESegmentType_Dynamic == segmentType) ? stod(requestedStopTemperature) : startTemperature;
    double stepTemperature = std::stod(requestedStepTemperature);
    double programDuration = std::stod(requestedProgramDuration);
    DSC::SegmentsManager::TimeUnit programDurationTimeUnit = convertComboBoxIndexToTimeUnit(ui->comboBoxSegmentsConfiguratorActiveScale->currentIndex());

    DSC::SegmentsManager::saveSegment(segmentType, startTemperature, stopTemperature, stepTemperature, programDuration, programDurationTimeUnit);

    refreshDisplayedSegmentProgram();

    if (DSC::SegmentsManager::TimeUnit::Minutes == programDurationTimeUnit)
    {
        programDuration *= 60.0;
    }
    else if (DSC::SegmentsManager::TimeUnit::Hours == programDurationTimeUnit)
    {
        programDuration *= 3600.0;
    }

    SegmentsProgramPlotManager::addProgramToPlot(startTemperature, stopTemperature, programDuration);
}

void MainWindow::segmentsConfiguratorDeleteFromProgramClicked()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);

    QMessageBox msgBox;
    QString message = "Functionality disabled in program release.";
    msgBox.setText(message);
    msgBox.setDefaultButton(QMessageBox::Button::Ok);
    msgBox.exec();
}

void MainWindow::segmentsConfiguratorApplyProgramClicked()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);

    if (0 == DSC::SegmentsManager::getNumberOfSegments())
    {
        QMessageBox msgBox;
        QString message = "Lack of segments added to program.";
        msgBox.setText(message);
        msgBox.setDefaultButton(QMessageBox::Button::Ok);
        msgBox.exec();
        return;
    }

    ThreadPool::submit
    (
        TaskPriority::Normal,
        []()
        {
            DSC::SegmentsManager::registerAllSegments();
        }
    );

    ui->pushButtonSegmentsConfiguratorApplyProgram->setEnabled(false);
    ui->pushButtonDscDataViewStartProgram->setEnabled(true);
}

void MainWindow::segmentsConfiguratorUpClicked()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);

    if (1 == mActiveSegment)
    {
        return;
    }

    --mActiveSegment;
    refreshDisplayedSegmentProgram();
}

void MainWindow::segmentsConfiguratorDownClicked()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);

    if (DSC::SegmentsManager::getNextFreeSegmentNumber() == mActiveSegment)
    {
        return;
    }

    ++mActiveSegment;
    refreshDisplayedSegmentProgram();
}

void MainWindow::segmentsConfiguratorSegmentTypeChanged()
{
    std::lock_guard<std::mutex> lockGuard(mSegmentsConfiguratorMtx);

    auto currentIndex = ui->comboBoxSegmentsConfiguratorActiveType->currentIndex();
    if (0 == currentIndex)
    {
        ui->textEditSegmentsConfiguratorActiveTemperatureStep->setEnabled(true);
        ui->textEditSegmentsConfiguratorActiveTemperatureStop->setEnabled(true);
    }
    else
    {
        ui->textEditSegmentsConfiguratorActiveTemperatureStep->setEnabled(false);
        ui->textEditSegmentsConfiguratorActiveTemperatureStop->setEnabled(false);
    }
}

void MainWindow::callibrationSettingsUpdateFilterDataClicked()
{
    std::lock_guard<std::mutex> lockGuard(mCallibrationSettingsMtx);

    auto threshold1 = ui->textEditCallibrationSettingsThreshold1->toPlainText().toStdString();
    auto threshold2 = ui->textEditCallibrationSettingsThreshold2->toPlainText().toStdString();
    auto threshold3 = ui->textEditCallibrationSettingsThreshold3->toPlainText().toStdString();

    auto coefficient1 = ui->textEditCallibrationSettingsCoefficient1->toPlainText().toStdString();
    auto coefficient2 = ui->textEditCallibrationSettingsCoefficient2->toPlainText().toStdString();
    auto coefficient3 = ui->textEditCallibrationSettingsCoefficient3->toPlainText().toStdString();
    auto coefficient4 = ui->textEditCallibrationSettingsCoefficient4->toPlainText().toStdString();

    bool result = true;

    Utilities::conditionalExecutor(result, [threshold1](){ return Utilities::isDouble(threshold1); });
    Utilities::conditionalExecutor(result, [threshold2](){ return Utilities::isDouble(threshold2); });
    Utilities::conditionalExecutor(result, [threshold3](){ return Utilities::isDouble(threshold3); });
    Utilities::conditionalExecutor(result, [coefficient1](){ return Utilities::isDouble(coefficient1); });
    Utilities::conditionalExecutor(result, [coefficient2](){ return Utilities::isDouble(coefficient2); });
    Utilities::conditionalExecutor(result, [coefficient3](){ return Utilities::isDouble(coefficient3); });
    Utilities::conditionalExecutor(result, [coefficient4](){ return Utilities::isDouble(coefficient4); });

    if (!result)
    {
        QMessageBox msgBox;
        QString message = "At least one filter parameter is not correct number value!";
        msgBox.setText(message);
        msgBox.setDefaultButton(QMessageBox::Button::Ok);
        msgBox.exec();
        return;
    }

    DSC::DataManager::updateData(EDataType::FilteringThreshold1, std::stod(threshold1));
    DSC::DataManager::updateData(EDataType::FilteringThreshold2, std::stod(threshold2));
    DSC::DataManager::updateData(EDataType::FilteringThreshold3, std::stod(threshold3));
    DSC::DataManager::updateData(EDataType::FilteringThreshold1Coefficient, std::stod(coefficient1));
    DSC::DataManager::updateData(EDataType::FilteringThreshold2Coefficient, std::stod(coefficient2));
    DSC::DataManager::updateData(EDataType::FilteringThreshold3Coefficient, std::stod(coefficient3));
    DSC::DataManager::updateData(EDataType::FilteringThreshold4Coefficient, std::stod(coefficient4));

    ThreadPool::submit
    (
        TaskPriority::Normal,
        []()
        {
            DSC::SampleCarrierDataManager::updateFilterData();
        }
    );
}

void MainWindow::callibrationSettingsThreshold1Changed()
{
    std::string str = "if ( difference > ";
    str += ui->textEditCallibrationSettingsThreshold1->toPlainText().toStdString();
    str += " )";

    ui->labelCallibrationSettingsThreshold1Value->setText(QString::fromStdString(str));
}

void MainWindow::callibrationSettingsThreshold2Changed()
{
    std::string str = "else if ( difference > ";
    str += ui->textEditCallibrationSettingsThreshold2->toPlainText().toStdString();
    str += " )";

    ui->labelCallibrationSettingsThreshold2Value->setText(QString::fromStdString(str));
}

void MainWindow::callibrationSettingsThreshold3Changed()
{
    std::string str = "else if ( difference > ";
    str += ui->textEditCallibrationSettingsThreshold3->toPlainText().toStdString();
    str += " )";

    ui->labelCallibrationSettingsThreshold3Value->setText(QString::fromStdString(str));
}

void MainWindow::callibrationSettingsCoefficient1Changed()
{
    std::string str = "actualDscValue = ";
    std::string valueString = ui->textEditCallibrationSettingsCoefficient1->toPlainText().toStdString();
    if (Utilities::isDouble(valueString) && (1.0 == std::stod(valueString)))
    {
        str += "newDscValue";
    }
    else
    {
        str += "actualDscValue * ";
        str += valueString;
        str += " + newDscValue * ";

        if (Utilities::isDouble(valueString))
        {
            str += convertDoubleToQString(1.0 - std::stod(valueString), 6).toStdString();
        }
        else
        {
            str += "N/A";
        }
    }

    ui->labelCallibrationSettingsCoefficient1Value->setText(QString::fromStdString(str));
}

void MainWindow::callibrationSettingsCoefficient2Changed()
{
    std::string str = "actualDscValue = ";
    std::string valueString = ui->textEditCallibrationSettingsCoefficient2->toPlainText().toStdString();
    if (Utilities::isDouble(valueString) && (1.0 == std::stod(valueString)))
    {
        str += "newDscValue";
    }
    else
    {
        str += "actualDscValue * ";
        str += valueString;
        str += " + newDscValue * ";

        if (Utilities::isDouble(valueString))
        {
            str += convertDoubleToQString(1.0 - std::stod(valueString), 6).toStdString();
        }
        else
        {
            str += "N/A";
        }
    }

    ui->labelCallibrationSettingsCoefficient2Value->setText(QString::fromStdString(str));
}

void MainWindow::callibrationSettingsCoefficient3Changed()
{
    std::string str = "actualDscValue = ";
    std::string valueString = ui->textEditCallibrationSettingsCoefficient3->toPlainText().toStdString();
    if (Utilities::isDouble(valueString) && (1.0 == std::stod(valueString)))
    {
        str += "newDscValue";
    }
    else
    {
        str += "actualDscValue * ";
        str += valueString;
        str += " + newDscValue * ";

        if (Utilities::isDouble(valueString))
        {
            str += convertDoubleToQString(1.0 - std::stod(valueString), 6).toStdString();
        }
        else
        {
            str += "N/A";
        }
    }

    ui->labelCallibrationSettingsCoefficient3Value->setText(QString::fromStdString(str));
}

void MainWindow::callibrationSettingsCoefficient4Changed()
{
    std::string str = "actualDscValue = ";
    std::string valueString = ui->textEditCallibrationSettingsCoefficient4->toPlainText().toStdString();
    if (Utilities::isDouble(valueString) && (1.0 == std::stod(valueString)))
    {
        str += "newDscValue";
    }
    else
    {
        str += "actualDscValue * ";
        str += valueString;
        str += " + newDscValue * ";

        if (Utilities::isDouble(valueString))
        {
            str += convertDoubleToQString(1.0 - std::stod(valueString), 6).toStdString();
        }
        else
        {
            str += "N/A";
        }
    }

    ui->labelCallibrationSettingsCoefficient4Value->setText(QString::fromStdString(str));
}

void MainWindow::dscDataViewStartProgramClicked()
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    auto samplingValue = 0.0;
    switch (ui->comboBoxDscDataViewFileSps->currentIndex())
    {
        case 0:
        {
            samplingValue = 0.5;
            break;
        }

        case 1:
        {
            samplingValue = 1.0;
            break;
        }

        case 2:
        {
            samplingValue = 2.0;
            break;
        }

        case 3:
        {
            samplingValue = 5.0;
            break;
        }

        case 4:
        {
            samplingValue = 10.0;
            break;
        }
    }

    ThreadPool::submit
    (
        TaskPriority::High,
        [samplingValue]()
        {
            DSC::DataManager::updateData(EDataType::DscDataFileDataSampling, samplingValue);
        }
    );

    ui->pushButtonDscDataViewStartProgram->setEnabled(false);
    ui->pushButtonDscDataViewStopProgram->setEnabled(true);

    ThreadPool::submit
    (
        TaskPriority::Normal,
        []()
        {
            DSC::SegmentsManager::startProgram();
        }
    );

    DscDataViewPlotManager::startDrawing();
}

void MainWindow::dscDataViewStopProgramClicked()
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    ThreadPool::submit
    (
        TaskPriority::Normal,
        []()
        {
            DSC::SegmentsManager::stopProgram();
        }
    );

    ui->pushButtonDscDataViewStartProgram->setEnabled(true);
    ui->pushButtonDscDataViewStopProgram->setEnabled(false);

    DscDataViewPlotManager::stopDrawing();
}

void MainWindow::dscDataViewPlotDataCallback()
{
    DscDataViewPlotManager::addNewDataToPlotCallback();
}

void MainWindow::dscDataViewSaveDataToFileClicked()
{
    std::lock_guard<std::mutex> lockGuard(mDscDataViewMtx);

    if (ui->checkBoxDscDataViewSaveToFile->isChecked())
    {
        auto samplingValue = 1.0;
        switch (ui->comboBoxDscDataViewFileSps->currentIndex())
        {
            case 0:
            {
                samplingValue = 0.5;
                break;
            }

            case 1:
            {
                samplingValue = 1.0;
                break;
            }

            case 2:
            {
                samplingValue = 2.0;
                break;
            }

            case 3:
            {
                samplingValue = 5.0;
                break;
            }

            case 4:
            {
                samplingValue = 10.0;
                break;
            }
        }

        ThreadPool::submit
        (
            TaskPriority::High,
            [samplingValue]()
            {
                DSC::DataManager::updateData(EDataType::DscDataFileDataSampling, samplingValue);
            }
        );

        auto fileName = ui->textEditDscDataViewFilename->toPlainText().toStdString();
        ThreadPool::submit
        (
            TaskPriority::High,
            [fileName]()
            {
                DSC::DataManager::updateUnitAttribute(EUnitId_Raspberry, "DscDataFileName", fileName);
            }
        );

        ThreadPool::submit
        (
            TaskPriority::Normal,
            []()
            {
                DSC::FileDataManager::startRegisteringDscData();
            }
        );
    }
    else
    {
        ThreadPool::submit
        (
            TaskPriority::Normal,
            []()
            {
                DSC::FileDataManager::stopRegisteringDscData();
            }
        );
    }
}
