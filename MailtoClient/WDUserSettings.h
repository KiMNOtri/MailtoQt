#ifndef WDUSERSETTINGS_H
#define WDUSERSETTINGS_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>>
#include "MTGlobalVar.h"
#include "MTNetwork.h"

namespace Ui {
class WDUserSettings;
}

class WDUserSettings : public QDialog
{
    Q_OBJECT

public:
    explicit WDUserSettings(QWidget *parent = nullptr);
    ~WDUserSettings();

private:
    Ui::WDUserSettings *ui;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // WDUSERSETTINGS_H
