#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include <qtablewidget.h>

#include <memory>
#include <mutex>
#include <utility>
#include <vector>
#include "Core/DevicePeripherals/UnitsDetector.hpp"
#include "Core/Utilities/Logger.hpp"
#include "Core/FaultManagement/FaultManager.hpp"
#include "Core/DSC/EControlMode.hpp"
#include "Core/DSC/EDataType.hpp"
#include "Core/Defines/CommonDefines.hpp"

namespace Ui {
class MainWindow;
}

class InfoDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void run();
    void applicationTabWidgetChanged();

private:

    Ui::MainWindow* ui;
    InfoDialog* mInfoDialog;

    void closeEvent(QCloseEvent *event);

    // INFO DIALOG
    void openInfoDialog();
    void closeInfoDialog();

    // APPLICATION DOCK LOGGER

    void setupApplicationDockLogger();

    // DOCK LOGGER

    void setupDockLogger();

    // LOGGER TABLE

    std::mutex mLoggerTableMtx;
    u8 mNumberOfDebugLogs;
    u16 mNumberOfLogs;

    void setupLoggerTable();
    void addNewLogToLoggerTable(std::shared_ptr<Logger::StoredLog> log);
    void removeTheOldestDebugLogFromLoggerTable();
    const QColor & getColorForLogSeverity(ELogSeverity severity);
    const QString & getQStringSource(Logger::LogSource source);
    const QString & getQStringSeverity(ELogSeverity severity);

    // FOOTER TABLE

    std::mutex mFooterTableMtx;
    void setupFooterTable();

    // FAULTS TABLE

    std::mutex mFaultsTableMtx;
    FaultManager::CallbackId mNewFaultCallbackId;
    std::vector<std::pair<std::shared_ptr<SFaultIndication>, QTableWidgetItem*>> mStoredFaults;
    void setupFaultsTable();
    void addNewFaultToFaultsTable(std::shared_ptr<SFaultIndication> faultInd);
    void removeFaultFromFaultsTable(std::shared_ptr<SFaultIndication> faultInd);

    // AUTODETECTION TAB LABELS
    void setupAutodetectionTabLabels();
    void changeUnitStatus(EUnitId unitId, DevicePeripherals::UnitsDetector::Status status);
    QLabel* getQLabelForUnit(EUnitId unitId);
    const QString & getColorForUnitStatus(DevicePeripherals::UnitsDetector::Status status);
    const QString & getQStringForUnitStatus(DevicePeripherals::UnitsDetector::Status status);

    // DSC DEVICE DATA LABELS
    void setupDscDeviceDataLabels();
    void changeControlMode(EControlMode controlMode);
    const QString & getQStringForControlMode(EControlMode controlMode);
    void changeDataValue(EDataType dataType, double value);
    QLabel* getQLabelForDataType(EDataType dataType);
    QString convertDataValueToQString(EDataType dataType, double value);

    // Units Data Viewer

    std::mutex mUnitsDataViewerMtx;
    bool mIsUnitsDataViewerWorking;
    std::map<EDataType, TimerManager::TimerId> mUnitsDataViewerQLabelToTimerId;

    void setupUnitsDataViewer();
    void unitsDataViewerStartWorking();
    void unitsDataViewerStopWorking();
    void changeUnitsDataViewerDataValue(EDataType dataType, double value);
    QLabel* getQLabelForUnitsDataViewer(EDataType dataType);
    QString convertUnitsDataViewerDataValueToQString(EDataType dataType, double value);

    // Heater Power Control

    std::mutex mHeaterPowerControlMtx;
    bool mIsHeaterPowerControlWorking;

    void setupHeaterPowerControl();
};

#endif // MAINWINDOW_H
