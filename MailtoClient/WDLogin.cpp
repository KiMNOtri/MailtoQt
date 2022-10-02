#include "WDLogin.h"
#include "ui_WDLogin.h"

WDLogin::WDLogin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WDLogin)
{

    ui->setupUi(this);

    mgr = new MTNetwork();
    st = new MTStorage();
    ui->lNetowkStatus->setVisible(false);
    QString rUsername = st->readSetting("uid");
    QString rPassword = st->readSetting("upasswd");
    setWindowFlags(Qt::WindowMinimizeButtonHint);

    if(st->readSetting("rememberMe")=="true"){
        ui->cAutoLogin->setCheckState(Qt::Checked);
        if(!rUsername.isEmpty()){
            ui->eUserName->setText(rUsername);
        }
        if(!rPassword.isEmpty()){
            ui->ePassword->setText(rPassword);
        }
    } else {
        ui->cAutoLogin->setCheckState(Qt::Unchecked);
    }

    connect(ui->btnLogin,&QPushButton::clicked,this,&WDLogin::loginProcess);
    connect(ui->cAutoLogin,&QCheckBox::stateChanged,this,&WDLogin::rememberMe);
    connect(mgr,&MTNetwork::MTSServerError,this,[=](){
        loginWindowResizeAnimation();
        ui->lNetowkStatus->setText("与服务器的通信出现问题，请稍后再试。");
    });
    connect(ui->btnRegister,&QPushButton::clicked,this,[=](){ (new WDRegister())->show(); });
    connect(mgr,&MTNetwork::MTSUserLoginReceive,this,[=](QString repo){

        // 登陆信息处理
        if(repo=="success"){
            saveLoginForm();
            loginWindowResizeAnimation();
            //QMessageBox::information(this, "登陆状态","登陆成功！");
            ui->lNetowkStatus->setText("登陆成功。");
            WDContacts* c =new WDContacts();
            c->show();
            this->close();
        }
        if(repo=="incorrect"){
            loginWindowResizeAnimation();
            ui->lNetowkStatus->setText("用户名或密码错误。");
            // QMessageBox::critical(this, "登陆状态","输入密码不正确！");
        }
        if(repo=="notfound"){
            loginWindowResizeAnimation();
            ui->lNetowkStatus->setText("用户名或密码错误。");
        }
    });

}

void WDLogin::loginProcess()
{
    qDebug() << "Clicked Login Button > Go Login Process";

    QString username = ui->eUserName->text();
    QString passwd = ui->ePassword->text();



    if(username.isEmpty()||passwd.isEmpty()){
        loginWindowResizeAnimation();
        ui->lNetowkStatus->setText("请将登陆信息填写完整。");
        return;
    }
    bool number_username;
    username.toDouble(&number_username);
    if(!number_username){
        loginWindowResizeAnimation();
        ui->lNetowkStatus->setText("用户 ID 应为纯数字。");
        return;
    }


    qDebug() << "Username : "<< username << " ,Password : "<<passwd;

    // Login Process

    mgr->MTSUserLogin(username,passwd);

}

void WDLogin::rememberMe(int reply)
{

    qDebug() << "Remember me : " << reply;
    if(reply==2){
        st->writeSetting("rememberMe","true");
    } else {
        st->writeSetting("rememberMe","false");
    }
}

void WDLogin::saveLoginForm()
{
    QString username = ui->eUserName->text();
    QString passwd = ui->ePassword->text();
    st->writeSetting("uid",username);
    st->writeSetting("upasswd",passwd);

    MTGUserID = username;
    MTGUserPasswd = passwd;
}

void WDLogin::loginWindowResizeAnimation()
{
    if(resizeAnimation==true){
        return;
    }
    qDebug() << "Resize Animation ...";
    this->setMaximumHeight(height()+70);

    // 动画对象
    QPropertyAnimation * pWidgetProcessUp = new QPropertyAnimation(this, "geometry");

    pWidgetProcessUp->setStartValue(geometry());
    pWidgetProcessUp->setEndValue(QRect(geometry().x(), geometry().y(), width(), height()+70));
    pWidgetProcessUp->setDuration(300);
    pWidgetProcessUp->setEasingCurve(QEasingCurve::Linear);
    pWidgetProcessUp->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(350,this,[this](){
        this->setMinimumHeight(height());
        this->setFixedSize(width(),height());
        ui->lNetowkStatus->setVisible(true);
        resizeAnimation = true;
    });


}

WDLogin::~WDLogin()
{
    delete ui;
}
