#include "WGContactItem.h"
#include "ui_WGContactItem.h"

WGContactItem::WGContactItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WGContactItem)
{
    ui->setupUi(this);
}

void WGContactItem::setTitleContent(QString s)
{
    ui->lUsername->setText(s);
}

void WGContactItem::setDescriptionContent(QString s)
{
    ui->lStatus->setText(s);
}

void WGContactItem::setAvatar(WGContactItemAvatarType type)
{
    QImage *img=new QImage;
        //ui->label->setPixmap(QPixmap::fromImage(*img));
    switch(type) {
        case WGContactItemAvatarType::online:{
            img->load("://software/pic/people_online.png");
            break;
        }
    case WGContactItemAvatarType::offline:{
        img->load("://software/pic/people_offline.png");
        break;
    }
    case WGContactItemAvatarType::waiting:{
        img->load("://software/pic/people_waiting.png");
        break;
    }
    case WGContactItemAvatarType::error:{
        img->load("://software/pic/people_error.png");
        break;
    }

    default:{
        break;
    }
    }

    ui->lAvatar->setPixmap(QPixmap::fromImage(*img));
}

WGContactItem::~WGContactItem()
{
    delete ui;
}
