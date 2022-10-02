#ifndef WDCONTACTS_H
#define WDCONTACTS_H

#include <QMainWindow>
#include "MTNetwork.h"
#include "WDSystemTrayIcon.h"
#include "MTStorage.h"
#include "WDMessage.h"
#include <QStandardItem>
#include "WDAddContacts.h"
#include <QCloseEvent>
#include "MTGlobalVar.h"
#include "WGContactItem.h"
#include <QAbstractItemView>
#include "WDPreferences.h"
#include "WDUserSettings.h"
#include "WDMessageHistory.h"
namespace Ui {
class WDContacts;
}

class WDContacts : public QMainWindow
{
    Q_OBJECT

public:
    explicit WDContacts(QWidget *parent = nullptr);
    void getContactsList();
    ~WDContacts();

private:
    Ui::WDContacts *ui;

    WDSystemTrayIcon* trayIcon;

    MTNetwork* mgr;
    MTStorage* st;

    QString my_UID;
    QString my_Name;

    QList<MTUserInfo> userInfoList;
    QStandardItemModel* listModel;
    QTimer* msgCountTimer;
    int currentcount = -1;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
    void addContactToList(MTUserInfo );
    void refreshContactsList();
    int list_select_count = -1;
};

#endif // WDCONTACTS_H
