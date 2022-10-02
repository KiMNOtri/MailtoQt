#include "WGMessageDialogWhite.h"
#include "ui_WGMessageDialogWhite.h"

WGMessageDialogWhite::WGMessageDialogWhite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WGMessageDialogWhite)
{
    ui->setupUi(this);

    ui->msgContent->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

void WGMessageDialogWhite::setMessageTitle(QString s)
{
    ui->msgTitle->setText(s);

}
void WGMessageDialogWhite::setMessageContent(QString s)
{
    ui->msgContent->setText(s);
    ui->msgContent->adjustSize();
    ui->frame->adjustSize();
}

WGMessageDialogWhite::~WGMessageDialogWhite()
{
    delete ui;
}
