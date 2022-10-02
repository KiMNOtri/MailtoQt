#ifndef WDADDCONTACTS_H
#define WDADDCONTACTS_H

#include <QMainWindow>
#include <QMessageBox>
#include "MTStorage.h"
#include "MTNetwork.h"
#include "MTGlobalVar.h"

namespace Ui {
class WDAddContacts;
}

class WDAddContacts : public QMainWindow
{
    Q_OBJECT

public:
    explicit WDAddContacts(QWidget *parent = nullptr);

    ~WDAddContacts();

private:
    Ui::WDAddContacts *ui;

    MTNetwork* mgr;
};

#endif // WDADDCONTACTS_H
