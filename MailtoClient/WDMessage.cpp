#include "WDMessage.h"
#include "ui_WDMessage.h"

WDMessage::WDMessage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WDMessage)
{
    ui->setupUi(this);

    QMessageBox::critical(this,"未知参数","未指定参数，无法载入聊天。");

    this->close();

}

WDMessage::WDMessage(QString uidr,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WDMessage)
{
    ui->setupUi(this);
    ui->tMain->setSelectionMode(QAbstractItemView::NoSelection);
    mgr = new MTNetwork();
    st = new MTStorage();
    msgData = new QList<MTMessageStruct>();
    msgGetTimer = new QTimer(this);

    //lUID = st->readSetting("uid");
    lUID = MTGUserID;
    rUID = uidr;
    //lName = st->readSetting("uname");
    lName = MTGUserName;
    rName = "Unknown";

    mgr->MTSMessageGetSender(lUID);
    mgr->MTSMessageGetReceiver(lUID);
    mgr->MTSUserLook(rUID);

    connect(mgr,&MTNetwork::MTSMessageSendReceive,this,[=](QString str){
        qDebug() << "Message Send Status < "<< str;
        if(str!="success"){
            addNoticeUIDialog("消息发送失败，请确定好友状态。");

        }
    });
    connect(ui->eMessage,&QLineEdit::returnPressed,ui->btnSend,&QPushButton::click);
    connect(mgr,&MTNetwork::MTSMessageReceiveSenderReceive,this,[=](QJsonArray a){
        // 查找我发的消息
        qDebug() << "Sender Receive Message Count < " << a.count();
        for(int i = 0 ; i < a.count() ; i++){
            QJsonObject o = a.at(i).toObject();
            QString recvID = QString::number(o.value("mreceiver").toInt(0));
            qDebug() << recvID;
            if(recvID!=rUID){
                continue;
            }
            MTMessageStruct s;

            s.receiveID = recvID;
            s.receiveName = o.value("mname").toString();
            s.sendID = this->lUID;
            s.sendName = this->lName;
            s.content = o.value("mcontent").toString();
            s.date = QDateTime::fromString(o.value("mtime").toString(),"yyyy-MM-dd hh:mm:ss");
            msgData->append(s);
        }

        addMessageList();

    });
    connect(mgr,&MTNetwork::MTSMessageReceiveReceiverReceive,this,[=](QJsonArray a){
        qDebug() << "Receiver Receive Message Count< " << a.count();
        // 查找我收到的消息
        for(int i = 0 ; i < a.count() ; i++){
            QJsonObject o = a.at(i).toObject();
            QString sID = QString::number(o.value("msender").toInt(0));
            if(sID!=rUID){
                continue;
            }
            MTMessageStruct s;
            s.sendID = sID;
            s.sendName = o.value("mname").toString();
            s.receiveID = this->lUID;
            s.receiveName = this->lName;
            s.content = o.value("mcontent").toString();
            s.date = QDateTime::fromString(o.value("mtime").toString(),"yyyy-MM-dd hh:mm:ss");
            msgData->append(s);
        }

        addMessageList();

    });
    connect(mgr,&MTNetwork::MTSUserLookReceive,this,[=](MTUserInfo i){
        ui->lContact->setText(i.uname);
        if(i.ustatus=="1"){
            ui->lStatus->setText("在线");
        } else {
            ui->lStatus->setText("离线");
        }

        //qDebug() << i.umail;
        //qDebug() << i.ustatus;
        this->setWindowTitle(i.uname);
        this->rName = i.uname;
    });
    connect(ui->btnSend,&QPushButton::clicked,this,[=](){
        sendMessageProcess();
    });

    connect(mgr,&MTNetwork::MTSMessageCountReceive,this,[=](int c){
        if(my_count==-1){
            my_count = c;
        }
        if(c==my_count){
            return;
        }
        if(c>my_count){
            qDebug() << "New Message Arrive !";
            msgData->clear();
            //ui->tMain->setText("");
            mgr->MTSMessageGetSender(lUID);
            mgr->MTSMessageGetReceiver(lUID);
            my_count = c;
        }
    });
    connect(msgGetTimer,&QTimer::timeout,[=](){
        qDebug() << "Get My Message Count ...";
        mgr->MTSMessageCountGet(lUID);
        mgr->MTSUserLook(rUID);
    });
    msgGetTimer->setInterval(5000);
    msgGetTimer->start();
}

