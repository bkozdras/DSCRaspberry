#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include <qtablewidget.h>
#include <qtimer.h>

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
#include "Core/DSC/DataManager.hpp"
#include "Core/DSC/SegmentsManager.hpp"

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

    void heaterPowerControlClearDataClicked();
    void heaterPowerControlApplyValueClicked();
    void heaterPowerControlControlModeChanged();
    void heaterPowerControlCVSliderChanged();
    void heaterPowerControlSaveDataToFileClicked();
    void heaterPowerControPlotDataClicked();
    void heaterPowerControlPlotFrequencyChanged();
    void heaterPowerControlDataSaveFrequencyClicked();
    void heaterPowerControlValueTextChanged();
    void heaterPowerControlSetTunesClicked();
    void heaterPowerControlSetTemperatureClicked();

    void heaterPowerControlPlotData();
    void heaterPowerControlPlotNewSamplingCallback();
    void heaterPowerControlPlotNewControlModeCallback();

    void segmentsConfiguratorAddToProgramClicked();
    void segmentsConfiguratorDeleteFromProgramClicked();
    void segmentsConfiguratorApplyProgramClicked();
    void segmentsConfiguratorUpClicked();
    void segmentsConfiguratorDownClicked();
    void segmentsConfiguratorSegmentTypeChanged();

    void callibrationSettingsUpdateFilterDataClicked();
    void callibrationSettingsThreshold1Changed();
    void callibrationSettingsThreshold2Changed();
    void callibrationSettingsThreshold3Changed();
    void callibrationSettingsCoefficient1Changed();
    void callibrationSettingsCoefficient2Changed();
    void callibrationSettingsCoefficient3Changed();
    void callibrationSettingsCoefficient4Changed();

    void dscDataViewStartProgramClicked();
    void dscDataViewStopProgramClicked();
    void dscDataViewPlotDataCallback();
    void dscDataViewSaveDataToFileClicked();

signals :

    void signalHeaterPowerControlPlotNewSamplingCallback();
    void signalHeaterPowerControlPlotNewControlModeCallback();

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
    std::map<EDataType, TimerManager::TimerId> mHeaterPowerQLabelToTimerId;

    void setupHeaterPowerControl();
    void heaterPowerControlStartWorking();
    void heaterPowerControlStopWorking();
    void changeHeaterPowerControlDataValue(EDataType dataType, double value);
    QLabel* getQLabelForHeaterPowerControl(EDataType dataType);
    QString convertHeaterPowerControlDataValueToQString(EDataType dataType, double value);
    void heaterPowerNewFilenameCallback(const std::string & filename);

    void setActiveHeaterPowerTab();
    void setActiveHeaterPowerComboBoxMode();
    void setActiveHeaterPowerComboBoxPlotSps();
    void setActiveHeaterPowerComboBoxFileSps();
    void setHeaterPowerCVSlider();
    QString convertHeaterPowerToQString(double value);
    QString convertHeaterTemperatureToQString(double value);
    double convertQStringToHeaterPower(const QString & qString);

    DSC::DataManager::CallbackId mHeaterPowerControlPlotNewDataCallbackId;
    DSC::DataManager::CallbackId mHeaterPowerControlPlotNewModeCallbackId;
    DSC::DataManager::CallbackId mHeaterPowerControlFileNewFilenameCallbackId;
    QTimer* mHeaterPowerControlPlotTimer;

    // Segments Configurator

    std::mutex mSegmentsConfiguratorMtx;
    u8 mActiveSegment;

    void setupSegmentsConfigurator();
    void segmentsConfiguratorStartWorking();
    void refreshDisplayedSegmentProgram();
    QString convertDoubleToQString(const double & value, int precision = 2);

    DSC::SegmentsManager::TimeUnit convertComboBoxIndexToTimeUnit(int index);
    int convertTimeUnitToComboBoxIndex(DSC::SegmentsManager::TimeUnit timeUnit);
    QString convertTimeUnitToQString(DSC::SegmentsManager::TimeUnit timeUnit);
    ESegmentType convertComboBoxIndexToSegmentType(int index);
    int convertSegmentTypeToComboBoxIndex(ESegmentType type);
    QString convertSegmentTypeToQString(ESegmentType type);

    // Callibation Settings

    std::mutex mCallibrationSettingsMtx;

    void setupCallibrationSettings();
    void callibrationSettingsStartWorking();

    // DSC Data View

    std::mutex mDscDataViewMtx;
    QTimer* mDscDataViewPlotTimer;
    DSC::DataManager::CallbackId mDscDataViewNewDataCallbackId;
    DSC::DataManager::CallbackId mDscDataViewFileNewFilenameCallbackId;

    void setupDscDataView();
    void dscDataViewStartWorking();
    void dscDataViewStopWorking();

    void changeDscViewDataValue(EDataType dataType, double value);
    QLabel* getQLabelForDscViewData(EDataType dataType);
    QString convertDscViewDataValueToQString(EDataType dataType, double value);
    void updateSegmentsLabels(u8 number);
    void dscDataNewFilenameCallback(const std::string & filename);
};

#endif // MAINWINDOW_H
