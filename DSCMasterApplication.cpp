#include "MainWindow.h"
#include <QApplication>
#include <qfile.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qtimer.h>
#include <iostream>

#include "../Core/System/DeviceConfigurator.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DeviceConfigurator::configureEnvironment();

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

    MainWindow w;
    w.show();

    QTimer::singleShot(0, &w, SLOT(run()));

    return a.exec();
}
