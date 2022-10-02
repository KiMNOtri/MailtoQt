#ifndef WDMESSAGE_H
#define WDMESSAGE_H

#include <QMainWindow>
#include <QMessageBox>
#include "MTNetwork.h"
#include "MTStorage.h"
#include "MTMessage.h"
#include "MTGlobalVar.h"
#include "WGMessageDialogGreen.h"
#include "WGMessageDialogWhite.h"
#include <QTimer>
#include <QCloseEvent>

namespace Ui {
class WDMessage;
}

class WDMessage : public QMainWindow
{
    Q_OBJECT

public:
    explicit WDMessage(QWidget *parent = nullptr);
    explicit WDMessage(QString uidr,QWidget *parent = nullptr);
    ~WDMessage();

    void sendMessageProcess();
    void addMessageList();
    static bool msgDataCompareFunction(const MTMessageStruct& s1,const MTMessageStruct& s2);
    static bool msgDataEqualFunction(const MTMessageStruct& s1,const MTMessageStruct& s2);

private:
    Ui::WDMessage *ui;
    MTNetwork* mgr;
    MTStorage* st;
    QTimer* msgGetTimer;
    QString lUID;
    QString rUID;
    QString lName;
    QString rName;
    int my_count = -1;
    QList<MTMessageStruct>* msgData;

    void addNoticeUIDialog(QString);
    void addMessageUIDialog(MTMessageStruct);

protected :
    void closeEvent(QCloseEvent *event);
};

#endif // WDMESSAGE_H
