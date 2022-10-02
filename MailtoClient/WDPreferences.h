#ifndef WDPREFERENCES_H
#define WDPREFERENCES_H

#include <QDialog>

namespace Ui {
class WDPreferences;
}

class WDPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit WDPreferences(QWidget *parent = nullptr);
    ~WDPreferences();

private:
    Ui::WDPreferences *ui;
};

#endif // WDPREFERENCES_H
