#ifndef WGMESSAGEDIALOGWHITE_H
#define WGMESSAGEDIALOGWHITE_H

#include <QWidget>

namespace Ui {
class WGMessageDialogWhite;
}

class WGMessageDialogWhite : public QWidget
{
    Q_OBJECT

public:
    explicit WGMessageDialogWhite(QWidget *parent = nullptr);
    ~WGMessageDialogWhite();

    void setMessageTitle(QString);
    void setMessageContent(QString);

private:
    Ui::WGMessageDialogWhite *ui;
};

#endif // WGMESSAGEDIALOGWHITE_H
