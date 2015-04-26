/********************************************************************************
** Form generated from reading UI file 'InfoDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFODIALOG_H
#define UI_INFODIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_InfoDialog
{
public:
    QPushButton *startButton;
    QLabel *infoLabel;

    void setupUi(QDialog *InfoDialog)
    {
        if (InfoDialog->objectName().isEmpty())
            InfoDialog->setObjectName(QString::fromUtf8("InfoDialog"));
        InfoDialog->resize(605, 350);
        startButton = new QPushButton(InfoDialog);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(150, 230, 281, 61));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        startButton->setFont(font);
        infoLabel = new QLabel(InfoDialog);
        infoLabel->setObjectName(QString::fromUtf8("infoLabel"));
        infoLabel->setGeometry(QRect(150, 60, 341, 101));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(16);
        infoLabel->setFont(font1);

        retranslateUi(InfoDialog);
        QObject::connect(startButton, SIGNAL(clicked()), InfoDialog, SLOT(startButtonClicked()));

        QMetaObject::connectSlotsByName(InfoDialog);
    } // setupUi

    void retranslateUi(QDialog *InfoDialog)
    {
        InfoDialog->setWindowTitle(QApplication::translate("InfoDialog", "@Info", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("InfoDialog", "Start!", 0, QApplication::UnicodeUTF8));
        infoLabel->setText(QApplication::translate("InfoDialog", "<html><head/><body><p>DSC Application</p><p>@Copyright by Bartlomiej Kozdras</p><p>Wroclaw 2015</p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InfoDialog: public Ui_InfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFODIALOG_H
