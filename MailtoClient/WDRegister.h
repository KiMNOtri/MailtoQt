#ifndef WDREGISTER_H
#define WDREGISTER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include "MTNetwork.h"

namespace Ui {
class WDRegister;
}

class WDRegister : public QMainWindow
{
    Q_OBJECT

public:
    MTNetwork* mgr;

    explicit WDRegister(QWidget *parent = nullptr);
    ~WDRegister();

private:
    Ui::WDRegister *ui;
    void registerNotice(QString data);
    void labelEffect();
};

#endif // WDREGISTER_H
