#ifndef WDMESSAGEHISTORY_H
#define WDMESSAGEHISTORY_H

#include <QMainWindow>
#include "MTMessage.h"
#include "MTGlobalVar.h"
#include <QStringListModel>
#include <QMessageBox>

namespace Ui {
class WDMessageHistory;
}

class WDMessageHistory : public QMainWindow
{
    Q_OBJECT

public:
    explicit WDMessageHistory(QWidget *parent = nullptr);
    ~WDMessageHistory();

private:
    Ui::WDMessageHistory *ui;

};

#endif // WDMESSAGEHISTORY_H