void WDMessage::sendMessageProcess()
{
    QString content = ui->eMessage->text();
    mgr->MTSMessageSend(content,lUID,rUID);
    QTimer::singleShot(500,this,[=](){
        mgr->MTSMessageGetSender(lUID);
        mgr->MTSMessageGetReceiver(lUID);
    });
    ui->eMessage->clear();

}

void WDMessage::addMessageList()
{
    qDebug() << "Sorting Data ...";
    std::sort(msgData->begin(),msgData->end(),msgDataCompareFunction);
    QList<MTMessageStruct>::iterator it = std::unique(msgData->begin(),msgData->end(),msgDataEqualFunction);
    msgData->erase(it,msgData->end());
    QString repo = MTMessage::generateDisplayData(msgData);
    /*
    if(ui->tMain->toPlainText()==repo){
        return;
    } else {
        ui->tMain->setText(repo);
    }
    */
    ui->tMain->clear();
    for(int k=0;k<msgData->count();k++){
        addMessageUIDialog(msgData->at(k));
    }
    ui->tMain->setCurrentRow(ui->tMain->count()-1);

}

bool WDMessage::msgDataCompareFunction(const MTMessageStruct &s1,const MTMessageStruct &s2)
{
    return s1.date < s2.date;
}

bool WDMessage::msgDataEqualFunction(const MTMessageStruct& s1,const MTMessageStruct& s2)
{
    if(s1.content==s2.content && s1.date==s2.date && s1.receiveID==s2.receiveID && s1.sendID==s2.sendID){
        return true;
    } else {
        return false;
    }
}

void WDMessage::addNoticeUIDialog(QString s)
{
    QLabel *label_1 = new QLabel;
    label_1->setText(s);
    label_1->setAlignment(Qt::AlignCenter);
    QListWidgetItem *listItem1 = new QListWidgetItem();
    listItem1->setSizeHint(QSize(0, 30));
    ui->tMain->addItem(listItem1);
    ui->tMain->setItemWidget(listItem1, label_1);
}

void WDMessage::addMessageUIDialog(MTMessageStruct s)
{
    if(s.sendID==rUID && s.receiveID==lUID){
        // 收件人是我，白色泡泡
        WGMessageDialogWhite* w = new WGMessageDialogWhite();
        w->setMessageTitle(s.sendName+" "+s.date.toString("yyyy-MM-dd hh:mm:ss"));
        w->setMessageContent(s.content);
        QListWidgetItem *listItem1 = new QListWidgetItem();
        listItem1->setSizeHint(w->size());
        ui->tMain->addItem(listItem1);
        ui->tMain->setItemWidget(listItem1,w);
    }
    if(s.sendID==lUID && s.receiveID==rUID){
        // 发送人是我，绿色泡泡
        WGMessageDialogGreen* w = new WGMessageDialogGreen();
        w->setMessageTitle(s.sendName+" "+s.date.toString("yyyy-MM-dd hh:mm:ss"));
        w->setMessageContent(s.content);
        QListWidgetItem *listItem1 = new QListWidgetItem();
        listItem1->setSizeHint(w->size());
        ui->tMain->addItem(listItem1);
        ui->tMain->setItemWidget(listItem1,w);
    }
}

void WDMessage::closeEvent(QCloseEvent * event)
{
    MTMessage::messageRecordWriter(lUID,rUID,msgData);


}

WDMessage::~WDMessage()
{
    delete ui;
}
