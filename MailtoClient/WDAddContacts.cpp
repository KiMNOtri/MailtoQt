#include "WDAddContacts.h"
#include "ui_WDAddContacts.h"

WDAddContacts::WDAddContacts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WDAddContacts)
{
    ui->setupUi(this);

    mgr = new MTNetwork();

    connect(mgr,&MTNetwork::MTSAddContactsReceive,this,[=](QString re){
        if(re=="success"){
            QMessageBox::information(this,"好友请求发送成功","成功发送了好友请求。");
            this->close();
        }
        if(re=="wait"){
            QMessageBox::information(this,"好友请求发送成功","已经发送了好友请求，请等待对方确认。");
            this->close();
        }
        if(re=="already"){
            QMessageBox::critical(this,"添加好友失败","您已经和此用户成为好友。");
            this->close();
        }
        if(re=="failure"){
            QMessageBox::critical(this,"添加好友失败","输入的用户 ID 不存在。");
            this->close();
        }
    });

    connect(ui->btnAdd,&QPushButton::clicked,this,[=](){
        //MTStorage* st = new MTStorage();
        QString uid_r = ui->lID->text();
        QString uid_l = MTGUserID;
        if(uid_r.isEmpty()){
            QMessageBox::critical(this,"好友 ID 输入有误","请提供正确的用户 ID。");
            return;
        }
        bool number_username;
        uid_r.toDouble(&number_username);

        if(!number_username){
            QMessageBox::critical(this,"好友 ID 输入有误","用户 ID 应为纯数字。");
            return;
        }

        if(uid_r==uid_l){
            QMessageBox::critical(this,"好友 ID 输入有误","你不能和自己成为好友。");
            return;
        }
        mgr->MTSAddContacts(uid_l,uid_r);
    });
}

WDAddContacts::~WDAddContacts()
{
    delete ui;
}
