#include "WDUserSettings.h"
#include "ui_WDUserSettings.h"

WDUserSettings::WDUserSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WDUserSettings)
{
    ui->setupUi(this);

    connect(ui->btnFinish,&QPushButton::clicked,this,[=](){
        QString unamenew = ui->lUsername->text();
        QString umailnew = ui->lMail->text();
        QString upwdold = ui->pPwd->text();
        QString upwdnew = ui->pwdNew->text();
        QString upwdnewre = ui->pwdNewRepeat->text();

        QString upd_id = MTGUserID;
        QString upd_uname = MTGUserName;
        if(!unamenew.isEmpty()){
            upd_uname = unamenew;
        }
        QString upd_umail = MTGUserMail;
        if(!umailnew.isEmpty()){
            upd_umail = umailnew;
        }
        QString upd_upwd = MTGUserPasswd;
        if(!upwdnewre.isEmpty() || !upwdold.isEmpty() || !upwdnewre.isEmpty() ){
            // 要改密码
            if(upd_upwd!=upwdold){
                QMessageBox::critical(this,"密码错误","原始密码错误，无法进行操作。",QMessageBox::Ok);
                return;
            }
            if(upwdnew!=upwdnewre){
                QMessageBox::critical(this,"密码错误","两次输入的密码不一致，请再试。",QMessageBox::Ok);
                return;
            }

            upd_upwd = upwdnew;

        }

        MTNetwork* mt = new MTNetwork();
        mt->MTSUpdateUser(upd_id,upd_uname,upd_umail,upd_upwd);
        mt->MTSUpdateUserStatus(MTGUserID,"0");
        QMessageBox::information(this,"修改资料完成","修改资料完成，请重新登陆。",QMessageBox::Ok);
        QApplication* app;
        app->exit(0);
    });
}

void WDUserSettings::closeEvent(QCloseEvent *event)
{

    event->accept();
}

WDUserSettings::~WDUserSettings()
{
    delete ui;
}
