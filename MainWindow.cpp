#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "InfoDialog.h"
#include <QMessageBox>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <limits>
#include <map>
#include <string>
#include <sstream>
#include "../Core/FaultManagement/FaultManager.hpp"
#include "../Core/System/ThreadPool.hpp"
#include "../Core/System/DeviceConfigurator.hpp"
#include "../Core/Utilities/ToStringConverter.hpp"
#include "../Core/DevicePeripherals/UnitsDetector.hpp"
#include "../Core/DSC/IntegratedCircuitsManager.hpp"
#include "../Core/DSC/DataManager.hpp"
#include <map>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mInfoDialog = nullptr;
    ui->setupUi(this);
    
    setupDockLogger();
    setupLoggerTable();
    setupFaultsTable();
    setupFooterTable();
    setupAutodetectionTabLabels();
    setupDscDeviceDataLabels();

    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonClicked()
{
    Logger::debug("Sending message to Nucleo!");
    DSC::IntegratedCircuitsManager::setADS1248ChannelGainValue(EADS1248GainValue_4);
}

void MainWindow::run()
{
    openInfoDialog();
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
    mInfoDialog->close();
}

void MainWindow::setupApplicationDockLogger()
{
    {
        QFont font("Arial", 16);
        font.setBold(true);
        ui->applicationTabWidget->setFont(font);
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

    const auto timeColumnWidth = 60;
    const auto sourceColumnWidth = 80;
    const auto severityColumnWidth = 70;
    const auto logColumnWidth = 972;
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
        removeTheOldestDebugLogFromLoggerTable();
    }

    {
        ui->loggerTable->insertRow(0);
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
            ui->loggerTable->removeRow(iter);
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

    {
        ui->footerDataTable->setColumnWidth(0, 70);
        ui->footerDataTable->setItem(0, 0, new QTableWidgetItem());
        ui->footerDataTable->item(0, 0)->setFont(font);
        ui->footerDataTable->item(0, 0)->setForeground(blackBrush);
        ui->footerDataTable->item(0, 0)->setText("@bkozdras");
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
        for (auto iter = 0; 4 > iter; ++iter)
        {
            ui->faultsTable->setItem(0, iter, new QTableWidgetItem());

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
            ui->faultsTable->removeRow(rowNumber);
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
        qLabel->setText(getQStringForUnitStatus(status));
        qLabel->setStyleSheet(getColorForUnitStatus(status));
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
        { EControlMode::Feedback, "Feedback" },
        { EControlMode::NoControl, "No Control" },
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