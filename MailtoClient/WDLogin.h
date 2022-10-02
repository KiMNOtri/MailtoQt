#ifndef WDLOGIN_H
#define WDLOGIN_H

#include <QMainWindow>
#include <QtCore>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include "WDContacts.h"
#include "MTStorage.h"
#include "MTNetwork.h"
#include "WDRegister.h"
#include "MTGlobalVar.h"

namespace Ui {
class WDLogin;
}

class WDLogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit WDLogin(QWidget *parent = nullptr);
    ~WDLogin();

    void loginProcess();
    void rememberMe(int);

    MTStorage* st;
    MTNetwork* mgr;

private:
    Ui::WDLogin *ui;
    void saveLoginForm();
    void loginWindowResizeAnimation();
    bool resizeAnimation = false;
};

#endif // WDLOGIN_H
