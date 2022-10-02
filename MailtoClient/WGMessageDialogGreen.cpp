#include "WGMessageDialogGreen.h"
#include "ui_WGMessageDialogGreen.h"

WGMessageDialogGreen::WGMessageDialogGreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WGMessageDialogGreen)
{
    ui->setupUi(this);

    ui->msgContent->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

void WGMessageDialogGreen::setMessageTitle(QString s)
{
    ui->msgTitle->setText(s);
}
void WGMessageDialogGreen::setMessageContent(QString s)
{
    ui->msgContent->setText(s);
    //ui->msgContent->adjustSize();
    //ui->frame->adjustSize();
}

WGMessageDialogGreen::~WGMessageDialogGreen()
{
    delete ui;
}
