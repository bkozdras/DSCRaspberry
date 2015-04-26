#include "InfoDialog.h"
#include "ui_InfoDialog.h"
#include "MainWindow.h"

#include "../Core/System/ThreadPool.hpp"

InfoDialog::InfoDialog(std::function<void()> closeDialogCallback, QWidget *parent) :
QDialog(parent),
ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    mCloseDialogCallback = closeDialogCallback;
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::startButtonClicked()
{
    ThreadPool::submit
    (
        TaskPriority::Normal,
        [this]()
        {
            mCloseDialogCallback();
        }
    );
}
