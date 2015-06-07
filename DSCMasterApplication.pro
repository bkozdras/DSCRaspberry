#Generated by VisualGDB project wizard. 
#Feel free to modify any flags you want.
#Visit http://visualgdb.com/makefiles for more details.

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DSCMasterApplication
TEMPLATE = app

#By default the following file lists are updated automatically by VisualGDB.

SOURCES   += Core/DevicePeripherals/StateManager.cpp Core/DevicePeripherals/UnitsDetector.cpp Core/DSC/DataManager.cpp Core/DSC/FileDataManager.cpp Core/DSC/HeaterManager.cpp Core/DSC/IntegratedCircuitsManager.cpp Core/DSC/SampleCarrierManager.cpp Core/DSC/SegmentsManager.cpp Core/DSC/SMPCBTemperatureManager.cpp Core/FaultManagement/FaultManager.cpp Core/Nucleo/DeviceCommunicator.cpp Core/Nucleo/MessageGateway.cpp Core/RaspberryPeripherals/AsyncSerial.cpp Core/RaspberryPeripherals/GPIOClass.cpp Core/RaspberryPeripherals/GpioManager.cpp Core/RaspberryPeripherals/UartManager.cpp Core/RaspberryPeripherals/UartMessageMemoryManager.cpp Core/System/ActiveObject.cpp Core/System/DeviceConfigurator.cpp Core/System/SystemErrorsManager.cpp Core/System/ThreadPool.cpp Core/System/TimerManager.cpp Core/System/Utilities/ScopedThread.cpp Core/Utilities/Logger.cpp Core/Utilities/ToStringConverter.cpp Core/Utilities/Utilities.cpp DSCMasterApplication.cpp InfoDialog.cpp MainWindow.cpp qcustomplot.cpp WindowApp/DscDataViewPlotManager.cpp WindowApp/HeaterTemperaturePlotManager.cpp WindowApp/SegmentsProgramPlotManager.cpp
HEADERS   += Core/SharedDefines/ADS1248Types.h Core/SharedDefines/EControllerDataType.h Core/SharedDefines/EControlSystemType.h Core/SharedDefines/EFaultId.h Core/SharedDefines/EFaultIndicationState.h Core/SharedDefines/ELogSeverity.h Core/SharedDefines/EMessageId.h Core/SharedDefines/EMessagePart.h Core/SharedDefines/EPid.h Core/SharedDefines/ERegisteringDataType.h Core/SharedDefines/EUnitId.h Core/SharedDefines/LMP90100Types.h Core/SharedDefines/MessagesDefines.h Core/SharedDefines/PrinterPrefixes.h Core/SharedDefines/SControllerData.h Core/SharedDefines/SFaultIndication.h Core/SharedDefines/SPidTunes.h Core/SharedDefines/SProcessModelParameters.h Core/SharedDefines/SRTDPolynomialCoefficients.h Core/SharedDefines/SSampleCarrierData.h Core/SharedDefines/SSegmentData.h Core/SharedDefines/TMessage.h InfoDialog.h MainWindow.h qcustomplot.h
FORMS     += InfoDialog.ui MainWindow.ui
RESOURCES += qdarkstyle/style.qrc

include($$lower($$join(CONFIGNAME,,,.pro)))

CONFIG += c++14
QMAKE_CFLAGS 	+= $$COMMONFLAGS
QMAKE_CFLAGS  += -std=c++14
QMAKE_CXXFLAGS 	+= $$COMMONFLAGS
QMAKE_CXXFLAGS  += -std=c++14
QMAKE_LFLAGS 	+= $$COMMONFLAGS

OBJECTS_DIR = $$DESTDIR
MOC_DIR     = $$DESTDIR
RCC_DIR     = $$DESTDIR
UI_DIR      = $$DESTDIR
