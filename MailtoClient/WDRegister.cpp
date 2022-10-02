#include "WDRegister.h"
#include "ui_WDRegister.h"

WDRegister::WDRegister(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WDRegister)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint);
    mgr = new MTNetwork();
    connect(mgr,&MTNetwork::MTSServerError,this,[=](){
        ui->tNickName->setEnabled(false);
        ui->tMail->setEnabled(false);
        ui->tPassword->setEnabled(false);
        ui->tPasswordRepeat->setEnabled(false);
        ui->tNickName->setText("");
        ui->tMail->setText("");
        ui->tPassword->setText("");
        ui->tPasswordRepeat->setText("");
        registerNotice("错误：暂时无法连接到服务器，请稍后再试");
    });
    connect(ui->btnRegister,&QPushButton::clicked,this,[=](){
        QString nickName = ui->tNickName->text();
        QString contactInfo = ui->tMail->text();
        QString passwd = ui->tPassword->text();
        QString passwdrpt = ui->tPasswordRepeat->text();

        if(nickName.isEmpty()||contactInfo.isEmpty()||passwd.isEmpty()||passwdrpt.isEmpty()){
            registerNotice("请将内容表单填写完整");
            return;
        }
        if(nickName.length()>10){
            registerNotice("用户名只能在 10 位以内");
            ui->tPassword->clear();
            ui->tPasswordRepeat->clear();
            return;
        }
        if(contactInfo.length()>30){
            registerNotice("联系方式只能在 30 位以内");
            ui->tPassword->clear();
            ui->tPasswordRepeat->clear();
            return;
        }
        if(passwd!=passwdrpt){
            registerNotice("请确定确认密码与输入密码一致");
            ui->tPassword->clear();
            ui->tPasswordRepeat->clear();
            return;
        }
        if(passwd.length()<5){
            registerNotice("密码过短，请设定一个 5 ～ 25 位的密码");
            ui->tPassword->clear();
            ui->tPasswordRepeat->clear();
            return;
        }
        if(passwd.length()>25){
            registerNotice("密码过长，请设定一个 5 ～ 25 位的密码");
            ui->tPassword->clear();
            ui->tPasswordRepeat->clear();
            return;
        }

        // 此处可以进行本地检验
        // 开始注册流程 POST 注册信息
        mgr->MTSUserRegister(nickName,contactInfo,passwd);
        //mgr->MTSServerStatusRequest();
    });

    connect(mgr,&MTNetwork::MTSUserRegisterReceive,this,[=](QString data){
        if(data=="failure"){
            QMessageBox::critical(this,"Mailto Account 注册","与服务器通信时出现未知问题，请稍后再试。");
        } else {
            QMessageBox::information(this,"Mailto Account 注册","注册成功，你的用户 ID : "+data);
        }
        this->close();
    });


    mgr->MTSServerStatusRequest();
}

void WDRegister::registerNotice(QString data)
{
    ui->lNotice->setText(data);
    QTimer::singleShot(300,this,[=](){
        labelEffect();
    });
    QTimer::singleShot(600,this,[=](){
        labelEffect();
    });

    labelEffect();
}

void WDRegister::labelEffect()
{
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    ui->lNotice->setGraphicsEffect(opacity);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacity, "opacity");
    opacityAnimation->setDuration(300);
    opacityAnimation->setStartValue(0);
    opacityAnimation->setEndValue(1);
    opacityAnimation->start();
}

WDRegister::~WDRegister()
{
    delete ui;
}
