/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      62,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     115,   11,   11,   11, 0x09,
     121,   11,   11,   11, 0x09,
     151,   11,   11,   11, 0x09,
     188,   11,   11,   11, 0x09,
     226,   11,   11,   11, 0x09,
     265,   11,   11,   11, 0x09,
     301,   11,   11,   11, 0x09,
     343,   11,   11,   11, 0x09,
     378,   11,   11,   11, 0x09,
     419,   11,   11,   11, 0x09,
     464,   11,   11,   11, 0x09,
     501,   11,   11,   11, 0x09,
     537,   11,   11,   11, 0x09,
     579,   11,   11,   11, 0x09,
     608,   11,   11,   11, 0x09,
     652,   11,   11,   11, 0x09,
     699,   11,   11,   11, 0x09,
     728,   11,   11,   11, 0x09,
     760,   11,   11,   11, 0x09,
     798,   11,   11,   11, 0x09,
     839,   11,   11,   11, 0x09,
     891,   11,   11,   11, 0x09,
     933,   11,   11,   11, 0x09,
     980,   11,   11,   11, 0x09,
    1022,   11,   11,   11, 0x09,
    1054,   11,   11,   11, 0x09,
    1088,   11,   11,   11, 0x09,
    1129,   11,   11,   11, 0x09,
    1175,   11,   11,   11, 0x09,
    1228,   11,   11,   11, 0x09,
    1268,   11,   11,   11, 0x09,
    1308,   11,   11,   11, 0x09,
    1348,   11,   11,   11, 0x09,
    1390,   11,   11,   11, 0x09,
    1432,   11,   11,   11, 0x09,
    1474,   11,   11,   11, 0x09,
    1516,   11,   11,   11, 0x09,
    1572,   11,   11,   11, 0x09,
    1626,   11,   11,   11, 0x09,
    1659,   11,   11,   11, 0x09,
    1691,   11,   11,   11, 0x09,
    1721,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0"
    "signalHeaterPowerControlPlotNewSamplingCallback()\0"
    "signalHeaterPowerControlPlotNewControlModeCallback()\0"
    "run()\0applicationTabWidgetChanged()\0"
    "heaterPowerControlClearDataClicked()\0"
    "heaterPowerControlApplyValueClicked()\0"
    "heaterPowerControlControlModeChanged()\0"
    "heaterPowerControlCVSliderChanged()\0"
    "heaterPowerControlSaveDataToFileClicked()\0"
    "heaterPowerControPlotDataClicked()\0"
    "heaterPowerControlPlotFrequencyChanged()\0"
    "heaterPowerControlDataSaveFrequencyClicked()\0"
    "heaterPowerControlValueTextChanged()\0"
    "heaterPowerControlSetTunesClicked()\0"
    "heaterPowerControlSetTemperatureClicked()\0"
    "heaterPowerControlPlotData()\0"
    "heaterPowerControlPlotNewSamplingCallback()\0"
    "heaterPowerControlPlotNewControlModeCallback()\0"
    "testInputDataSourceChanged()\0"
    "testInputApplySettingsClicked()\0"
    "testInputCreatorDistributionChanged()\0"
    "testInputExternalFileChooseFileClicked()\0"
    "testInputExperimentInfoStartStopExperimentClicked()\0"
    "segmentsConfiguratorAddToProgramClicked()\0"
    "segmentsConfiguratorDeleteFromProgramClicked()\0"
    "segmentsConfiguratorApplyProgramClicked()\0"
    "segmentsConfiguratorUpClicked()\0"
    "segmentsConfiguratorDownClicked()\0"
    "segmentsConfiguratorSegmentTypeChanged()\0"
    "callibrationSettingsUpdateFilterDataClicked()\0"
    "callibrationSettingsUpdateAverageFilterDataClicked()\0"
    "callibrationSettingsThreshold1Changed()\0"
    "callibrationSettingsThreshold2Changed()\0"
    "callibrationSettingsThreshold3Changed()\0"
    "callibrationSettingsCoefficient1Changed()\0"
    "callibrationSettingsCoefficient2Changed()\0"
    "callibrationSettingsCoefficient3Changed()\0"
    "callibrationSettingsCoefficient4Changed()\0"
    "callibrationSettingsThresholdFilteringEnablingClicked()\0"
    "callibrationSettingsAverageFilteringEnablingClicked()\0"
    "dscDataViewStartProgramClicked()\0"
    "dscDataViewStopProgramClicked()\0"
    "dscDataViewPlotDataCallback()\0"
    "dscDataViewSaveDataToFileClicked()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->signalHeaterPowerControlPlotNewSamplingCallback(); break;
        case 1: _t->signalHeaterPowerControlPlotNewControlModeCallback(); break;
        case 2: _t->run(); break;
        case 3: _t->applicationTabWidgetChanged(); break;
        case 4: _t->heaterPowerControlClearDataClicked(); break;
        case 5: _t->heaterPowerControlApplyValueClicked(); break;
        case 6: _t->heaterPowerControlControlModeChanged(); break;
        case 7: _t->heaterPowerControlCVSliderChanged(); break;
        case 8: _t->heaterPowerControlSaveDataToFileClicked(); break;
        case 9: _t->heaterPowerControPlotDataClicked(); break;
        case 10: _t->heaterPowerControlPlotFrequencyChanged(); break;
        case 11: _t->heaterPowerControlDataSaveFrequencyClicked(); break;
        case 12: _t->heaterPowerControlValueTextChanged(); break;
        case 13: _t->heaterPowerControlSetTunesClicked(); break;
        case 14: _t->heaterPowerControlSetTemperatureClicked(); break;
        case 15: _t->heaterPowerControlPlotData(); break;
        case 16: _t->heaterPowerControlPlotNewSamplingCallback(); break;
        case 17: _t->heaterPowerControlPlotNewControlModeCallback(); break;
        case 18: _t->testInputDataSourceChanged(); break;
        case 19: _t->testInputApplySettingsClicked(); break;
        case 20: _t->testInputCreatorDistributionChanged(); break;
        case 21: _t->testInputExternalFileChooseFileClicked(); break;
        case 22: _t->testInputExperimentInfoStartStopExperimentClicked(); break;
        case 23: _t->segmentsConfiguratorAddToProgramClicked(); break;
        case 24: _t->segmentsConfiguratorDeleteFromProgramClicked(); break;
        case 25: _t->segmentsConfiguratorApplyProgramClicked(); break;
        case 26: _t->segmentsConfiguratorUpClicked(); break;
        case 27: _t->segmentsConfiguratorDownClicked(); break;
        case 28: _t->segmentsConfiguratorSegmentTypeChanged(); break;
        case 29: _t->callibrationSettingsUpdateFilterDataClicked(); break;
        case 30: _t->callibrationSettingsUpdateAverageFilterDataClicked(); break;
        case 31: _t->callibrationSettingsThreshold1Changed(); break;
        case 32: _t->callibrationSettingsThreshold2Changed(); break;
        case 33: _t->callibrationSettingsThreshold3Changed(); break;
        case 34: _t->callibrationSettingsCoefficient1Changed(); break;
        case 35: _t->callibrationSettingsCoefficient2Changed(); break;
        case 36: _t->callibrationSettingsCoefficient3Changed(); break;
        case 37: _t->callibrationSettingsCoefficient4Changed(); break;
        case 38: _t->callibrationSettingsThresholdFilteringEnablingClicked(); break;
        case 39: _t->callibrationSettingsAverageFilteringEnablingClicked(); break;
        case 40: _t->dscDataViewStartProgramClicked(); break;
        case 41: _t->dscDataViewStopProgramClicked(); break;
        case 42: _t->dscDataViewPlotDataCallback(); break;
        case 43: _t->dscDataViewSaveDataToFileClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::signalHeaterPowerControlPlotNewSamplingCallback()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MainWindow::signalHeaterPowerControlPlotNewControlModeCallback()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
