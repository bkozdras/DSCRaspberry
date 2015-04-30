#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include <qtablewidget.h>

#include <memory>
#include <mutex>
#include <utility>
#include <vector>
#include "Core/Nucleo/UnitsDetector.hpp"
#include "Core/Utilities/Logger.hpp"

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
    void ButtonClicked();
    void run();

private:

    Ui::MainWindow* ui;
    InfoDialog* mInfoDialog;

    // INFO DIALOG
    void openInfoDialog();
    void closeInfoDialog();

    // APPLICATION DOCK LOGGER

    void setupApplicationDockLogger();

    // DOCK LOGGER

    void setupDockLogger();

    // LOGGER TABLE

    std::mutex mLoggerTableMtx;
    
    void setupLoggerTable();
    void addNewLogToLoggerTable(std::shared_ptr<Logger::StoredLog> log);
    const QColor & getColorForLogSeverity(ELogSeverity severity);
    const QString & getQStringSource(Logger::LogSource source);
    const QString & getQStringSeverity(ELogSeverity severity);

    // FOOTER TABLE

    std::mutex mFooterTableMtx;
    void setupFooterTable();

    // FAULTS TABLE

    std::mutex mFaultsTableMtx;
    std::vector<std::pair<std::shared_ptr<SFaultIndication>, QTableWidgetItem*>> mStoredFaults;
    void setupFaultsTable();
    void addNewFaultToFaultsTable(std::shared_ptr<SFaultIndication> faultInd);
    void removeFaultFromFaultsTable(std::shared_ptr<SFaultIndication> faultInd);

    // AUTODETECTION TAB LABELS
    void setupAutodetectionTabLabels();
    void changeUnitStatus(EUnitId unitId, Nucleo::UnitsDetector::Status status);
    QLabel* getQLabelForUnit(EUnitId unitId);
    const QColor & getColorForUnitStatus(Nucleo::UnitsDetector::Status status);
    const QString & getQStringForUnitStatus(Nucleo::UnitsDetector::Status status);
};

#endif // MAINWINDOW_H
