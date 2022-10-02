#ifndef WGMESSAGEDIALOGGREEN_H
#define WGMESSAGEDIALOGGREEN_H

#include <QWidget>

namespace Ui {
class WGMessageDialogGreen;
}

class WGMessageDialogGreen : public QWidget
{
    Q_OBJECT

public:
    explicit WGMessageDialogGreen(QWidget *parent = nullptr);
    ~WGMessageDialogGreen();

    void setMessageTitle(QString);
    void setMessageContent(QString);
private:
    Ui::WGMessageDialogGreen *ui;
};

#endif // WGMESSAGEDIALOGGREEN_H
