#pragma once

#include <QDialog>

#include <memory>
#include <mutex>
#include "Core/Utilities/Logger.hpp"
#include <functional>

namespace Ui {
    class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDialog(std::function<void()> closeDialogCallback, QWidget *parent = 0);
    ~InfoDialog();

protected slots:
    void startButtonClicked();

private:
    Ui::InfoDialog *ui;
    std::function<void()> mCloseDialogCallback;
};
